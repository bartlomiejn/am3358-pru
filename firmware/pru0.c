#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include <rsc_types.h>
#include "binary.h"
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

#define S_TO_NS_RATIO 1_000_000_000
#define CYCLE_TIME_NS 5

static void set_P8_11(bool value);
static void set_P8_20(bool value);
static bool get_P8_15(void);
static bool get_P8_21(void);
static void setup_ocp(void);
static void setup_rpmsg(void);
static void setup_cycle_counter(void);
static void reset_host_int(void);
static void run_main_loop(void);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
volatile uint8_t *status = &resource_table.rpmsg_vdev.status;
static struct pru_rpmsg_transport transport;
uint8_t rpmsg_receive_buf[396];
uint16_t rpmsg_src, rpmsg_dst, rpmsg_receive_len;
bool last_p8_15, last_p8_21;

int main(void)
{
    set_P8_11(true);
    set_P8_20(true);
    last_p8_15 = get_P8_15();
    last_p8_21 = get_P8_21();
    setup_ocp();
    setup_rpmsg();
    setup_cycle_counter();
    run_main_loop();
    __halt();
    return 0;
}

static void set_P8_11(bool value)
{
    r32_set_nth_bit(&__R30, 15, (int)value);
}

static void set_P8_20(bool value)
{
    r32_set_nth_bit(&__R30, 13, (int)value);
}

static bool get_P8_15(void)
{
    return ui32_get_nth_bit(__R31, 15);
}

static bool get_P8_21(void)
{
    return ui32_get_nth_bit(__R31, 12);
}

/// Setup OCP master port access for accessing external memories
static void setup_ocp(void)
{
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
}

/// Clear the PRUSS system event that ARM will use to notify us
static void setup_rpmsg(void)
{
    reset_host_int();
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

/// Reset cycles and start the cycle counter
static void setup_cycle_counter(void)
{
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.EN = 1;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
}

/// Clear status of PRUSS system event from ARM
static void reset_host_int(void)
{
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
}


// static void set_P8_11(bool value);
// static void set_P8_20(bool value);
// static bool get_P8_15(void);
// static bool get_P8_21(void);
//
// static void update_switch1()
// {
//     if (last_p8_15 != get_P8_15())
//     {
//
//     }
// }
//
// static void update_switch2()
// {
//
// }

static int16_t receive_from_arm()
{
    return pru_rpmsg_receive(
        &transport,
        &rpmsg_src,
        &rpmsg_dst,
        rpmsg_receive_buf,
        &rpmsg_receive_len
    );
}

static void send_to_arm(char* message)
{
    return pru_rpmsg_send(
        &transport,
        rpmsg_dst,
        rpmsg_src,
        message,
        strlen(message)
    );
}

static void run_main_loop(void)
{
    while (true)
    {
        if (__R31 & HOST_INT)
        {
            reset_host_int();
            while (receive_from_arm() == PRU_RPMSG_SUCCESS) {
                char *message;
                double time = PRU0_CTRL.CYCLE * CYCLE_TIME_NS / S_TO_NS_RATIO;
                sprintf(message, "Time since cycle reset: %f\n", time);
                send_to_arm(message);
            }
        }
    }
}
