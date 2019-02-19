/**
 * \file model-check/check_list.c
 *
 * \brief Model check of list routines.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#include <minunit/minunit.h>
#include <modelcheck/model_assert.h>
#include <modelcheck/allocator.h>
#include <string.h>

unsigned int nondet_unsigned_int();

static void dummy_test_func(minunit_test_context_t* ctx)
{
}

int main(int argc, char* argv[])
{
    unsigned int suites, tests;

    /* we can reverse an empty list, and the counts are valid. */
    minunit_test_case_t* empty = NULL;

    minunit_list_reverse(&empty);
    MODEL_ASSERT(NULL == empty);
    suites = nondet_unsigned_int(); tests = nondet_unsigned_int();
    minunit_list_count(empty, &suites, &tests);
    MODEL_ASSERT(0 == suites);
    MODEL_ASSERT(0 == tests);

    /* create a list with one unit test. */
    empty = (minunit_test_case_t*)malloc(sizeof(minunit_test_case_t));
    if (NULL == empty)
    {
        /* escape hatch for nondeterministic bad allocation. */
        return 1;
    }

    /* populate this with valid data. */
    memset(empty, 0, sizeof(minunit_test_case_t));
    empty->name = "simple test";
    empty->type = MINUNIT_TEST_TYPE_UNIT;
    empty->method = &dummy_test_func;
    empty->next = NULL;
    empty->failed = false;

    /* reverse the list. */
    minunit_list_reverse(&empty);
    MODEL_ASSERT(NULL != empty);

    /* count the list. */
    suites = nondet_unsigned_int(); tests = nondet_unsigned_int();
    minunit_list_count(empty, &suites, &tests);
    MODEL_ASSERT(0 == suites);
    MODEL_ASSERT(1 == tests);

    /* create a suite for the test. */
    minunit_test_case_t* next =
        (minunit_test_case_t*)malloc(sizeof(minunit_test_case_t));
    if (NULL == next)
    {
        /* escape hatch for nondeterministic bad allocation. */
        free(empty);
        return 2;
    }

    /* populate this with valid data. */
    memset(next, 0, sizeof(minunit_test_case_t));
    next->name = "simple suite";
    next->type = MINUNIT_TEST_TYPE_SUITE;
    next->method = NULL;
    next->next = NULL;
    next->failed = false;

    /* append this suite to our list. */
    empty->next = next;

    /* reverse the list. */
    minunit_list_reverse(&empty);
    MODEL_ASSERT(NULL != empty);

    /* count the list. */
    suites = nondet_unsigned_int(); tests = nondet_unsigned_int();
    minunit_list_count(empty, &suites, &tests);
    MODEL_ASSERT(1 == suites);
    MODEL_ASSERT(1 == tests);

    /* clean up. */
    while (NULL != empty)
    {
        minunit_test_case_t* tmp = empty;
        empty = empty->next;
        free(tmp);
    }

    return 0;
}
