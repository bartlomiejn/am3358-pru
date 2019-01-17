#include <stdint.h>
#include <stdlib.h>
#include <pru_ctrl.h>
#include "cycle_counter.h"

#define CYC_RESET_THRESHOLD 4000000000

void cycle_counter_reset(struct cycle_counter *self);
void cycle_counter_start(struct cycle_counter *self);
void cycle_counter_update(struct cycle_counter *self);
uint32_t cycle_counter_cycle(struct cycle_counter *self);

void cycle_counter_init(struct cycle_counter* self)
{
    self->reset_thresh = CYC_RESET_THRESHOLD;
    self->reset = cycle_counter_reset;
    self->start = cycle_counter_start;
    self->update = cycle_counter_update;
    self->cycle = cycle_counter_cycle;
}

void cycle_counter_deinit(struct cycle_counter* self)
{
    free(self);
}

void cycle_counter_reset(struct cycle_counter *self)
{
    PRU0_CTRL.CYCLE = 0;
}

void cycle_counter_start(struct cycle_counter *self)
{
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
}

void cycle_counter_update(struct cycle_counter *self)
{
    if (PRU0_CTRL.CYCLE > CYC_RESET_THRESHOLD)
    {
        PRU0_CTRL.CYCLE -= CYC_RESET_THRESHOLD;
    }
}

uint32_t cycle_counter_cycle(struct cycle_counter *self)
{
    return PRU0_CTRL.CYCLE;
}
