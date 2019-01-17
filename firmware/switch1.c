#include <stdlib.h>
#include <string.h>
#include "switch1.h"

#define CYCLES_PER_MS 200000
#define DEBOUNCE_MS 100

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void set_p8_11(bool value);
bool get_p8_15(void);
void update(struct switch1* self, uint32_t curr_cyc);

bool is_switch1_id(char *str)
{
    return strncmp(str, _SWITCH1_ID, strlen(_SWITCH1_ID)) == 0;
}

void switch1_init(struct switch1* self, void (*on_will_change)(void), void (*on_did_change)(void))
{
    self = malloc(sizeof(struct switch1));
    debouncer_init(self->debouncer, DEBOUNCE_MS);
    set_p8_11(1);
    self->state = get_p8_15();
    self->update = &update;

    // TODO: Remove all this once working
    self->_last_change_cyc = 0;
    self->_switch1_start_cyc = 0;
    self->_switch1_curr_ms = 0;
    self->_switch1_curr_cyc_plus = 0;
    self->_switch1_curr_cyc_in_ms = 0;
    self->_switch1_last_ms = -1;
    self->on_will_change = on_will_change;
    self->on_did_change = on_did_change;
}

void switch1_deinit(struct switch1* self)
{
    debouncer_deinit(self->debouncer);
    free(self);
}

/// Set P8_11 GPO to value - R30 bit 15
void set_p8_11(bool value)
{
    __R30 = (__R30 & (~(1 << 15))) | (value << 15);
}

/// Return P8_15 GPI value - R31 bit 15
bool get_p8_15(void)
{
    return (__R31 >> 15) & 1;
}

void update(struct switch1* self, uint32_t curr_cyc)
{
    if (get_p8_15() != self->state && self->debouncer->is_stable) // If state change just started
    {
        self->debouncer->debounce(self->debouncer, curr_cyc);
        return;
    }
    else // If during state change
    {
        self->debouncer->debounce(self->debouncer, curr_cyc);
        if (self->debouncer->is_stable)
        {
            self->on_will_change();

            self->state = get_p8_15();

            // TODO: Remove all this once working
            self->_last_change_cyc = curr_cyc;
            self->_switch1_curr_cyc_plus = curr_cyc - self->_switch1_start_cyc;
            self->_switch1_curr_cyc_in_ms = self->_switch1_curr_cyc_plus / CYCLES_PER_MS;
            self->_switch1_last_ms = self->_switch1_curr_ms + self->_switch1_curr_cyc_in_ms;
            self->_switch1_start_cyc = curr_cyc;
            self->_switch1_curr_ms = 0;

            self->on_did_change();
        }
    }
}
