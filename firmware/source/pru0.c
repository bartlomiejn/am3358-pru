#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "software/rpmsg.h"
#include "software/debouncer.h"
#include "hardware/gpi/gpi_p8_15.h"
#include "hardware/gpi/gpi_p8_21.h"
#include "hardware/gpo/gpo_p8_11.h"
#include "hardware/gpo/gpo_p8_20.h"
#include "hardware/cycle_counter/pru_cycle_counter.h"
#include "hardware/switch1.h"
#include "hardware/switch2.h"
#include "utils.h"

#define DEBOUNCE_MS         200
#define CYC_PER_MS          200000
#define CYC_RESET_THRESH    4000000000

void on_rpmsg_receive(void);

struct gpo p8_11;
struct gpi p8_15;
struct gpo p8_20;
struct gpi p8_21;
struct cycle_counter counter;
struct debouncer switch1_debouncer;
struct debouncer switch2_debouncer;
struct switch1 switch1;
struct switch2 switch2;

int main(void)
{
    rpmsg_setup();
    gpo_p8_11_init(&p8_11);
    gpi_p8_15_init(&p8_15);
    gpo_p8_20_init(&p8_20);
    gpi_p8_21_init(&p8_21);
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
    switch2_init(&switch2, &counter, &switch2_debouncer, &p8_21, &p8_20);
    counter.reset(&counter);
    counter.start(&counter);
    while (true)
    {
        counter.update(&counter);
        switch1.update(&switch1);
        switch2.update(&switch2);
        rpmsg_try_receive(&on_rpmsg_receive);
    };
}

char message[128];

void on_rpmsg_receive(void)
{
    char last_change[16];
    char last_on[16];
    i32_to_str(switch1.last_change_ms, last_change);
    i32_to_str(switch2.last_on_ms, last_on);
    strcpy(message, last_change);
    strcat(message, ";");
    strcat(message, last_on);
    rpmsg_send(message);
}
