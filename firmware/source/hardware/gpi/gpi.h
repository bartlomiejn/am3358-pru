#ifndef _GPI_H
#define _GPI_H

#include <stdbool.h>

struct gpi
{
    bool (*get)(struct gpi* self);
};

#endif
