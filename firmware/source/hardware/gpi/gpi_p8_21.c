#include <stdint.h>
#include "gpi_p8_21.h"

volatile register uint32_t __R31;

bool get_p8_21(struct gpi* self);

void gpi_p8_21_init(struct gpi* self)
{
    self->get = &get_p8_21;
}

/// Return P8_21 value - R31 bit 12
bool get_p8_21(struct gpi* self)
{
    return (__R31 >> 12) & 1;
}
