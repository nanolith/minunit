PROJECT(minunit)

cmake_minimum_required(VERSION 3.10)

INCLUDE_DIRECTORIES(include)
AUX_SOURCE_DIRECTORY(src MINUNIT_SOURCES)

ADD_LIBRARY(minunit STATIC ${MINUNIT_SOURCES})

#Build a pkg-config file
SET(MINUNIT_PC "${CMAKE_BINARY_DIR}/minunit.pc")
FILE(WRITE  ${MINUNIT_PC} "Name: minunit")
FILE(APPEND ${MINUNIT_PC} "\nDescription: minunit unit testing suite")
FILE(APPEND ${MINUNIT_PC} "\nVersion: 0.1.0")
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
