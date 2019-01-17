#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_

#include <stdint.h>
#include <stdbool.h>

struct debouncer
{
    bool is_stable;
    uint32_t interval_ms;
    int32_t _start_cycle;
    bool (*debounce)(struct debouncer* self, uint32_t cycle);
};

void debouncer_init(struct debouncer* self, uint32_t interval_ms);
void debouncer_deinit(struct debouncer* self);

#endif
