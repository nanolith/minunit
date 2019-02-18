/**
 * \file src/minunit_runner.cpp
 *
 * \brief Simple test runner for minunit.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#include <minunit/minunit.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Global linked list of test cases and suites.
 */
static minunit_test_case_t* minunit_test_cases = nullptr;

/**
 * \brief Internal method to register a minunit test suite.
 *
 * \param name          The name of this test suite.
 *
 * \returns 0 on success and non-zero on failure.
 */
int minunit_register_suite(const char* name)
{
    /* create the test suite entry. */
    minunit_test_case_t* newtest =
        (minunit_test_case_t*)malloc(sizeof(minunit_test_case_t));
    memset(newtest, 0, sizeof(minunit_test_case_t));

    /* initialize the entry. */
    newtest->next = minunit_test_cases;
    newtest->type = MINUNIT_TEST_TYPE_SUITE;
    newtest->name = name;

    /* add the entry to the linked list. */
    minunit_test_cases = newtest;

    return 0;
}

int minunit_register_test(minunit_test_func_t test_func, const char* name)
{
    /* create unit test entry. */
    minunit_test_case_t* newtest =
        (minunit_test_case_t*)malloc(sizeof(minunit_test_case_t));
    memset(newtest, 0, sizeof(minunit_test_case_t));

    /* initialize unit test entry. */
    newtest->next = minunit_test_cases;
    newtest->type = MINUNIT_TEST_TYPE_UNIT;
    newtest->name = name;
    newtest->method = test_func;

    /* add the entry to the linked list. */
    minunit_test_cases = newtest;

    return 0;
}

/**
 * \brief Run the unit tests.
 */
static int test_runner()
{
    int ret = 0;

    /* first, reverse the list of registered tests. */
    minunit_list_reverse(&minunit_test_cases);

    /* count suites and tests. */
    unsigned int suites = 0;
    unsigned int tests = 0;
    unsigned int fail_count = 0;
    minunit_list_count(minunit_test_cases, &suites, &tests);

    printf("\033[0m[%s]\033[0m Executing %u suite%s with %s%u test%s.\n",
           "==========",
           suites, suites == 0 || suites > 1 ? "s" : "",
           tests == 0 || tests > 1 ? "a total " : "",
           tests, tests == 0 || tests > 1 ? "s" : "");

    const char* suite = "";
    const char* suite_tag = "";

    /* run the tests. */
    minunit_test_case_t* test = minunit_test_cases;
    while (test)
    {
        /* is this a suite? */
        if (MINUNIT_TEST_TYPE_SUITE == test->type)
        {
            if (strcmp(suite,""))
            {
                printf("\033[0m[%s]\033[0m\n",
                       "----------");
                printf("\n");
            }

            suite = test->name;
            suite_tag = "::";

            printf("\033[0m[%s]\033[0m\n",
                   "----------");
            printf("\033[0m[%s]\033[0m %s\n",
                   " SUITE    ", suite);

        }
        else
        {
            printf("\033[32m[%s]\033[0m Test %s%s%s\n",
                   " RUN      ", 
                   suite, suite_tag, test->name);

            minunit_test_context_t result = { true };
            test->method(&result);
            if (!result.pass)
            {
                test->failed = true;
                ++fail_count;
                ret = 1;
                printf("\033[31m[%s]\033[0m Test %s%s%s\n",
                       "   FAIL   ",
                       suite, suite_tag, test->name);
            }
            else
            {
                printf("\033[32m[%s]\033[0m Test %s%s%s\n",
                       "       OK ",
                       suite, suite_tag, test->name);
            }
        }

        test = test->next;
    }

    if (strcmp(suite,""))
    {
        printf("\033[0m[%s]\033[0m\n",
               "----------");
        printf("\n");
    }

    printf("\033[0m[%s]\033[0m Test Summary \n", "==========");

    if (fail_count > 0)
    {
        printf("\033[0m[%s]\033[0m Encountered %u failure%s:\n",
               "----------", fail_count, fail_count > 1 ? "s" : "");

        test = minunit_test_cases;
        suite = "";
        suite_tag = "";

        while (nullptr != test)
        {
            if (MINUNIT_TEST_TYPE_SUITE == test->type)
            {
                suite = test->name;
                suite_tag = "::";
            }
            else if (test->failed)
            {
                printf("\033[31m[%s]\033[0m %s%s%s\n",
                       "   FAIL   ", suite, suite_tag, test->name);
            }

            minunit_test_case_t* next = test->next;
            memset(test, 0, sizeof(minunit_test_case_t));
            free(test);
            test = next;
        }
    }
    else
    {
        printf("\033[32m[%s] All tests passed.\033[0m\n",
               "       OK ");
    }

    return ret;
}

int main(int argc, char* argv[])
{
    return test_runner();
}
