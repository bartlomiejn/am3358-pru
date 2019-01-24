#ifndef _CYCLE_COUNTER_H_
#define _CYCLE_COUNTER_H_

#include <stdint.h>
#include <stdbool.h>

struct cycle_counter
{
    uint32_t reset_thresh;
    uint32_t cycles_per_ms;
    void (*reset)(struct cycle_counter* self);
    void (*start)(struct cycle_counter* self);
    uint32_t (*cycle)(struct cycle_counter* self);
};

#endif
