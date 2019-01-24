#include "mock_cycle_timer.h"

bool mock_tick = true;

static bool mock_cycle_timer_tick(struct cycle_timer* self);

void mock_cycle_timer_init(
    struct cycle_timer *self,
    struct cycle_counter *counter,
    uint32_t interval_ms,
    uint32_t cycles_per_ms
){
    self->counter = counter;
    self->interval_ms = interval_ms;
    self->cycles_per_ms = cycles_per_ms;
    self->cycle = 0;
    self->tick = mock_cycle_timer_tick;
}

/// Returns true once after approximately `interval_ms` passes.
static bool mock_cycle_timer_tick(struct cycle_timer* self)
{
    return mock_tick;
}
