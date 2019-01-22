#include <stdint.h>
#include "gpi_p8_16.h"

volatile register uint32_t __R31;

bool get_p8_16(struct gpi* self);

void gpi_p8_16_init(struct gpi* self)
{
    self->get = &get_p8_16;
}

/// Return P8_16 value - R31 bit 14
bool get_p8_16(struct gpi* self)
{
    return (__R31 >> 14) & 1;
}
