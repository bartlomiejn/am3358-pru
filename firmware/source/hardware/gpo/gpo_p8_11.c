#include <stdint.h>
#include <stdbool.h>
#include "gpo_p8_11.h"

volatile register uint32_t __R30;

void set_p8_11(struct gpo* self, bool value);

void gpo_p8_11_init(struct gpo* self)
{
    self->set = &set_p8_11;
}

/// Set P8_11 to value - R30 bit 15
void set_p8_11(struct gpo* self, bool value)
{
    __R30 = (__R30 & (~(1 << 15))) | (value << 15);
}
