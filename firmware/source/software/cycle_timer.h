#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/cycle_counter/cycle_counter.h"

struct cycle_timer {
    struct cycle_counter *counter;
    uint32_t interval_ms;
    uint32_t cycles_per_ms;
    uint32_t cycle;
    bool (*tick)(struct cycle_timer* self);
};

void cycle_timer_init(
    struct cycle_timer *self,
    struct cycle_counter *counter,
    uint32_t interval_ms,
    uint32_t cycles_per_ms
);

#endif
