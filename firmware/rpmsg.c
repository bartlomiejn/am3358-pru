#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include "rpmsg.h"
#include "resource_table_0.h"

const int RPMSG_RECEIVED = PRU_RPMSG_SUCCESS;
const int RPMSG_NOT_CONNECTED = PRU_RPMSG_NO_KICK;
uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ];

volatile register uint32_t __R31;
volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
struct pru_rpmsg_transport transport;
uint16_t src, dst, receive_len;
bool is_connected;

bool _is_host0_int_set(void);
void _reset_host0_int(void);

void rpmsg_setup(void)
{
    // Setup OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    // Reset host0 interrupt
    _reset_host0_int();
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
    if (_is_host0_int_set())
    {
        _reset_host0_int();
        while (pru_rpmsg_receive(
            &transport,
            &src,
            &dst,
            rpmsg_receive_buf,
            &receive_len
        ) == PRU_RPMSG_SUCCESS)
        {
            if (!is_connected)
            {
                is_connected = true;
            }
            handler();
        }
    }
}

uint8_t rpmsg_send(char *message)
{
    return is_connected ? pru_rpmsg_send(
        &transport,
        dst,
        src,
        message,
        strlen(message)
    ) : RPMSG_NOT_CONNECTED;
}

bool _is_host0_int_set(void)
{
    return __R31 & HOST_INT;
}

void _reset_host0_int(void)
{
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
}
