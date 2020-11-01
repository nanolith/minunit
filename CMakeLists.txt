PROJECT(minunit)

cmake_minimum_required(VERSION 3.10)

INCLUDE(CheckSymbolExists)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

option(FORKED_TEST_RUNNER_SELECTED "Use a forked test runner." ON)
option(MODELCHECK_ENABLED "Enable Model Checking")

if (MODELCHECK_ENABLED)
    find_package(modelcheck REQUIRED)
    set(HAS_MODELCHECK)
endif()

INCLUDE_DIRECTORIES(include)
AUX_SOURCE_DIRECTORY(src MINUNIT_SOURCES)

ADD_LIBRARY(minunit STATIC ${MINUNIT_SOURCES})
TARGET_COMPILE_OPTIONS(minunit
                       PRIVATE ${MODELCHECK_CFLAGS}
                               "-I${CMAKE_BINARY_DIR}")

#detect various platform options
check_symbol_exists(dup2 "unistd.h" HAS_DUP2)
check_symbol_exists(fork "unistd.h" HAS_FORK)
check_symbol_exists(isatty "unistd.h" HAS_ISATTY)
check_symbol_exists(signal "signal.h" HAS_SIGNAL)
check_symbol_exists(socketpair "sys/socket.h" HAS_SOCKETPAIR)
check_symbol_exists(waitpid "sys/wait.h" HAS_WAITPID)

#Build config.h
configure_file(config.h.cmake config.h)

#Build a pkg-config file
SET(MINUNIT_PC "${CMAKE_BINARY_DIR}/minunit.pc")
FILE(WRITE  ${MINUNIT_PC} "Name: minunit")
FILE(APPEND ${MINUNIT_PC} "\nDescription: minunit unit testing suite")
FILE(APPEND ${MINUNIT_PC} "\nVersion: 0.2.2")
FILE(APPEND ${MINUNIT_PC} "\nURL: https://github.com/nanolith/minunit")
FILE(APPEND ${MINUNIT_PC} "\nprefix=${CMAKE_INSTALL_PREFIX}")
FILE(APPEND ${MINUNIT_PC} "\nlibdir=\${prefix}/lib")
FILE(APPEND ${MINUNIT_PC} "\nincludedir=\${prefix}/include")
FILE(APPEND ${MINUNIT_PC} "\nLibs: -L\${libdir} -lminunit")
FILE(APPEND ${MINUNIT_PC} "\nCflags: -I\${includedir}")
INSTALL(FILES ${MINUNIT_PC} DESTINATION lib/pkgconfig)

#Install headers
FILE(GLOB MINUNIT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/minunit/*.h")
INSTALL(FILES ${MINUNIT_INCLUDES} DESTINATION include/minunit)

#Install library
INSTALL(TARGETS minunit
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#Examples
ADD_SUBDIRECTORY(examples)

if (MODELCHECK_ENABLED)
    #Model checks
    ADD_SUBDIRECTORY(model-check)
endif()
