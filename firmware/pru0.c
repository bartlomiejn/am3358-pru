#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "rpmsg.h"
#include "cycle_counter.h"
#include "debouncer.h"
#include "switch1.h"
#include "switch2.h"
#include "utils.h"

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
    switch1_debouncer = malloc(sizeof(struct debouncer));
    switch1 = malloc(sizeof(struct switch1));
    switch2 = malloc(sizeof(struct switch2));
    cycle_counter_init(counter);
    debouncer_init(switch1_debouncer, DEBOUNCE_MS, counter->reset_thresh);
    switch1_init(switch1, counter, switch1_debouncer);
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

void on_rpmsg_receive(void)
{
    if (is_switch1_id((char*)rpmsg_receive_buf))
    {
        char message[24];
        i32_to_str(switch1->last_change_ms, message);
        rpmsg_send(message);
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
