#include <stdint.h>
#include <stdbool.h>
#include "mock_gpo.h"

bool spy_gpo_value = 0;

void mock_gpo_set(struct gpo* self, bool value);

void init_mock_gpo(struct gpo *self)
{
    self->set = &mock_gpo_set;
}

void mock_gpo_set(struct gpo* self, bool value)
{
    spy_gpo_value = value;
}