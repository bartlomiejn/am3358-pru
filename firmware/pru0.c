#include "rpmsg.h"
#include "cycle_counter.h"
#include "debouncer.h"
#include "switch1.h"
#include "switch2.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DEBOUNCE_MS 100

void on_switch1_change(void);
void on_rpmsg_receive(void);

volatile register uint32_t __R30;
volatile register uint32_t __R31;
struct cycle_counter* counter;
struct debouncer* switch1_debouncer;
struct switch1* switch1;
struct switch2* switch2;

int main(void)
{
    rpmsg_setup();
    counter = malloc(sizeof(struct cycle_counter));
    cycle_counter_init(counter);
    switch1_debouncer = malloc(sizeof(struct debouncer));
    debouncer_init(switch1_debouncer, DEBOUNCE_MS, counter->reset_thresh);
    switch1_init(switch1, counter, switch1_debouncer, &on_switch1_change);
    switch2_init(switch2);
    counter->reset(counter);
    counter->start(counter);
    while (true)
    {
        counter->update(counter);
        switch1->update(switch1);
        rpmsg_try_receive(&on_rpmsg_receive);
    };
}


void on_switch1_change(void)
{
    rpmsg_send("on_switch1_change\n");
}

void on_rpmsg_receive(void)
{
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        rpmsg_send("1\n");
    }
    else if (is_switch2_id((char*)rpmsg_receive_buf))
    {
        rpmsg_send("2\n");
    }
    else
    {
        rpmsg_send("Unrecognized, try: switch1, switch2\n");
    }
}
