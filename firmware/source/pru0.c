#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
#define SHARED_MEM_ADDR     0x00010000

bool should_write();
void write_state_shared_mem();

struct gpo p8_11;
struct gpi p8_15;
struct gpo p8_12;
struct gpi p8_16;
struct cycle_counter counter;
struct debouncer switch1_debouncer;
struct debouncer switch2_debouncer;
struct switch1 switch1;
struct switch2 switch2;
volatile uint8_t* shared_mem = (volatile uint8_t *)SHARED_MEM_ADDR;
static char message[128];

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
        if (should_write()) write_state_shared_mem();
    };
}

bool should_write()
{
    return shared_mem[0] == 0
}

void write_state_shared_mem()
{
    char last_change[16];
    char last_on[16];
    int i;
    i32_to_str(switch1.last_change_ms, last_change);
    i32_to_str(switch2.last_on_ms, last_on);
    strcpy(message, last_change);
    strcat(message, ";");
    strcat(message, last_on);
    for (i = 0; i < strlen(message); i++)
    {
        shared_mem[i] = message[i];
    }
    shared_mem[i + 1] = 0;
}
