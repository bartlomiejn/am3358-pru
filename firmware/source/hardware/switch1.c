#include "switch1.h"

void switch1_update(struct switch1 *self);
void switch1_handle_overflow(struct switch1 *self, uint32_t curr_cyc);

void switch1_init(
    struct switch1* self,
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
    self->last_change_ms = -1;
    self->change_ms = 0;
    self->change_cyc = 0;
    self->update = &switch1_update;
}

// Set P8_11 GPO to value - R30 bit 15
//void set_p8_11(bool value)
//{
//    __R30 = (__R30 & (~(1 << 15))) | (value << 15);
//}
//
// Return P8_15 GPI value - R31 bit 15
//bool get_p8_15(void)
//{
//    return (__R31 >> 15) & 1;
//}

void switch1_update(struct switch1 *self)
{
    uint32_t curr_cyc = self->counter->cycle(self->counter);
    bool curr_state = self->gpi->get(self->gpi);
    // If state change just started
    if (self->state != curr_state && self->debouncer->is_stable)
    {
        switch1_handle_overflow(self, curr_cyc);
        uint32_t diff_cyc = curr_cyc - self->change_cyc;
        uint32_t diff_ms = diff_cyc / self->counter->cycles_per_ms;
        self->change_ms += diff_ms;
        self->change_cyc = curr_cyc;
        self->debouncer->debounce(self->debouncer, curr_cyc);
    }
    // If during state change
    else if (!self->debouncer->is_stable)
    {
        self->debouncer->debounce(self->debouncer, curr_cyc);
        if (self->debouncer->is_stable && self->state != curr_state)
        {
            self->state = curr_state;
            self->last_change_ms = self->change_ms;
            self->change_ms = 0;
        }
    }
    // If no state change
    else if (self->debouncer->is_stable)
    {
        switch1_handle_overflow(self, curr_cyc);
    }
}

/// Handles a cycle overflow
void switch1_handle_overflow(struct switch1 *self, uint32_t curr_cyc)
{
    // If cycles overflown, add the difference to current change counter
    if (curr_cyc < self->change_cyc)
    {
        uint32_t diff_cyc =
            (self->counter->reset_thresh - self->change_cyc) + curr_cyc;
        uint32_t diff_ms = diff_cyc / self->counter->cycles_per_ms;
        self->change_ms += diff_ms;
        self->change_cyc = curr_cyc;
    }
}
