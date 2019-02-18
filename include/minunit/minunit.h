/**
 * \file minunit/minunit.h
 *
 * \brief Minimal unit testing, in C/C++.
 *
 * This header creates a simple minimal unit testing framework which can be used
 * to test C/C++ code.  The purpose of this framework is to eliminate external
 * dependencies to better simplify the code base.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#ifndef  MINUNIT_MINUNIT_HEADER_GUARD
# define MINUNIT_MINUNIT_HEADER_GUARD

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <stdio.h>
#include <minunit/internal.h>
#include <minunit/list.h>

/**
 * \brief Begin a test suite.
 *
 * From this point on, all unit tests will belong to this suite. Note that this
 * is an all-or-nothing thing.  Either use test suites or don't, but don't mix
 * suites and non-suite code.  Place this declaration at the beginning of a test
 * source file.
 */
#define TEST_SUITE(name) \
    static int minunit_reserved_## name ##_init = \
        minunit_register_suite(#name)

/**
 * \brief Unit Test definition.
 */

#define TEST(name) \
    static void minunit_reserved_## name ##_test_func( \
        minunit_test_context_t* minunit_reserved_context); \
    static int minunit_reserved_## name ##_init = \
        minunit_register_test(&minunit_reserved_## name ## _test_func, #name); \
    static void minunit_reserved_## name ##_test_func( \
        minunit_test_context_t* minunit_reserved_context)

/**
 * \brief If this is the last statement in a test, and no assertions failed,
 * this forces the test to pass.
 */
#define TEST_SUCCESS() minunit_reserved_context->pass = true;

/**
 * \brief This forces a test to fail.
 */
#define TEST_FAILURE() minunit_reserved_context->pass = false;

/**
 * \brief Assert that a given condition is true.
 *
 * If this assertion fails, the unit test will fail and control will be returned
 * immediately to the runner.
 */
#define TEST_ASSERT(cond) TEST_ASSERT_MESSAGE(#cond, __FILE__, __LINE__, cond)

/**
 * \brief Expect a given condition to be true.
 *
 * If this expectation fails, the unit test will fail but execution will
 * continue.
 */
#define TEST_EXPECT(cond) TEST_EXPECT_MESSAGE(#cond, __FILE__, __LINE__, cond)

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*MINUNIT_MINUNIT_HEADER_GUARD*/
