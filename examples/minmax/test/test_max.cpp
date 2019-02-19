/**
 * \file examples/minmax/test/test_max.cpp
 *
 * Unit tests for example_max.
 */

#include <minunit/minunit.h>

#include "../src/minmax.h"

#include <stdlib.h>

TEST_SUITE(max);

TEST(positive)
{
    int64_t x = 44;
    int64_t y = 55;

    TEST_EXPECT(y == example_max(x, y));
    TEST_EXPECT(y == example_max(y, x));
}

TEST(negative)
{
    int64_t x = -44;
    int64_t y = -55;

    TEST_EXPECT(x == example_max(x, y));
    TEST_EXPECT(x == example_max(y, x));
}
