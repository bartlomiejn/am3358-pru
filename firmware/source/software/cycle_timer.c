#include "software/cycle_timer.h"

static bool cycle_timer_tick(struct cycle_timer* self);

void cycle_timer_init(
    struct cycle_timer *self,
    struct cycle_counter *counter,
    uint32_t interval_ms,
    uint32_t cycles_per_ms
){
    self->counter = counter;
    self->interval_ms = interval_ms;
    self->cycles_per_ms = cycles_per_ms;
    self->cycle = 0;
    self->tick = cycle_timer_tick;
}

/// Returns true once after approximately `interval_ms` passes.
static bool cycle_timer_tick(struct cycle_timer* self)
{
    uint32_t curr_cycle = self->counter->cycle(self->counter);
    uint32_t interval = self->interval_ms * self->cycles_per_ms;
    if (
        curr_cycle > self->cycle
        && (self->cycle + interval) < curr_cycle
    ){
        self->cycle = self->cycle + interval;
        return true;
    }
    else if (
        curr_cycle <= self->cycle
        && self->cycle - self->counter->reset_thresh + interval < curr_cycle
    ){
        self->cycle = (self->cycle + interval) % self->counter->reset_thresh;
        return true;
    }
    return false;
}
