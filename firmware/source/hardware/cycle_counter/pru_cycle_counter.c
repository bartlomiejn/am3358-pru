#include <stdint.h>
#include <stdlib.h>
#include <pru_ctrl.h>
#include "pru_cycle_counter.h"

static void pru_cycle_counter_reset(struct cycle_counter *self);
static void pru_cycle_counter_start(struct cycle_counter *self);
static uint32_t pru_cycle_counter_cycle(struct cycle_counter *self);

void pru_cycle_counter_init(
    struct cycle_counter* self,
    uint32_t cycles_per_ms,
    uint32_t reset_thresh
){
    self->reset_thresh = reset_thresh;
    self->cycles_per_ms = cycles_per_ms;
    self->reset = pru_cycle_counter_reset;
    self->start = pru_cycle_counter_start;
    self->cycle = pru_cycle_counter_cycle;
}

static void pru_cycle_counter_reset(struct cycle_counter *self)
{
    PRU0_CTRL.CYCLE = 0;
}

static void pru_cycle_counter_start(struct cycle_counter *self)
{
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
}

static uint32_t pru_cycle_counter_cycle(struct cycle_counter *self)
{
    if (PRU0_CTRL.CYCLE > self->reset_thresh)
    {
        PRU0_CTRL.CYCLE -= self->reset_thresh;
    }
    return PRU0_CTRL.CYCLE;
}
