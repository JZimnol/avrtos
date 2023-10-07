set(CMAKE_BUILD_TYPE Debug)

# Unity
add_library(Unity STATIC
            deps/Unity/src/unity.c)
target_include_directories(Unity PUBLIC
                           deps/Unity/src)

# avrtos architecture-independent files to test
add_library(avrtos_arch_ind STATIC
            ${CMAKE_SOURCE_DIR}/src/circular_buffer_arch_ind.c
            ${CMAKE_SOURCE_DIR}/src/linked_list_arch_ind.c
            ${CMAKE_SOURCE_DIR}/src/logger_arch_ind.c)
target_include_directories(avrtos_arch_ind PUBLIC
                           ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(avrtos_arch_ind PUBLIC
                           AVRTOS_UNIT_TEST
                           AVRTOS_UNIT_TEST_SINGLE_LOG_MAX_SIZE=35)

# suites
function(avrtos_unit_test_add SuiteName)
    add_executable(${SuiteName} ${ARGN})
    target_link_libraries(${SuiteName}
                          Unity
                          avrtos_arch_ind)
    add_test(NAME ${SuiteName}
             COMMAND ${SuiteName})
endfunction()

# prepare tests
file(GLOB_RECURSE TEST_SUITE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/suites "suites/*_unit_test.c")
set(TEST_SUITE_LIST "")
foreach(SuiteFile ${TEST_SUITE_FILES})
    string(REGEX REPLACE "\.c$" "" SUITE_NAME ${SuiteFile})
    list(APPEND TEST_SUITE_LIST ${SUITE_NAME})
    avrtos_unit_test_add(${SUITE_NAME} suites/${SuiteFile})
endforeach()

message("Test suites: ${TEST_SUITE_LIST}")
