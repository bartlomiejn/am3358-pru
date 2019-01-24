#ifndef _DEBOUNCED_GPI_H
#define _DEBOUNCED_GPI_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/gpi/gpi.h"

struct debounced_gpi
{
    struct gpi* gpi;
    bool is_stable;
    bool state;
    uint16_t bounce;
    bool (*debounce)(struct debounced_gpi *self);
};

void init_debounced_gpi(struct debounced_gpi *self, struct gpi* gpi);

#endif
