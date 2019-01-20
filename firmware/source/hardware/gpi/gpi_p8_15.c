#include <stdint.h>
#include "gpi_p8_15.h"

volatile register uint32_t __R31;

bool get_p8_15(struct gpi* self);

void gpi_p8_15_init(struct gpi* self)
{
    self->get = &get_p8_15;
}

/// Return P8_15 GPI value - R31 bit 15
bool get_p8_15(struct gpi* self)
{
    return (__R31 >> 15) & 1;
}
