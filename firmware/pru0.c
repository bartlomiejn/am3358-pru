#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include "resource_table_0.h"
#include "utils.h"

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT                    ((uint32_t) 1 << 30)
/*
 * The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST_SYS_EVENT       16
#define FROM_ARM_HOST_SYS_EVENT     17
/*
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
#define CHAN_NAME                   "rpmsg-pru"
#define CHAN_DESC                   "Channel 30"
#define CHAN_PORT                   30
/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK   4

#define RPMSG_MSG_SZ 396
#define CYCLE_THRESHOLD 4000000000
#define MS_PER_THRESHOLD 20000
#define CYCLES_PER_MS 200000

bool are_cycles_past_threshold(void);
void reduce_cycles_and_update_switch1(void);
void handle_switch1_p8_15_change(bool switch1_curr_p8_15);
uint8_t attempt_receive_from_arm(void);
void handle_query_from_arm(void);
void send_to_arm(char *message);

// PRU Registers
volatile register uint32_t __R30;
volatile register uint32_t __R31;

// RPMsg
volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport rpmsg_transport;
uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ], rpmsg_send_buf[RPMSG_MSG_SZ];
uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;

// Switch 1
bool switch1_last_p8_15;
uint32_t switch1_start_cycle = 0;
int32_t switch1_curr_ms = 0;
int32_t switch1_last_ms = -1;

// Switch 2
bool switch2_last_p8_21;

int main(void)
{
    // Set GPOs: P8_11 to 1 (bit 15 of R30), set P8_20 to 1 (bit 13 of R30)
    __R30 = (__R30 & (~(1 << 15))) | (1 << 15);
    __R30 = (__R30 & (~(1 << 13))) | (1 << 13);

    // Setup OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    // Reset host0 interrupt
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;

    // Setup RPMsg
    status = &resource_table.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
    pru_rpmsg_init(
        &rpmsg_transport,
        &resource_table.rpmsg_vring0,
        &resource_table.rpmsg_vring1,
        TO_ARM_HOST_SYS_EVENT,
        FROM_ARM_HOST_SYS_EVENT
    );
    while (pru_rpmsg_channel(
        RPMSG_NS_CREATE,
        &rpmsg_transport,
        CHAN_NAME,
        CHAN_DESC,
        CHAN_PORT
    ) != PRU_RPMSG_SUCCESS);

    // Get initial GPI states
    switch1_last_p8_15 = (__R31 >> 15) & 1;
    switch2_last_p8_21 = (__R31 >> 12) & 1;

    // Setup and reset cycle counter
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;

    // Run main loop
    while (true)
    {
        if (are_cycles_past_threshold())
        {
            reduce_cycles_and_update_switch1();
        }

        bool switch1_curr_p8_15 = (__R31 >> 15) & 1;
        if (switch1_curr_p8_15 != switch1_last_p8_15)
        {
            handle_switch1_p8_15_change(switch1_curr_p8_15);
        }

        // If received a host0 interrupt
        if (__R31 & HOST_INT)
        {
            // Reset the host0 interrupt
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
            while (attempt_receive_from_arm() == PRU_RPMSG_SUCCESS)
            {
                handle_query_from_arm();
            }
        }
    };
}

/// Are cycles approaching the 4 bil threshold value, which is fairly close to
/// uint32_t bounds
bool are_cycles_past_threshold(void)
{
    return PRU0_CTRL.CYCLE > CYCLE_THRESHOLD;
}

/// Reduce cycle count and update switch1 counters
void reduce_cycles_and_update_switch1(void)
{
    PRU0_CTRL.CTRL_bit.CTR_EN = 0;
    PRU0_CTRL.CYCLE -= CYCLE_THRESHOLD;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
    switch1_start_cycle = PRU0_CTRL.CYCLE;
    switch1_curr_ms += MS_PER_THRESHOLD;
}

void handle_switch1_p8_15_change(bool switch1_curr_p8_15)
{
    uint32_t curr_cycle = PRU0_CTRL.CYCLE;
    switch1_last_p8_15 = switch1_curr_p8_15;
    switch1_last_ms = switch1_curr_ms
        + ((curr_cycle - switch1_start_cycle) / CYCLES_PER_MS);
    switch1_start_cycle = curr_cycle;
    switch1_curr_ms = 0;
}

/// Attempts to receive a message over RPMsg.
uint8_t attempt_receive_from_arm(void)
{
    return pru_rpmsg_receive(
        &rpmsg_transport,
        &rpmsg_src,
        &rpmsg_dst,
        rpmsg_receive_buf,
        &rpmsg_receive_len
    );
}

static const char* switch1_id = "switch1";
static const char* switch2_id = "switch2";

bool is_switch1_id(char *str)
{
    return strncmp(str, switch1_id, strlen(switch1_id)) == 0;
}

bool is_switch2_id(char *str)
{
    return strncmp(str, switch2_id, strlen(switch2_id)) == 0;
}

/// Handles receiving a message from ARM over RPMsg.
void handle_query_from_arm(void)
{
    char message[256];
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        send_to_arm("1");
    }
    else if (is_switch2_id((char*)rpmsg_receive_buf))
    {
        send_to_arm("2");
    }
    else
    {
        send_to_arm("U");
    }

    // i32_to_string(strlen((char*)rpmsg_receive_buf), message);
    // strcat(message, " strlen, ");
    //
    // char strcmp_buf[256];
    // i32_to_string(
    //     strncmp((char*)rpmsg_receive_buf, switch1_id, strlen(switch1_id)),
    //     strcmp_buf
    // );
    // strcat(message, strcmp_buf);
    // strcat(message, " strncmp 7 chars diff");
    //
    // send_to_arm(message);
}

/// Sends message to ARM over RPMsg
void send_to_arm(char *message)
{
    pru_rpmsg_send(
        &rpmsg_transport,
        rpmsg_dst,
        rpmsg_src,
        message,
        strlen(message)
    );
}
