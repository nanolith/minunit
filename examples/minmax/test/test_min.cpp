/**
 * \file examples/minmax/test/test_min.cpp
 *
 * Unit tests for example_min.
 */

#include <minunit/minunit.h>

#include "../src/minmax.h"

TEST_SUITE(min);

TEST(positive)
{
    int64_t x = 44;
    int64_t y = 55;

    TEST_EXPECT(x == example_min(x, y));
    TEST_EXPECT(x == example_min(y, x));
}

TEST(negative)
{
    int64_t x = -44;
    int64_t y = -55;

    TEST_EXPECT(y == example_min(x, y));
    TEST_EXPECT(y == example_min(y, x));
}
