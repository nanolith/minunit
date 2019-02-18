function(check_target)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES FEATURES)
    cmake_parse_arguments(
        CHECK_TARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
                        
    ADD_CUSTOM_TARGET(
        ${CHECK_TARGET_NAME}
        COMMAND cbmc --bounds-check --pointer-check --memory-leak-check
                     --div-by-zero-check --signed-overflow-check
                     --unsigned-overflow-check --pointer-overflow-check
                     --conversion-check --trace --stop-on-fail -DMODEL_CHECK
                     --object-bits 16 --drop-unused-functions --unwind 10
                     --full-slice
                     -I ../include
                     ${CHECK_TARGET_FEATURES}
                     ${MODELCHECK_CFLAGS}
                     ${MODELCHECK_ALLOCATOR_SOURCES}
                     ${CHECK_TARGET_SOURCES}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    ADD_DEPENDENCIES(run_tests ${CHECK_TARGET_NAME})
endfunction(check_target)

#check list.
check_target(
    NAME check_list SOURCES "../src/minunit_list.c" "check_list.c"
    FEATURES "--unwindset"
    "minunit_prop_list_node_valid_helper.0:3,minunit_list_reverse.0:3,main.0:3")
