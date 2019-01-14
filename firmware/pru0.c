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

#define RPMSG_RECEIVE_SZ 396
#define RPMSG_SEND_SZ 10
#define CYCLE_THRESHOLD 4000000000
#define MS_PER_THRESHOLD 20000
#define CYCLES_PER_MS 200000

void ui32_to_string(uint32_t n, char *buffer);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport rpmsg_transport;
uint8_t rpmsg_receive_buf[RPMSG_RECEIVE_SZ], rpmsg_send_buf[RPMSG_SEND_SZ];
uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;
bool last_p8_15, last_p8_21;
uint32_t elapsed_time_s, elapsed_time_ms;
uint32_t curr_cycles;

int main(void)
{
    // Set P8_11 to 1 (bit 15 of R30), set P8_20 to 1 (bit 13 of R30)
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

    // Setup and reset cycle counter
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.EN = 1;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;

    // Get initial P8_15 (bit 15 of R31) and P8_21 (bit 12 of R31) GPI state
    last_p8_15 = (__R31 >> 15) & 1;
    last_p8_21 = (__R31 >> 12) & 1;

    // Run main loop
    while (true)
    {
        if (__R31 & HOST_INT)
        {
            // Update elapsed time once a milisecond passes
            if (PRU0_CTRL.CYCLE > CYCLE_THRESHOLD)
            {
                PRU0_CTRL.CYCLE -= CYCLE_THRESHOLD;
                elapsed_time_ms += MS_PER_THRESHOLD;
            }

            // Reset host0 interrupt
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;

            // If there is a message, return elapsed time
            while (pru_rpmsg_receive(
                &rpmsg_transport,
                &rpmsg_src,
                &rpmsg_dst,
                rpmsg_receive_buf,
                &rpmsg_receive_len
            ) == PRU_RPMSG_SUCCESS)
            {
                uint32_t total_elapsed_time_ms = elapsed_time_ms
                    + PRU0_CTRL.CYCLE / CYCLES_PER_MS

                memset(rpmsg_send_buf, 0, RPMSG_SEND_SZ);
                ui32_to_string(total_elapsed_time_ms, (char*)rpmsg_send_buf);
                pru_rpmsg_send(
                    &rpmsg_transport,
                    rpmsg_dst,
                    rpmsg_src,
                    rpmsg_send_buf,
                    strlen((char*)rpmsg_send_buf)
                );
            }
        }
    };
}

/// Converts uint32_t to a string. Including sprintf makes the executable bigger
/// than allowed size and itoa is unimplemented
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
