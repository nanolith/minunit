Minimum Unit Test Framework
===========================

The Minimum Unit Test Framework provides a minimum test runner and unit test
registration mechanism so that unit tests can be executed against C/C++.  This
framework requires that tests be written in C++, but these can easily be used to
test C code.

This framework organizes unit tests into test suites.  Each test file should
begin with a `TEST_SUITE` declaration to name the test suite.  For instance:

```c++
    TEST_SUITE(foo);
```

Tests are written using the `TEST` macro.  The user should provide a unique name
per test.  Test macros work a lot like function definitions.  For instance:

```c++
    TEST(shell_init_basic)
    {
        //...
    }
```

Test assertions are written using the `TEST_ASSERT` and `TEST_EXPECT` macros.
The difference between the two is that a `TEST_ASSERT` returns immediately if a
failure occurs, and a `TEST_EXPECT` continues execution after logging a test
failure.  Generally, `TEST_EXPECT` should be used unless an assertion is needed
to protect downstream code.  For instance, use a `TEST_ASSERT` when checking
that a pointer is valid prior to checking values pointed to by that pointer.

```c++
    TEST(pointer_valid)
    {
        auto ptr = instrumented_method();

        //don't go further if the pointer is invalid.
        TEST_ASSERT(nullptr != ptr);

        //Verify that the pointer values are properly set.
        TEST_EXPECT(15 == ptr->first);
        TEST_EXPECT(20 == ptr->second);
    }
```

In the above example, if `ptr` is null, then the assertion causes an immediate
exit of the unit test with a failure.  Otherwise, the expect checks below verify
values in the structure pointed to by this pointer.

Test Runner
===========

By default, libmintest provides a test runner that executes tests from the
console.  This test runner assumes an ANSI compatible console.  In future
versions of this library, it will be possible to substitute in an alternative
test runner so that tests can be run from a GUI, in an embedded context, or via
some other user-defined mechanism.
