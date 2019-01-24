#include <stdlib.h>
#include <string.h>
#include <hardware/gpi/debounced_gpi.h>
#include "switch1.h"

void switch1_update(struct switch1 *self);

void switch1_init(
    struct switch1* self,
    struct debounced_gpi* gpi,
    struct gpo* gpo
){
    gpo->set(gpo, 1);
    self->gpi = gpi;
    self->state = gpi->state;
    self->last_time = -1;
    self->time = -1;
    self->curr_time = 0;
    self->update = &switch1_update;
}

void switch1_update(struct switch1 *self)
{
    bool last_state = self->gpi->state;
    bool last_is_stable = self->gpi->is_stable;
    bool state = self->gpi->debounce(self->gpi);
    bool is_stable = self->gpi->is_stable;
    if (!is_stable && is_stable != last_is_stable)
    {
        self->time = self->curr_time;
        self->curr_time = 0;
    }
    else if (is_stable && is_stable != last_is_stable)
    {
        if (last_state != state)
            self->last_time = self->time;
        else
            self->curr_time += self->time;
    }
    self->curr_time++;
    self->state = state;
}