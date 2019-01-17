#ifndef _SWITCH1_H_
#define _SWITCH1_H_

#include <stdint.h>
#include <stdbool.h>
#include "cycle_counter.h"
#include "debouncer.h"

#define _SWITCH1_ID "switch1"

struct switch1
{
    struct cycle_counter* counter;
    struct debouncer* debouncer;
    bool state;
    int32_t last_change_ms;
    int32_t curr_change_ms;
    uint32_t curr_change_cyc;
    void (*update)(struct switch1* switch1);
};

bool is_switch1_id(char *str);
void switch1_init(
    struct switch1* self,
    struct cycle_counter* counter,
    struct debouncer* debouncer
);
void switch1_deinit(struct switch1* self);

#endif
