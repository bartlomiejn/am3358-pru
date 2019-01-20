#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_

#include <stdint.h>
#include <stdbool.h>

struct debouncer
{
    bool is_stable;
    uint32_t interval_ms;
    uint32_t cycles_per_ms;
    uint32_t reset_thresh;
    uint32_t _start_cycle;
    void (*debounce)(struct debouncer* self, uint32_t cycle);
};

void debouncer_init(struct debouncer* self, uint32_t interval_ms, uint32_t cycles_per_ms, uint32_t reset_thresh);

#endif
