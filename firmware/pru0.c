#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include <pru_intc.h>
#include "rpmsg.h"
#include "utils.h"

#define CYCLE_THRESHOLD 4000000000
#define MS_PER_THRESHOLD 20000
#define CYCLES_PER_MS 200000

bool are_cycles_past_threshold(void);
void reduce_cycles_and_update_switch1(void);
void handle_switch1_p8_15_change(bool switch1_curr_p8_15);
void handle_query_from_arm(void);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
const char* switch1_id = "switch1";
const char* switch2_id = "switch2";
bool switch1_last_p8_15;
uint32_t switch1_start_cycle = 0;
int32_t switch1_curr_ms = 0;
int32_t switch1_last_ms = -1;
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
    rpmsg_setup();

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
            reduce_cycles_and_update_switch1();

        bool switch1_curr_p8_15 = (__R31 >> 15) & 1;
        if (switch1_curr_p8_15 != switch1_last_p8_15)
            handle_switch1_p8_15_change(switch1_curr_p8_15);

        // If received a host0 interrupt
        if (__R31 & HOST_INT)
        {
            // Reset the host0 interrupt
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST_SYS_EVENT;
            while (rpmsg_get_from_arm() == RPMSG_RECEIVED)
                handle_query_from_arm();
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

bool is_switch1_id(char *str)
{
    return strncmp(str, switch1_id, strlen(switch1_id)) == 0;
}

bool is_switch2_id(char *str)
{
    return strncmp(str, switch2_id, strlen(switch2_id)) == 0;
}

// bool switch1_last_p8_15;
// uint32_t switch1_start_cycle = 0;
// int32_t switch1_curr_ms = 0;
// int32_t switch1_last_ms = -1;

char buff[32];

void handle_query_from_arm(void)
{
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        i32_to_str(switch1_last_ms, buff);
        strcat(buff, " last");
        rpmsg_send_to_arm(buff);

        // char curr_ms[16];
        // i32_to_str(switch1_curr_ms, curr_ms);
        // strcat(curr_ms, " curr, ");
        // rpmsg_send_to_arm(last_ms);
        //
        // char start_cyc[16];
        // ui32_to_str(switch1_start_cycle, start_cyc);
        // strcat(start_cyc, " start cyc, ");
        // rpmsg_send_to_arm(start_cyc);
        //
        // char last_p8_15[16];
        // i32_to_str((int)switch1_last_p8_15, last_p8_15);
        // strcat(last_p8_15, " last p8 15");
        // rpmsg_send_to_arm(last_p8_15);
    }
    else if (is_switch2_id((char*)rpmsg_receive_buf))
    {
        rpmsg_send_to_arm("2");
    }
    else
    {
        rpmsg_send_to_arm("Unrecognized, try: switch1, switch2");
    }
}
