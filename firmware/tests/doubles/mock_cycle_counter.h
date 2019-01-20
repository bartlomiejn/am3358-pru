#ifndef _MOCK_CYCLE_COUNTER_H_
#define _MOCK_CYCLE_COUNTER_H_

#include <stdint.h>
#include "hardware/cycle_counter/cycle_counter.h"

extern uint32_t mock_cycle_count;

void mock_cycle_counter_init(
    struct cycle_counter* self,
    uint32_t cycles_per_ms,
    uint32_t reset_thresh
);

#endif
