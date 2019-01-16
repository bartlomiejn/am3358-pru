#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "switch2.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void _set_p8_20(bool value);
bool _get_p8_21(void);

bool is_switch2_id(char *str)
{
    return strncmp(str, _SWITCH2_ID, strlen(_SWITCH2_ID)) == 0;
}

void switch2_init(struct switch2* self)
{
    _set_p8_20(1);
    self->_state = _get_p8_21();
}

/// Set P8_20 GPO to value - R30 bit 13
void _set_p8_20(bool value)
{
    __R30 = (__R30 & (~(1 << 13))) | (value << 13);
}

/// Return P8_21 GPI value - R31 bit 12
bool _get_p8_21(void)
{
    return (__R31 >> 12) & 1;
}
