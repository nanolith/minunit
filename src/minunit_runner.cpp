/**
 * \file src/minunit_runner.cpp
 *
 * \brief Simple test runner for minunit.
 *
 * \copyright 2019 Justin Handville.  Please see LICENSE.txt in this
 * distribution for more information.
 */

#include <config.h>
#include <minunit/minunit.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#ifdef FORKED_TEST_RUNNER
# include <signal.h>
# include <sys/socket.h>
# include <sys/wait.h>
#endif

#ifdef FORKED_TEST_RUNNER
static bool child_process_died = false;
static pid_t child;

static void set_child_process_died(int)
{
    child_process_died = true;

    int status;
    waitpid(child, &status, 0);
}
#endif

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
 * \brief Setup action function.
 */
typedef void (*setup_action_func_t)(void*);

/**
 * No-op setup action.
 */
static void no_op_setup_action(void*)
{
}

#ifdef FORKED_TEST_RUNNER
pid_t fork_test_runner(int parentfd, int childfd)
{
    child = fork();

    /* parent */
    if (child != 0)
    {
        close(childfd);
    }
    /* child */
    else
    {
        close(parentfd);
    }

    return child;
}

static void parent_setup_action(void* ctx)
{
    int* s = (int*)ctx;

    char ch = 0;

    write(*s, &ch, sizeof(ch));
}

static void child_setup_action(void* ctx)
{
    int* s = (int*)ctx;

    char ch;

    read(*s, &ch, sizeof(ch));
}

static void write_test_result(void* ctx, bool result)
{
    int* s = (int*)ctx;

    uint32_t val = result ? 1 : 0;

    write(*s, &val, sizeof(val));
}

static bool read_test_result(void* ctx)
{
    int* s = (int*)ctx;

    uint32_t val;

    if (read(*s, &val, sizeof(val)) < sizeof(val))
    {
        set_child_process_died(0);
        return false;
    }

    if (val)
        return true;
    else
        return false;
}
#endif

/**
 * \brief Run the unit tests.
 */
static int test_runner(const minunit_test_options_t* minunit_reserved_options)
{
    int ret = 0;

    setup_action_func_t setup_action = nullptr;
    void* forked_context = nullptr;

#ifdef FORKED_TEST_RUNNER
    int pair[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) < 0)
        perror("socketpair");

    pid_t child = fork_test_runner(pair[0], pair[1]);

    if (child != 0)
    {
        setup_action = parent_setup_action;
        forked_context = pair;
    }
    else
    {
        setup_action = child_setup_action;
        forked_context = pair + 1;
    }
#else
    setup_action = no_op_setup_action;
    forked_context = nullptr;
#endif

    /* first, reverse the list of registered tests. */
    minunit_list_reverse(&minunit_test_cases);

    /* count suites and tests. */
    unsigned int suites = 0;
    unsigned int tests = 0;
    unsigned int fail_count = 0;
    minunit_list_count(minunit_test_cases, &suites, &tests);

    minunit_reserved_options->terminal_set_color(MINUNIT_TERMINAL_COLOR_NORMAL);
#ifdef FORKED_TEST_RUNNER
    if (child != 0)
    {
#endif
        printf("[%s] Executing %u suite%s with %s%u test%s.\n",
               "==========",
               suites, suites == 0 || suites > 1 ? "s" : "",
               tests == 0 || tests > 1 ? "a total " : "",
               tests, tests == 0 || tests > 1 ? "s" : "");
#ifdef FORKED_TEST_RUNNER
    }
#endif

    const char* suite = "";
    const char* suite_tag = "";

    /* run the tests. */
    minunit_test_case_t* test = minunit_test_cases;
    while (test)
    {
        /* is this a suite? */
        if (MINUNIT_TEST_TYPE_SUITE == test->type)
        {
#ifdef FORKED_TEST_RUNNER
            if (child != 0)
            {
#endif
                if (strcmp(suite,""))
                {
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_NORMAL);
                    printf("[%s]\n",
                           "----------");
                    printf("\n");
                }

                suite = test->name;
                suite_tag = "::";

                minunit_reserved_options->terminal_set_color(
                    MINUNIT_TERMINAL_COLOR_NORMAL);
                printf("[%s]\n",
                       "----------");
                printf("[%s] %s\n",
                       " SUITE    ", suite);

#ifdef FORKED_TEST_RUNNER
            }
#endif
        }
        else
        {
#ifdef FORKED_TEST_RUNNER
            if (child != 0)
            {
#endif
                minunit_reserved_options->terminal_set_color(
                    MINUNIT_TERMINAL_COLOR_GREEN);
                printf("[%s]", 
                       " RUN      ");
                minunit_reserved_options->terminal_set_color(
                    MINUNIT_TERMINAL_COLOR_NORMAL);
                printf(" Test %s%s%s\n",
                       suite, suite_tag, test->name);
#ifdef FORKED_TEST_RUNNER
            }
#endif

            minunit_test_context_t result = { true };

            /* run the setup action before executing this test. */
            setup_action(forked_context);

#ifdef FORKED_TEST_RUNNER
            if (child == 0)
            {
#endif
                test->method(minunit_reserved_options, &result);
#ifdef FORKED_TEST_RUNNER

                write_test_result(forked_context, result.pass);
            }

            if (child != 0)
            {
                result.pass = read_test_result(forked_context);

                if (child_process_died)
                {
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_RED);
                    printf("[%s]",
                           "  CRASH   ");
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_NORMAL);
                    printf(" Test %s%s%s\n",
                           suite, suite_tag, test->name);

                    return 1;
                }
            }
#endif

#ifdef FORKED_TEST_RUNNER
            if (child != 0)
            {
#endif
                if (!result.pass)
                {
                    test->failed = true;
                    ++fail_count;
                    ret = 1;
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_RED);
                    printf("[%s]",
                           "   FAIL   ");
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_NORMAL);
                    printf(" Test %s%s%s\n",
                           suite, suite_tag, test->name);
                }
                else
                {
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_GREEN);
                    printf("[%s]",
                           "       OK ");
                    minunit_reserved_options->terminal_set_color(
                        MINUNIT_TERMINAL_COLOR_NORMAL);
                    printf(" Test %s%s%s\n",
                           suite, suite_tag, test->name);
                }
#ifdef FORKED_TEST_RUNNER
            }
#endif
        }

        test = test->next;
    }

#ifdef FORKED_TEST_RUNNER
    if (child == 0)
    {
        return 0;
    }
    else
    {
        int status;
        waitpid(child, &status, 0);
    }
#endif

    if (strcmp(suite,""))
    {
        minunit_reserved_options->terminal_set_color(
            MINUNIT_TERMINAL_COLOR_NORMAL);
        printf("[%s]\n",
               "----------");
        printf("\n");
    }

    minunit_reserved_options->terminal_set_color(
        MINUNIT_TERMINAL_COLOR_NORMAL);
    printf("[%s] Test Summary \n", "==========");

    if (fail_count > 0)
    {
        minunit_reserved_options->terminal_set_color(
            MINUNIT_TERMINAL_COLOR_NORMAL);
        printf("[%s] Encountered %u failure%s:\n",
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
                minunit_reserved_options->terminal_set_color(
                    MINUNIT_TERMINAL_COLOR_RED);
                printf("[%s]",
                       "   FAIL   ");
                minunit_reserved_options->terminal_set_color(
                    MINUNIT_TERMINAL_COLOR_NORMAL);
                printf(" %s%s%s\n", suite, suite_tag, test->name);
            }

            minunit_test_case_t* next = test->next;
            memset(test, 0, sizeof(minunit_test_case_t));
            free(test);
            test = next;
        }
    }
    else
    {
        minunit_reserved_options->terminal_set_color(
            MINUNIT_TERMINAL_COLOR_GREEN);
        printf("[%s] All tests passed.\n",
               "       OK ");
        minunit_reserved_options->terminal_set_color(
            MINUNIT_TERMINAL_COLOR_NORMAL);
    }

    return ret;
}

static void ansi_terminal_set_color(int color)
{
    switch (color)
    {
        case MINUNIT_TERMINAL_COLOR_GREEN:
            printf("\033[32m");
            break;

        case MINUNIT_TERMINAL_COLOR_RED:
            printf("\033[31m");
            break;

        default:
            printf("\033[0m");
            break;
    }
}

static void no_set_color(int color)
{
    (void)color;
}

bool running_in_color_terminal()
{
    /* with isatty() support, if we are using redirection, disable color. */
#ifdef HAS_ISATTY
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout)))
        return false;
#endif

    /* no-color.org suggestion. */
    if (NULL != getenv("NO_COLOR"))
        return false;

    /* by default, use color. */
    return true;
}

static void install_dead_child_reaper()
{
#ifdef FORKED_TEST_RUNNER
    signal(SIGCHLD, set_child_process_died);
#endif
}

int main(int argc, char* argv[])
{
    minunit_test_options_t options;

    install_dead_child_reaper();

    if (running_in_color_terminal())
        options.terminal_set_color = &ansi_terminal_set_color;
    else
        options.terminal_set_color = &no_set_color;

    return test_runner(&options);
}
