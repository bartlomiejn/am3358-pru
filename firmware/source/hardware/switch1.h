#ifndef _SWITCH1_H_
#define _SWITCH1_H_

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpo/gpo.h"
#include "hardware/gpi/debounced_gpi.h"

struct switch1
{
    struct debounced_gpi* gpi;
    bool state;
    int32_t last_time;
    int32_t time;
    int32_t curr_time;
    void (*update)(struct switch1* switch1);
};

void switch1_init(
    struct switch1* self,
    struct debounced_gpi* gpi,
    struct gpo* gpo
);

#endif
