#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pru_ctrl.h>
#include "rpmsg.h"
#include "switch1.h"
#include "switch2.h"
#include "utils.h"

#define CYC_RESET_THRESHOLD 4000000000
#define MS_PER_THRESHOLD 20000
#define CYCLES_PER_MS 200000
#define DEBOUNCE_MS 500

bool are_cycles_past_threshold(void);
void reduce_cycles_and_update_switch1(void);
void on_pre_switch1_change(void);
void on_post_switch1_change(void);
void handle_rpmsg(void);
void send_state(char *message);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
struct switch1* switch1;
struct switch2* switch2;

int main(void)
{
    rpmsg_setup();
    switch1_init(switch1);
    switch2_init(switch2);

    // Setup and reset cycle counter
    PRU0_CTRL.CYCLE = 0;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;

    while (true)
    {
        if (are_cycles_past_threshold())
        {
            reduce_cycles_and_update_switch1();
        }
        switch1->update(
            switch1,
            PRU0_CTRL.CYCLE,
            &on_pre_switch1_change,
            &on_post_switch1_change
        );
        rpmsg_try_receive(&handle_rpmsg);
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
    uint32_t cur_cycles = PRU0_CTRL.CYCLE;
    PRU0_CTRL.CYCLE -= CYC_RESET_THRESHOLD;
    switch1->_switch1_start_cyc = cur_cycles;
    switch1->_switch1_curr_ms += MS_PER_THRESHOLD;
}

void on_pre_switch1_change(void)
{
    // send_state("pre-switch1-change: ");
}

void on_post_switch1_change(void)
{
    // send_state("post-switch1-change: ");
}

void handle_rpmsg(void)
{
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        send_state("");
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
    char buff[128];
    strcpy(buff, message);

    char last_ms[16];
    i32_to_str(switch1->_switch1_last_ms, last_ms);
    strcat(last_ms, " last ");
    strcat(buff, last_ms);

    char curr_ms[16];
    i32_to_str(switch1->_switch1_curr_ms, curr_ms);
    strcat(curr_ms, " curr ");
    strcat(buff, curr_ms);

    char curr_cyc_plus[32];
    ui32_to_str(switch1->_switch1_curr_cyc_plus, curr_cyc_plus);
    strcat(curr_cyc_plus, " + cyc ");
    strcat(buff, curr_cyc_plus);

    char curr_cyc_in_ms[16];
    ui32_to_str(switch1->_switch1_curr_cyc_in_ms, curr_cyc_in_ms);
    strcat(curr_cyc_in_ms, " + ms ");
    strcat(buff, curr_cyc_in_ms);

    char start_cyc[32];
    ui32_to_str(switch1->_switch1_start_cyc, start_cyc);
    strcat(start_cyc, " start ");
    strcat(buff, start_cyc);

    // char last_p8_15[16];
    // i32_to_str((int)switch1->_state, last_p8_15);
    // strcat(last_p8_15, " last");
    // strcat(buff, last_p8_15);

    rpmsg_send(buff);
}
