#include <stdlib.h>
#include <string.h>
#include "switch2.h"

void switch2_update(struct switch2 *self);
void switch2_handle_overflow(struct switch2 *self, uint32_t curr_cyc);

void switch2_init(
    struct switch2* self,
    struct debounced_gpi* gpi,
    struct gpo* gpo
){
    gpo->set(gpo, 1);
    self->gpi = gpi;
    self->state = gpi->state;
    self->last_on = -1;
    self->on = -1;
    self->curr_on = 0;
    self->update = &switch2_update;
}

void switch2_update(struct switch2 *self)
{
    bool last_state = self->gpi->state;
    bool last_is_stable = self->gpi->is_stable;
    bool state = self->gpi->debounce(self->gpi);
    bool is_stable = self->gpi->is_stable;
    if (!is_stable && is_stable != last_is_stable)
    {
         if (last_state == 1)
             self->on = self->curr_on;
        self->curr_on = 0;
    }
    else if (is_stable && is_stable != last_is_stable && last_state == 1)
    {
        if (state == 0)
            self->last_on = self->on;
        else if (state == 1)
            self->curr_on += self->on;
    }
    self->curr_on++;
    self->state = state;
}

