#ifndef _PRU_CYCLE_COUNTER_H
#define _PRU_CYCLE_COUNTER_H

#include <stdint.h>
#include "cycle_counter.h"

void pru_cycle_counter_init(
    struct cycle_counter* self,
    uint32_t cycles_per_ms,
    uint32_t reset_thresh
);

#endif
