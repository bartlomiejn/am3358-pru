#include <stdint.h>
#include <stdbool.h>
#include "gpo_p8_12.h"

volatile register uint32_t __R30;

void set_p8_12(struct gpo* self, bool value);

void gpo_p8_12_init(struct gpo* self)
{
    self->set = &set_p8_12;
}

/// Set P8_12 GPO to value - R30 bit 14
void set_p8_12(struct gpo* self, bool value)
{
    __R30 = (__R30 & (~(1 << 14))) | (value << 14);
}
