#include <stdint.h>
#include <stdbool.h>
#include "mock_gpo.h"

bool spy_gpo_value = 0;

void mock_gpo_set(struct gpo* self, bool value);

void mock_gpo_init(struct gpo* self)
{
    self->set = &mock_gpo_set;
}

void mock_gpo_set(struct gpo* self, bool value)
{
    spy_gpo_value = value;
}