#include "mock_debounced_gpi.h"

bool mock_deb_gpi_debounce = true;
bool mock_deb_gpi_debounce_is_stable = true;

static bool debounce(struct debounced_gpi *self);

void init_mock_debounced_gpi(struct debounced_gpi *self)
{
    self->is_stable = 1;
    self->state = 0;
    self->bounce = 0;
    self->debounce = debounce;
}

static bool debounce(struct debounced_gpi *self)
{
    self->is_stable = mock_deb_gpi_debounce_is_stable;
    return mock_deb_gpi_debounce;
}
