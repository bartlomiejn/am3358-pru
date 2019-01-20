#ifndef _SWITCH2_H_
#define _SWITCH2_H_

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpo/gpo.h"
#include "hardware/gpi/gpi.h"
#include "hardware/cycle_counter/cycle_counter.h"
#include "software/debouncer.h"

struct switch2
{
    struct gpi* gpi;
    struct gpo* gpo;
    struct cycle_counter* counter;
    struct debouncer* debouncer;
    bool state;
    int32_t last_on_ms;
    int32_t on_ms;
    uint32_t on_cyc;
    void (*update)(struct switch2* switch1);
};

void switch2_init(
    struct switch2* self,
    struct cycle_counter* counter,
    struct debouncer* debouncer,
    struct gpi* gpi,
    struct gpo* gpo
);

#endif
