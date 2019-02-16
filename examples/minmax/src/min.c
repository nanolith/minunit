/**
 * \file examples/minmax/src/min.c
 *
 * Implementation of min function.
 */

#include "minmax.h"

int64_t example_min(int64_t x, int64_t y)
{
    if (x <= y)
        return x;
    else
        return y;
}
