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

char *ui32_to_string(uint32_t n);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport rpmsg_transport;
bool last_p8_15, last_p8_21;
uint8_t rpmsg_receive_buf[RPMSG_RECEIVE_SZ], rpmsg_send_buf[RPMSG_SEND_SZ];
uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;

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
            // Reset host0 interrupt
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
            while (pru_rpmsg_receive(
                &rpmsg_transport,
                &rpmsg_src,
                &rpmsg_dst,
                rpmsg_receive_buf,
                &rpmsg_receive_len
            ) == PRU_RPMSG_SUCCESS)
            {
                memset(rpmsg_send_buf, 0, RPMSG_SEND_SZ);
                ui32_to_string(PRU0_CTRL.CYCLE, (char*)rpmsg_send_buf);
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

void ui32_to_string(uint32_t n, char *buffer)
{
    if(n == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    int i = 0;
    bool is_neg = n < 0;
    unsigned int abs_n = is_neg ? -n : n;
    while(abs_n != 0)
    {
        buffer[i] = 48 + (abs_n % 10);
        i++;
        abs_n = abs_n / 10;
    }
    if(is_neg)
    {
        buffer[i] = '-';
        i++;
    }
    buffer[i] = '\0';
    int t;
    for(t = 0; t < i / 2; t++)
    {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }
    return buffer;
}
