#include <string.h>
#include <pru_rpmsg.h>
#include "rpmsg.h"
#include "resource_table_0.h"

const int RPMSG_RECEIVED = PRU_RPMSG_SUCCESS;
uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ];
static volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport rpmsg_transport;
static uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;

void rpmsg_setup(void)
{
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
}

uint8_t rpmsg_get_from_arm(void)
{
    return pru_rpmsg_receive(
        &rpmsg_transport,
        &rpmsg_src,
        &rpmsg_dst,
        rpmsg_receive_buf,
        &rpmsg_receive_len
    );
}

void rpmsg_send_to_arm(char *message)
{
    pru_rpmsg_send(
        &rpmsg_transport,
        rpmsg_dst,
        rpmsg_src,
        message,
        strlen(message)
    );
}
