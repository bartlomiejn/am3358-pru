#include <stdlib.h>
#include <stdbool.h>
#include "mock_cycle_counter.h"

static void mock_counter_reset(struct cycle_counter *self);
static void mock_counter_start(struct cycle_counter *self);
static void mock_counter_update(struct cycle_counter *self);
static uint32_t mock_counter_cycle(struct cycle_counter *self);

uint32_t mock_cycle_count = 0;
static bool is_counting = false;

void mock_cycle_counter_init(
    struct cycle_counter* self,
    uint32_t cycles_per_ms,
    uint32_t reset_thresh
){
    self->reset_thresh = reset_thresh;
    self->cycles_per_ms = cycles_per_ms;
    self->reset = mock_counter_reset;
    self->start = mock_counter_start;
    self->cycle = mock_counter_cycle;
}

static void mock_counter_reset(struct cycle_counter *self)
{
    mock_cycle_count = 0;
}

static void mock_counter_start(struct cycle_counter *self)
{
    is_counting = true;
}

static uint32_t mock_counter_cycle(struct cycle_counter *self)
{
    return mock_cycle_count;
}
