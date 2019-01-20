#include <stdint.h>
#include <stdbool.h>
#include "gpo_p8_20.h"

volatile register uint32_t __R30;

void set_p8_20(struct gpo* self, bool value);

void gpo_p8_20_init(struct gpo* self)
{
    self->set = &set_p8_20;
}

/// Set P8_20 GPO to value - R30 bit 13
void set_p8_20(struct gpo* self, bool value)
{
    __R30 = (__R30 & (~(1 << 13))) | (value << 13);
}
