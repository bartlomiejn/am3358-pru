#ifndef _MOCK_CYCLE_TIMER_H
#define _MOCK_CYCLE_TIMER_H

#include <stdbool.h>
#include <stdint.h>
#include "software/cycle_timer.h"

void mock_cycle_timer_init(
    struct cycle_timer *self,
    struct cycle_counter *counter,
    uint32_t interval_ms,
    uint32_t cycles_per_ms
);

extern bool mock_tick;

#endif
