#ifndef _GPO_H
#define _GPO_H

#include <stdbool.h>

struct gpo
{
    void (*set)(struct gpo* self, bool value);
};

#endif
