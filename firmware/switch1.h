#ifndef _SWITCH1_H_
#define _SWITCH1_H_

#include <stdint.h>
#include <stdbool.h>
#include "debouncer.h"

#define _SWITCH1_ID "switch1"

struct switch1
{
    bool state;
    struct debouncer* debouncer;
    void (*update)(struct switch1* self, uint32_t curr_cyc);

    // TODO: Remove all this once working
    uint32_t _last_change_cyc;
    uint32_t _switch1_start_cyc;
    int32_t _switch1_curr_ms;
    uint32_t _switch1_curr_cyc_plus;
    uint32_t _switch1_curr_cyc_in_ms;
    int32_t _switch1_last_ms;
    void (*on_will_change)(void);
    void (*on_did_change)(void);
};

bool is_switch1_id(char *str);
void switch1_init(struct switch1* self, void (*on_will_change)(void), void (*on_did_change)(void));
void switch1_deinit(struct switch1* self);

#endif
