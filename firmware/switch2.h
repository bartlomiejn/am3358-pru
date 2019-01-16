#include <stdint.h>
#include <stdbool.h>

#define _SWITCH2_ID "switch2"

bool is_switch2_id(char *str);

struct switch2
{
    bool _state;
};

void switch2_init(struct switch2* self);
