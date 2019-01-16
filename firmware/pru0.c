#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pru_ctrl.h>
#include "rpmsg.h"
#include "utils.h"

#define CYC_RESET_THRESHOLD 4000000000
#define MS_PER_THRESHOLD 20000
#define CYCLES_PER_MS 200000
#define DEBOUNCE_MS 500

bool are_cycles_past_threshold(void);
void reduce_cycles_and_update_switch1(void);
void handle_switch1_p8_15_change(void);
void handle_query_from_arm(void);
void send_state(char *message);
bool is_switch1_id(char *str);
bool is_switch2_id(char *str);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
const char* switch1_id = "switch1";
const char* switch2_id = "switch2";
bool switch1_last_p8_15;
uint32_t switch1_start_cyc = 0;
int32_t switch1_curr_ms = 0;
int32_t switch1_last_ms = -1;
bool switch2_last_p8_21;

int main(void)
{
    // Setup RPMsg
    rpmsg_setup();

    // Setup GPOs: P8_11 to 1 (bit 15 of R30), set P8_20 to 1 (bit 13 of R30)
    __R30 = (__R30 & (~(1 << 15))) | (1 << 15);
    __R30 = (__R30 & (~(1 << 13))) | (1 << 13);

    // Get initial GPI
    switch1_last_p8_15 = (__R31 >> 15) & 1;
    switch2_last_p8_21 = (__R31 >> 12) & 1;

    // Setup and reset cycle counter
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;

    while (true)
    {
        if (are_cycles_past_threshold())
        {
            reduce_cycles_and_update_switch1();
        }
        if ((__R31 >> 15) & 1 != switch1_last_p8_15)
        {
            handle_switch1_p8_15_change();
        }
        rpmsg_try_receive(&handle_query_from_arm);
    };
}

/// Are cycles approaching the 4 bil threshold value, which is fairly close to
/// uint32_t bounds
bool are_cycles_past_threshold(void)
{
    return PRU0_CTRL.CYCLE > CYC_RESET_THRESHOLD;
}

/// Reduce cycle count and update switch1 counters
void reduce_cycles_and_update_switch1(void)
{
    PRU0_CTRL.CTRL_bit.CTR_EN = 0;
    PRU0_CTRL.CYCLE -= CYC_RESET_THRESHOLD;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;

    switch1_start_cyc = PRU0_CTRL.CYCLE;
    switch1_curr_ms += MS_PER_THRESHOLD;
}

// Temporary
uint32_t switch1_curr_cyc_plus = 0;
uint32_t switch1_curr_cyc_in_ms = 0;

void handle_switch1_p8_15_change(void)
{
    send_state("pre-switch1-change: ");

    uint32_t curr_cycle = PRU0_CTRL.CYCLE;
    switch1_last_p8_15 = !switch1_last_p8_15;

    switch1_curr_cyc_plus = curr_cycle - switch1_start_cyc;
    switch1_curr_cyc_in_ms = switch1_curr_cyc_plus / CYCLES_PER_MS;
    switch1_last_ms = switch1_curr_ms + switch1_curr_cyc_in_ms;

    switch1_start_cyc = curr_cycle;
    switch1_curr_ms = 0;

    send_state("post-switch1-change: ");
}

void handle_query_from_arm(void)
{
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        send_state("switch1: ");
    }
    else if (is_switch2_id((char*)rpmsg_receive_buf))
    {
        rpmsg_send("2");
    }
    else
    {
        rpmsg_send("Unrecognized, try: switch1, switch2");
    }
}

void send_state(char *message)
{
    char buff[256];
    strcpy(buff, message);

    char last_ms[16];
    i32_to_str(switch1_last_ms, last_ms);
    strcat(last_ms, " last");
    strcat(buff, last_ms);

    char curr_ms[16];
    i32_to_str(switch1_curr_ms, curr_ms);
    strcat(curr_ms, " curr, ");
    strcat(buff, curr_ms);

    char curr_cyc_plus[32];
    ui32_to_str(switch1_curr_cyc_plus, curr_cyc_plus);
    strcat(curr_cyc_plus, " plus curr cyc, ");
    strcat(buff, curr_cyc_plus);

    char curr_cyc_in_ms[16];
    ui32_to_str(switch1_curr_cyc_in_ms, curr_cyc_in_ms);
    strcat(curr_cyc_in_ms, " plus curr as ms, ");
    strcat(buff, curr_cyc_in_ms);

    char start_cyc[32];
    ui32_to_str(switch1_start_cyc, start_cyc);
    strcat(start_cyc, " start cyc, ");
    strcat(buff, start_cyc);

    char last_p8_15[16];
    i32_to_str((int)switch1_last_p8_15, last_p8_15);
    strcat(last_p8_15, " last p8 15");
    strcat(buff, last_p8_15);

    rpmsg_send(buff);
}

bool is_switch1_id(char *str)
{
    return strncmp(str, switch1_id, strlen(switch1_id)) == 0;
}

bool is_switch2_id(char *str)
{
    return strncmp(str, switch2_id, strlen(switch2_id)) == 0;
}
