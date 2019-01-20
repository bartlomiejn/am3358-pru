#include "mock_gpi.h"

bool mock_gpi_value = 0;

bool mock_gpi_get(struct gpi *self);

void mock_gpi_init(struct gpi *self)
{
    self->get = &mock_gpi_get;
}

bool mock_gpi_get(struct gpi *self)
{
    return mock_gpi_value;
}
