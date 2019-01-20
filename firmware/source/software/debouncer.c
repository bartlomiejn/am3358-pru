#include <stdlib.h>
#include "debouncer.h"

void debouncer_debounce(struct debouncer *self, uint32_t cycle);

void debouncer_init(struct debouncer* self, uint32_t interval_ms, uint32_t cycles_per_ms, uint32_t reset_thresh)
{
    self->is_stable = true;
    self->interval_ms = interval_ms;
    self->cycles_per_ms = cycles_per_ms;
    self->reset_thresh = reset_thresh;
    self->_start_cycle = 0;
    self->debounce = debouncer_debounce;
}

void debouncer_debounce(struct debouncer *self, uint32_t cycle)
{
    if (self->is_stable)
    {
        self->_start_cycle = cycle;
        self->is_stable = false;
    }
    bool is_overflown = self->_start_cycle > cycle;
    uint32_t interval_cyc = self->interval_ms * self->cycles_per_ms;
    if ((!is_overflown && (cycle - self->_start_cycle) > interval_cyc)
    || (is_overflown && self->reset_thresh - self->_start_cycle + cycle > interval_cyc))
    {
        self->_start_cycle = 0;
        self->is_stable = true;
    }
}
