#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include "rpmsg.h"
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

bool is_host0_int_set(void);
void reset_host0_int(void);

const int RPMSG_RECEIVED = PRU_RPMSG_SUCCESS;
const int RPMSG_NOT_CONNECTED = PRU_RPMSG_NO_KICK;
uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ];

volatile register uint32_t __R31;
static volatile uint8_t *status;
static struct pru_rpmsg_transport transport;
static uint16_t src, dst, len;

void rpmsg_setup(void)
{
    // Setup OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    // Reset host0 interrupt
    reset_host0_int();
    status = &resource_table.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
    pru_rpmsg_init(
        &transport,
        &resource_table.rpmsg_vring0,
        &resource_table.rpmsg_vring1,
        TO_ARM_HOST_SYS_EVENT,
        FROM_ARM_HOST_SYS_EVENT
    );
    while (pru_rpmsg_channel(
        RPMSG_NS_CREATE,
        &transport,
        CHAN_NAME,
        CHAN_DESC,
        CHAN_PORT
    ) != PRU_RPMSG_SUCCESS);
}

void rpmsg_try_receive(void (*handler)(void))
{
    if (is_host0_int_set())
    {
        reset_host0_int();
        while (pru_rpmsg_receive(
            &transport,
            &src,
            &dst,
            rpmsg_receive_buf,
            &len
        ) == PRU_RPMSG_SUCCESS)
        {
            handler();
        }
    }
}

uint8_t rpmsg_send(char *message)
{
    return pru_rpmsg_send(
        &transport,
        dst,
        src,
        message,
        strlen(message)
    );
}

bool is_host0_int_set(void)
{
    return __R31 & HOST_INT;
}

void reset_host0_int(void)
{
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
}
