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
console.  This test runner assumes an ANSI compatible console.  This assumption
can be overridden as described in the next paragraph.  In future versions of
this library, it will be possible to substitute in an alternative test runner so
that tests can be run from a GUI, in an embedded context, or via some other
user-defined mechanism.

ANSI colors can be disabled by redirecting standard input or standard output to
a non-terminal device.  For instance, redirecting standard output to a file will
automatically disable ANSI colors so that the file will be plaintext.
Alternatively, as per the [`NO_COLOR`][no-color-org] standard, setting the
`NO_COLOR` environment variable will disable this feature.

[no-color-org]: https://no-color.org

Building and Installing
=======================

This library has one dependency, which is the [modelcheck][modelcheck-lib]
library.  You will need to install this to a suitable location and configure
CMake below to find it.  If you install it somewhere other than the system
default, then set `CMAKE_PREFIX_PATH` to pick up this location so that the
configuration process below succeeds.

[modelcheck-lib]: https://github.com/nanolith/modelcheck

This library uses CMake.  To install, set the `CMAKE_INSTALL_PREFIX` as
appropriate and build / install via cmake.  Ideally, create a separate build
subdirectory for running the build and install steps.  For instance, on Unix,
the following can be used, starting in the project root.

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
    make
    make install

The last installation step may require root privileges (i.e. run via sudo) if
you are installing to a privileged location.

This library creates a pkg-config file, called minunit.pc, which can be used to
set compile and link options for finding the include files for this library and
to link against the default test runner.

Development of minunit
======================

A `run_tests` target is defined and must pass successfully in order for a pull
request to be accepted into the minunit project.  This requires a relatively
recent snapshot of [CBMC][cbmc-github].  This target performs model checking on
some of the internals for the test runner, which ensures that the test runner is
sound.

[cbmc-github]: https://github.com/diffblue/cbmc
