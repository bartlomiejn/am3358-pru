#ifndef _CYCLE_COUNTER_H_
#define _CYCLE_COUNTER_H_

#include <stdint.h>
#include <stdbool.h>

struct cycle_counter
{
    uint32_t reset_thresh;
    void (*reset)(struct cycle_counter* self);
    void (*start)(struct cycle_counter* self);
    void (*update)(struct cycle_counter* self);
    uint32_t (*cycle)(struct cycle_counter* self);
};

void cycle_counter_init(struct cycle_counter* self);
void cycle_counter_deinit(struct cycle_counter* self);

#endif
