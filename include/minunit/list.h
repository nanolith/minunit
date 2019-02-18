/**
 * \file minunit/list.h
 *
 * \brief List functionality for minunit.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#ifndef  MINUNIT_LIST_HEADER_GUARD
# define MINUNIT_LIST_HEADER_GUARD

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <minunit/internal.h>
#include <stdbool.h>

/**
 * \brief Reverse a test case list.
 *
 * \param root      The root of the test case list.
 */
void minunit_list_reverse(minunit_test_case_t** root);

/**
 * \brief Count the suites and unit tests in this test list.
 *
 * \param root      The root of the test case list.
 * \param suites    The number of test suites found.
 * \param tests     The number of tests found.
 */
void minunit_list_count(
    minunit_test_case_t* root, unsigned int *suites, unsigned int *tests);

#ifdef MODEL_CHECK
    inline bool minunit_prop_list_node_valid_helper(minunit_test_case_t* x)
    {
        while (NULL != x)
        {
            if (MINUNIT_TEST_TYPE_SUITE != x->type
             && MINUNIT_TEST_TYPE_UNIT != x->type)
                return false;

            if (NULL == x->name)
                return false;

            if (x->failed)
                return false;

            if (MINUNIT_TEST_TYPE_UNIT == x->type
             && NULL == x->method)
                return false;

            if (MINUNIT_TEST_TYPE_SUITE == x->type
             && NULL != x->method)
                return false;

            x = x->next;
        }

        return true;
    }
#endif /*MODEL_CHECK*/

/**
 * Valid list property.
 *
 * \param x         Pointer to a list node.
 *
 * \returns true if valid and false if not.
 */
#define MINUNIT_PROP_LIST_NODE_VALID(x) \
    minunit_prop_list_node_valid_helper(x)

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*MINUNIT_LIST_HEADER_GUARD*/
