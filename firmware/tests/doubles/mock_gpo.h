#ifndef _MOCK_GPO_H
#define _MOCK_GPO_H

#include <stdbool.h>
#include "hardware/gpo/gpo.h"

extern bool spy_gpo_value;

void init_mock_gpo(struct gpo *self);

#endif
