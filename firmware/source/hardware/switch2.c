#include <stdlib.h>
#include <string.h>
#include "switch2.h"

void switch2_update(struct switch2 *self);
void switch2_handle_overflow(struct switch2 *self, uint32_t curr_cyc);

void switch2_init(
    struct switch2* self,
    struct cycle_counter* counter,
    struct debouncer* debouncer,
    struct gpi* gpi,
    struct gpo* gpo
){
    gpo->set(gpo, 1);
    self->counter = counter;
    self->debouncer = debouncer;
    self->gpi = gpi;
    self->gpo = gpo;
    self->state = gpi->get(gpi);
    self->last_on_ms = -1;
    self->on_ms = 0;
    self->on_cyc = 0;
    self->update = &switch2_update;
}

void switch2_update(struct switch2 *self)
{
    uint32_t curr_cyc = self->counter->cycle(self->counter);
    bool curr_state = self->gpi->get(self->gpi);
    // If state change just started
    if (self->state != curr_state && self->debouncer->is_stable)
    {
        if (curr_state == 1)
        {
            self->on_ms = 0;
            self->on_cyc = curr_cyc;
        }
        else
        {
            switch2_handle_overflow(self, curr_cyc);
            uint32_t diff_cyc = curr_cyc - self->on_cyc;
            uint32_t diff_ms = diff_cyc / self->counter->cycles_per_ms;
            self->on_ms += diff_ms;
            self->on_cyc = curr_cyc;
        }
        self->debouncer->debounce(self->debouncer, curr_cyc);
    }
    // If during state change
    else if (!self->debouncer->is_stable)
    {
        self->debouncer->debounce(self->debouncer, curr_cyc);
        if (self->debouncer->is_stable)
        {
            self->state = curr_state;
            if (self->state == 0)
            {
                self->last_on_ms = self->on_ms;
                self->on_ms = 0;
                self->on_cyc = 0;
            }
        }
    }
    // If no state change and on, handle possible overflow
    else if (self->debouncer->is_stable && self->state == 1)
    {
        switch2_handle_overflow(self, curr_cyc);
    }
}

/// Handles a cycle overflow
void switch2_handle_overflow(struct switch2 *self, uint32_t curr_cyc)
{
    // If cycles overflown, update on_ms
    if (curr_cyc < self->on_cyc)
    {
        uint32_t diff_cyc =
            (self->counter->reset_thresh - self->on_cyc) + curr_cyc;
        uint32_t diff_ms = diff_cyc / self->counter->cycles_per_ms;
        self->on_ms += diff_ms;
        self->on_cyc = curr_cyc;
    }
}
