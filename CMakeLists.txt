cmake_minimum_required(VERSION 3.14)

project(avrtos)

# for now, no proper build system using CMakeLists has been created

if(WITH_AVRTOS_TEST)
    include(CTest)

    add_custom_target(check COMMAND make && ${CMAKE_CTEST_COMMAND} || make check_failed)
    add_custom_target(check_verbose COMMAND make && ${CMAKE_CTEST_COMMAND} --verbose)

    add_subdirectory(tests)
endif()
