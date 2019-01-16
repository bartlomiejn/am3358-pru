#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "switch1.h"

#define CYCLES_PER_MS 200000
#define DEBOUNCE_MS 500
#define DEBOUNCE_CYCS (DEBOUNCE_MS * CYCLES_PER_MS)

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void _set_p8_11(bool value);
bool _get_p8_15(void);
void _switch1_update(
    struct switch1* self,
    uint32_t curr_cyc,
    void (*pre_change)(void),
    void (*post_change)(void)
);

bool is_switch1_id(char *str)
{
    return strncmp(str, _SWITCH1_ID, strlen(_SWITCH1_ID)) == 0;
}

void switch1_init(struct switch1* self)
{
    _set_p8_11(1);

    // TODO: Temporary
    self->_last_change_cyc = 0;
    self->_switch1_start_cyc = 0;
    self->_switch1_curr_ms = 0;
    self->_switch1_curr_cyc_plus = 0;
    self->_switch1_curr_cyc_in_ms = 0;
    self->_switch1_last_ms = -1;

    self->_state = _get_p8_15();
    self->update = &_switch1_update;
}

/// Set P8_11 GPO to value - R30 bit 15
void _set_p8_11(bool value)
{
    __R30 = (__R30 & (~(1 << 15))) | (value << 15);
}

/// Return P8_15 GPI value - R31 bit 15
bool _get_p8_15(void)
{
    return (__R31 >> 15) & 1;
}

void _switch1_update(
    struct switch1* self,
    uint32_t curr_cyc,
    void (*pre_change)(void),
    void (*post_change)(void)
){
    // TODO: Implement debounce
    if (_get_p8_15() != self->_state)
    {
        pre_change();

        self->_last_change_cyc = curr_cyc;

        self->_state = !self->_state;

        self->_switch1_curr_cyc_plus = curr_cyc - self->_switch1_start_cyc;
        self->_switch1_curr_cyc_in_ms = self->_switch1_curr_cyc_plus / CYCLES_PER_MS;
        self->_switch1_last_ms = self->_switch1_curr_ms + self->_switch1_curr_cyc_in_ms;

        self->_switch1_start_cyc = curr_cyc;
        self->_switch1_curr_ms = 0;

        post_change();
    }
}
