#ifndef _SWITCH2_H_
#define _SWITCH2_H_

#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpo/gpo.h"
#include "hardware/gpi/debounced_gpi.h"

struct switch2
{
    struct debounced_gpi* gpi;
    bool state;
    int32_t last_on;
    int32_t on;
    int32_t curr_on;
    void (*update)(struct switch2* switch1);
};

void switch2_init(
    struct switch2* self,
    struct debounced_gpi* gpi,
    struct gpo* gpo
);

#endif
