#include <stdlib.h>
#include "cycle_counter.h"
#include "debouncer.h"

#define CYCLES_PER_MS 200000

void debouncer_debounce(struct debouncer *self, uint32_t cycle);

void debouncer_init(struct debouncer* self, uint32_t interval_ms, uint32_t reset_thresh)
{
    self->is_stable = true;
    self->interval_ms = interval_ms;
    self->reset_thresh = reset_thresh;
    self->_start_cycle = 0;
    self->debounce = debouncer_debounce;
}

void debouncer_deinit(struct debouncer* self)
{
    free(self);
}

void debouncer_debounce(struct debouncer *self, uint32_t cycle)
{
    if (self->is_stable)
    {
        self->_start_cycle = cycle;
        self->is_stable = false;
    }
    bool is_overflown = self->_start_cycle > cycle;
    uint32_t interval_cyc = self->interval_ms * CYCLES_PER_MS;
    if ((!is_overflown && (cycle - self->_start_cycle) > interval_cyc)
    || (is_overflown && self->reset_thresh - self->_start_cycle + cycle > interval_cyc))
    {
        self->_start_cycle = 0;
        self->is_stable = true;
    }
}
