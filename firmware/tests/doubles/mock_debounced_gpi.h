#ifndef _MOCK_DEBOUNCED_GPI_H
#define _MOCK_DEBOUNCED_GPI_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/gpi/debounced_gpi.h"

extern bool mock_deb_gpi_debounce;
extern bool mock_deb_gpi_debounce_is_stable;

void init_mock_debounced_gpi(struct debounced_gpi *self);

#endif
