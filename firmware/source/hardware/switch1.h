#ifndef _SWITCH1_H_
#define _SWITCH1_H_

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpo/gpo.h"
#include "hardware/gpi/gpi.h"
#include "hardware/cycle_counter/cycle_counter.h"
#include "software/debouncer.h"

struct switch1
{
    struct gpi* gpi;
    struct gpo* gpo;
    struct cycle_counter* counter;
    struct debouncer* debouncer;
    bool state;
    int32_t last_change_ms;
    int32_t change_ms;
    uint32_t change_cyc;
    void (*update)(struct switch1* switch1);
};

void switch1_init(
    struct switch1* self,
    struct cycle_counter* counter,
    struct debouncer* debouncer,
    struct gpi* gpi,
    struct gpo* gpo
);

#endif
