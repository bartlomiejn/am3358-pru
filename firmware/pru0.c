#include <stdint.h>
#include <stdbool.h>
#include <pru_ctrl.h>
#include <pru_rpmsg.h>
#include "binary.h"

static bool get_P8_15(void);
static bool get_P8_21(void);
static void set_P8_11(bool value);
static void set_P8_20(bool value);
static void setup_ocp(void);
static void setup_rpmsg(void);
static void setup_cycle_counter(void);
static void run_main_loop(void);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
static struct pru_rpmsg_transport transport;

int main(void)
{
    set_P8_11(true);
    set_P8_20(true);
    bool last_p8_15 = get_P8_15();
    bool last_p8_21 = get_P8_21();
    setup_ocp();
    setup_rpmsg();
    setup_cycle_counter();
    run_main_loop();
    __halt();
    return 0;
}

static bool get_P8_15(void)
{
    return ui32_get_nth_bit(__R31, 15);
}

static bool get_P8_21(void)
{
    return ui32_get_nth_bit(__R31, 12);
}

static void set_P8_11(bool value)
{
    r32_set_nth_bit(&__R30, 15, (int)value);
}

static void set_P8_20(bool value)
{
    r32_set_nth_bit(&__R30, 13, (int)value);
}

// Setup OCP master port access for accessing external memories
static void setup_ocp(void)
{
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
}

// Clear the PRUSS system event that ARM will use to notify us
static void setup_rpmsg(void)
{
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
}

static void setup_cycle_counter(void)
{
    PRU0_CTRL.CTRL_bit.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.EN = 1;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
}

static void run_main_loop(void)
{
    while (true) {}
}
