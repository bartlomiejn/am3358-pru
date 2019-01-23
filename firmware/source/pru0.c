#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pru_cfg.h>
#include "software/rpmsg.h"
#include "software/debouncer.h"
#include "hardware/gpi/gpi_p8_15.h"
#include "hardware/gpi/gpi_p8_16.h"
#include "hardware/gpo/gpo_p8_11.h"
#include "hardware/gpo/gpo_p8_12.h"
#include "hardware/cycle_counter/pru_cycle_counter.h"
#include "hardware/switch1.h"
#include "hardware/switch2.h"
#include "utils.h"

#define DEBOUNCE_MS         200
#define CYC_PER_MS          200000
#define CYC_RESET_THRESH    4000000000
#define SWITCH_STATE_SZ     32
#define SHAREDMEM_PTR       (volatile uint8_t *)0x00010000
#define SWITCH1_BIT_PTR     SHAREDMEM_PTR
#define SWITCH1_STATE_PTR   SWITCH1_BIT_PTR + 1
#define SWITCH2_BIT_PTR     SWITCH1_STATE_PTR + SWITCH_STATE_SZ
#define SWITCH2_STATE_PTR   SWITCH2_BIT_PTR + 1

bool should_write_switch1(void);
bool should_write_switch2(void);
void write_switch1_state(void);
void write_switch2_state(void);

struct gpo p8_11;
struct gpi p8_15;
struct gpo p8_12;
struct gpi p8_16;
struct cycle_counter counter;
struct debouncer switch1_debouncer;
struct debouncer switch2_debouncer;
struct switch1 switch1;
struct switch2 switch2;
volatile uint8_t* switch1_bit_memptr   = SWITCH1_BIT_PTR;
volatile uint8_t* switch1_state_memptr = SWITCH1_STATE_PTR;
volatile uint8_t* switch2_bit_memptr   = SWITCH2_BIT_PTR;
volatile uint8_t* switch2_state_memptr = SWITCH2_STATE_PTR;
char switch1_buf[SWITCH_STATE_SZ];
char switch2_buf[SWITCH_STATE_SZ];

int main(void)
{
    // Setup OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    gpo_p8_11_init(&p8_11);
    gpi_p8_15_init(&p8_15);
    gpo_p8_12_init(&p8_12);
    gpi_p8_16_init(&p8_16);
    pru_cycle_counter_init(&counter, CYC_PER_MS, CYC_RESET_THRESH);
    debouncer_init(
        &switch1_debouncer,
        DEBOUNCE_MS,
        counter.cycles_per_ms,
        counter.reset_thresh
    );
    debouncer_init(
        &switch2_debouncer,
        DEBOUNCE_MS,
        counter.cycles_per_ms,
        counter.reset_thresh
    );
    switch1_init(&switch1, &counter, &switch1_debouncer, &p8_15, &p8_11);
    switch2_init(&switch2, &counter, &switch2_debouncer, &p8_16, &p8_12);
    counter.reset(&counter);
    counter.start(&counter);
    while (true)
    {
        counter.update(&counter);
        switch1.update(&switch1);
        switch2.update(&switch2);
        if (should_write_switch1()) write_switch1_state();
        if (should_write_switch2()) write_switch2_state();
    };
}

bool should_write_switch1(void)
{
    return switch1_bit_memptr[0] == 0;
}

bool should_write_switch2(void)
{
    return switch2_bit_memptr[0] == 0;
}

void write_switch1_state(void)
{
    int i = 0;
    i32_to_str(switch1.last_change_ms, switch1_buf);
    while (switch1_buf[i] != (char)0)
    {
        switch1_state_memptr[i] = (uint8_t)switch1_buf[i];
        i++;
    }
    switch1_state_memptr[i] = 0;
    switch1_bit_memptr[0] = 1;
}

void write_switch2_state(void)
{
    int i = 0;
    i32_to_str(switch2.last_on_ms, switch2_buf);
    while (switch2_buf[i] != (char)0)
    {
        switch2_state_memptr[i] = (uint8_t)switch2_buf[i];
        i++;
    }
    switch2_state_memptr[i] = 0;
    switch2_bit_memptr[0] = 1;
}
