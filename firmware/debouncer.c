#include <stdlib.h>
#include "debouncer.h"

#define CYCLES_PER_MS 200000

bool debounce(struct debouncer* self, uint32_t cycle);

void debouncer_init(struct debouncer* self, uint32_t interval_ms)
{
    self = malloc(sizeof(struct debouncer));
    self->is_stable = true;
    self->interval_ms = interval_ms;
    self->_start_cycle = -1;
    self->debounce = debounce;
}

void debouncer_deinit(struct debouncer* self)
{
    free(self);
}

/// Returns true if signal stable, false if still debouncing
bool debounce(struct debouncer* self, uint32_t cycle)
{
    if (self->_start_cycle == -1)
    {
        self->_start_cycle = cycle;
        self->is_stable = false;
    }
    bool is_overflown = self->_start_cycle > cycle;
    uint32_t interval_cyc = self->interval_ms * CYCLES_PER_MS;
    if ((!is_overflown && (cycle - self->_start_cycle) > interval_cyc)
    || (is_overflown && UINT32_MAX - self->_start_cycle + cycle > interval_cyc))
    {
        self->_start_cycle = -1;
        self->is_stable = true;
        return true;
    }
    return false;
}
