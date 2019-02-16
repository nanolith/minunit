/**
 * \file examples/minmax/src/max.c
 *
 * Implementation of max function.
 */

#include "minmax.h"

int64_t example_max(int64_t x, int64_t y)
{
    if (x >= y)
        return x;
    else
        return y;
}
