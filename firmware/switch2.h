#ifndef _SWITCH2_H_
#define _SWITCH2_H_

#include <stdint.h>
#include <stdbool.h>

#define _SWITCH2_ID "switch2"

bool is_switch2_id(char *str);

struct switch2
{
    bool state;
};

void switch2_init(struct switch2* self);
void switch2_deinit(struct switch2* self);

#endif
