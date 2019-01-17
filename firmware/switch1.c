#include <stdlib.h>
#include <string.h>
#include "switch1.h"

#define CYCLES_PER_MS 200000

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void set_p8_11(bool value);
bool get_p8_15(void);
void switch1_update(struct switch1 *self);
void switch1_handle_overflow(struct switch1 *self, uint32_t curr_cyc);

bool is_switch1_id(char *str)
{
    return strncmp(str, _SWITCH1_ID, strlen(_SWITCH1_ID)) == 0;
}

void switch1_init(
    struct switch1* self,
    struct cycle_counter* counter,
    struct debouncer* debouncer
){
    set_p8_11(1);
    self->counter = counter;
    self->debouncer = debouncer;
    self->state = get_p8_15();
    self->last_change_ms = -1;
    self->curr_change_ms = 0;
    self->curr_change_cyc = 0;
    self->update = &switch1_update;
}

void switch1_deinit(struct switch1* self)
{
    free(self);
}

/// Set P8_11 GPO to value - R30 bit 15
void set_p8_11(bool value)
{
    __R30 = (__R30 & (~(1 << 15))) | (value << 15);
}

/// Return P8_15 GPI value - R31 bit 15
bool get_p8_15(void)
{
    return (__R31 >> 15) & 1;
}

void switch1_update(struct switch1 *self)
{
    uint32_t curr_cyc = self->counter->cycle(self->counter);
    // If state change just started
    bool curr_state = get_p8_15();
    if (self->state != curr_state && self->debouncer->is_stable)
    {
        switch1_handle_overflow(self, curr_cyc);
        uint32_t diff_cyc = curr_cyc - self->curr_change_cyc;
        uint32_t diff_ms = diff_cyc / CYCLES_PER_MS;
        self->curr_change_ms += diff_ms;
        self->curr_change_cyc = curr_cyc;
        self->debouncer->debounce(self->debouncer, curr_cyc);
    }
    // If during state change
    else if (!self->debouncer->is_stable)
    {
        self->debouncer->debounce(self->debouncer, curr_cyc);
        if (self->debouncer->is_stable && self->state != curr_state)
        {
            self->state = curr_state;
            self->last_change_ms = self->curr_change_ms;
            self->curr_change_ms = 0;
        }
    }
    // If no state change
    else if (self->debouncer->is_stable)
    {
        switch1_handle_overflow(self, curr_cyc);
    }
}

/// Handles a possible cycle overflow
void switch1_handle_overflow(struct switch1 *self, uint32_t curr_cyc)
{
    // If cycles overflown, add the difference to current change counter
    if (curr_cyc < self->curr_change_cyc)
    {
        uint32_t diff_cyc =
            (self->counter->reset_thresh - self->curr_change_cyc) + curr_cyc;
        uint32_t diff_ms = diff_cyc / CYCLES_PER_MS;
        self->curr_change_ms += diff_ms;
        self->curr_change_cyc = curr_cyc;
    }
}
