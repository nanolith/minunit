/**
 * \file minunit/internal.h
 *
 * \brief Internals for minunit suite.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#ifndef  MINUNIT_INTERNAL_HEADER_GUARD
# define MINUNIT_INTERNAL_HEADER_GUARD

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <stdbool.h>
#include <stdio.h>

/**
 * \brief Enumeration for supported terminal colors.
 */
enum minunit_terminal_colors
{
    MINUNIT_TERMINAL_COLOR_NORMAL,
    MINUNIT_TERMINAL_COLOR_GREEN,
    MINUNIT_TERMINAL_COLOR_RED,
    MINUNIT_TERMINAL_COLOR_INVALID
};

/**
 * \brief Function type to set terminal color.
 */
typedef void (*terminal_set_color_func_t)(int color);

/**
 * \brief Global test options.
 */
typedef struct minunit_test_options
{
    terminal_set_color_func_t terminal_set_color;
} minunit_test_options_t;

/**
 * \brief Simple test context that exposes a pass or fail flag.
 */
typedef struct minunit_test_context
{
    bool pass;
} minunit_test_context_t;

/**
 * \brief Type of a minunit test function.
 */
typedef void (*minunit_test_func_t)(
    const minunit_test_options_t*, minunit_test_context_t*);

/**
 * \brief Internal method to register a minunit test.
 *
 * \param test_func     The test function to register.
 * \param name          The name of this test function.
 *
 * \returns 0 on success and non-zero on failure.
 */
int minunit_register_test(minunit_test_func_t test_func, const char* name);

/**
 * \brief Internal method to register a minunit test suite.
 *
 * \param name          The name of this test suite.
 *
 * \returns 0 on success and non-zero on failure.
 */
int minunit_register_suite(const char* name);

/**
 * \brief Internal enumeration to determine whether a node is a test case or a
 * suite.
 */
enum minunit_test_type
{
    MINUNIT_TEST_TYPE_SUITE,
    MINUNIT_TEST_TYPE_UNIT
};

/**
 * \brief Internal enumeration of test flags.
 */
enum minunit_test_flag
{
    MINUNIT_TEST_FLAG_ENABLED           = 1,
};

/**
 * A node in the test case linked list, which can be either a unit test or a
 * suite tag.
 */
typedef struct minunit_test_case
{
    struct minunit_test_case* next;
    enum minunit_test_type type;
    const char* name;
    minunit_test_func_t method;
    bool failed;
} minunit_test_case_t;

/**
 * \brief Internal macro.  Do not use.
 */
#define TEST_ASSERT_MESSAGE(message, file, line, cond) \
    do { \
        if (cond) \
        { \
        } \
        else \
        { \
            printf("%s:%d: ", file, line); \
            minunit_reserved_options->terminal_set_color( \
                MINUNIT_TERMINAL_COLOR_RED); \
            printf("error"); \
            minunit_reserved_options->terminal_set_color( \
                MINUNIT_TERMINAL_COLOR_NORMAL); \
            printf(": expecting %s.\n", message); \
            minunit_reserved_context->pass = false; \
            return; \
        } \
    } while (0)

/**
 * \brief Internal macro.  Do not use.
 */
#define TEST_EXPECT_MESSAGE(message, file, line, cond) \
    do { \
        if (cond) \
        { \
        } \
        else \
        { \
            printf("%s:%d: ", file, line); \
            minunit_reserved_options->terminal_set_color( \
                MINUNIT_TERMINAL_COLOR_RED); \
            printf("error"); \
            minunit_reserved_options->terminal_set_color( \
                MINUNIT_TERMINAL_COLOR_NORMAL); \
            printf(": expecting %s.\n", message); \
            minunit_reserved_context->pass = false; \
        } \
    } while (0)

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*MINUNIT_INTERNAL_HEADER_GUARD*/
