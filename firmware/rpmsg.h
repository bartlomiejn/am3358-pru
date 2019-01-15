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

volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport rpmsg_transport;
uint8_t rpmsg_receive_buf[RPMSG_MSG_SZ], rpmsg_send_buf[RPMSG_MSG_SZ];
uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;

void rpmsg_setup();
uint8_t rpmsg_get_from_arm(void);
void rpmsg_send_to_arm(char *message);
