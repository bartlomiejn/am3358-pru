#include "debounced_gpi.h"

static bool debounce(struct debounced_gpi *self);

void init_debounced_gpi(struct debounced_gpi *self, struct gpi* gpi)
{
    self->gpi = gpi;
    self->is_stable = true;
    self->state = gpi->get(gpi);
    self->bounce = self->state ? 0xffff : 0xfc00;
    self->debounce = debounce;
}

static bool debounce(struct debounced_gpi *self)
{
    self->bounce = (self->bounce << 1) | self->gpi->get(self->gpi) | 0xfc00;
    self->is_stable = self->bounce == 0xfc00 || self->bounce == 0xffff;
    if (self->is_stable)
        self->state = self->bounce & 1;
    return self->state;
}
