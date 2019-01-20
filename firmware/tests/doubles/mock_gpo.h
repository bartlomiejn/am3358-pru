#ifndef _MOCK_GPO_H
#define _MOCK_GPO_H

#include <stdbool.h>
#include "hardware/gpo/gpo.h"

extern bool spy_gpo_value;

void mock_gpo_init(struct gpo *self);

#endif
