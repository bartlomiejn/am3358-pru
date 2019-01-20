#ifndef _MOCK_GPI_H
#define _MOCK_GPI_H

#include <stdbool.h>
#include "hardware/gpi/gpi.h"

extern bool mock_gpi_value;

void mock_gpi_init(struct gpi *self);

#endif
