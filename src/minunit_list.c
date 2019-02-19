/**
 * \file src/minunit_list.cpp
 *
 * \brief List routines for minunit.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#include <minunit/minunit.h>
#include <modelcheck/model_assert.h>

/**
 * \brief Reverse a test case list.
 *
 * \param root      The root of the test case list.
 */
void minunit_list_reverse(minunit_test_case_t** root)
{
    minunit_test_case_t* prev = NULL;
    minunit_test_case_t* curr = *root;
    minunit_test_case_t* next = NULL;

    MODEL_ASSERT(MINUNIT_PROP_LIST_NODE_VALID(prev));
    MODEL_ASSERT(MINUNIT_PROP_LIST_NODE_VALID(curr));

    while (NULL != curr)
    {
        MODEL_ASSERT(MINUNIT_PROP_LIST_NODE_VALID(curr));

        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    *root = prev;

    MODEL_ASSERT(MINUNIT_PROP_LIST_NODE_VALID(*root));
}

/**
 * \brief Count the suites and unit tests in this test list.
 *
 * \param root      The root of the test case list.
 * \param suites    The number of test suites found.
 * \param tests     The number of tests found.
 */
void minunit_list_count(
    minunit_test_case_t* root, unsigned int *suites, unsigned int *tests)
{
    minunit_test_case_t* curr = root;

    MODEL_ASSERT(NULL != suites);
    MODEL_ASSERT(NULL != tests);

    *suites = 0;
    *tests = 0;

    while (NULL != curr)
    {
        MODEL_ASSERT(MINUNIT_PROP_LIST_NODE_VALID(curr));

        if (MINUNIT_TEST_TYPE_SUITE == curr->type)
            *suites += 1;
        if (MINUNIT_TEST_TYPE_UNIT == curr->type)
            *tests += 1;

        curr = curr->next;
    }
}
