#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include "resource_table_0.h"

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

void set_initial_switches_state();
bool are_cycles_past_threshold();
void reduce_cycles_and_update_switch1();
void handle_switch1_p8_15_change(bool switch1_curr_p8_15);
uint8_t receive_from_arm();
void handle_query_from_arm();
void send_to_arm(char *message);
void ui32_to_string(uint32_t n, char *buffer);

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
int32_t switch1_start_cycle = 0;
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

    // Setup initial switch info state
    set_initial_switches_state();

    // Setup and reset cycle counter
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.EN = 1;
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
            // Reset the interrupt
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
            while (receive_from_arm() == PRU_RPMSG_SUCCESS)
            {
                handle_query_from_arm();
            }
        }
    };
}

/// Get initial GPI states: P8_15 (bit 15 of R31) and P8_21 (bit 12 of
/// R31)
void set_initial_switches_state()
{
    switch1_last_p8_15 = (__R31 >> 15) & 1;
    switch2_last_p8_21 = (__R31 >> 12) & 1;
}

/// Are cycles approaching the 4 bil threshold value
bool are_cycles_past_threshold()
{
    return PRU0_CTRL.CYCLE > CYCLE_THRESHOLD;
}

/// Reduce cycle count once 4 bil cycles pass, which is fairly close to
/// uint32_t bounds
void reduce_cycles_and_update_switch1()
{
    PRU0_CTRL.CYCLE -= CYCLE_THRESHOLD;
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

/// Attempts to receive a message over RPMsg
uint8_t receive_from_arm()
{
    return pru_rpmsg_receive(
        &rpmsg_transport,
        &rpmsg_src,
        &rpmsg_dst,
        rpmsg_receive_buf,
        &rpmsg_receive_len
    );
}

/// Handles receieving a message from ARM over RPMsg
void handle_query_from_arm()
{
    if (strcmp((char*)rpmsg_receive_buf, "switch1"))
    {
        memset(rpmsg_send_buf, 0, RPMSG_MSG_SZ);
        ui32_to_string(switch1_last_ms, (char*)rpmsg_send_buf);
        send_to_arm((char*)rpmsg_send_buf);
    }
    else if (strcmp((char*)rpmsg_receive_buf, "switch2"))
    {
        // TODO: Handle querying for switch_2 state
    }
    else
    {
        char message[] = "Valid queries:\nswitch1 - returns the interval between last state changes in miliseconds, -1 if never happened.\n";
        send_to_arm(message);
    }
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

/// Converts uint32_t to a string.
///
/// Replacement for `sprintf` which makes the executable
/// bigger than allowed size and `itoa` which is unimplemented.
void ui32_to_string(uint32_t n, char *buffer)
{
    if(n == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    int i = 0;
    while(n != 0)
    {
        buffer[i] = 48 + (n % 10);
        i++;
        n /= 10;
    }
    buffer[i] = '\0';
    int t;
    for(t = 0; t < i / 2; t++)
    {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }
}
