cmake_minimum_required(VERSION 3.5)
project(PCSC-project)

set(CMAKE_CXX_STANDARD 17)

# Include directories for header files
include_directories(src/statistics src/sampling src/functions src/clt_tester)

# Add all source files in the sampling directory
file(GLOB SAMPLING_SRC "src/sampling/*.cc")
# Add all source files in the statistics directory
file(GLOB STATISTICS_SRC "src/statistics/*.cc")
# Add all source files in the functions directory
file(GLOB FUNCTIONS_SRC "src/functions/*.cc")


# Add all executables in the src directory (like main.cc) to the project
file(GLOB EXECUTABLES "src/*.cc")
add_executable(main ${EXECUTABLES} ${SAMPLING_SRC} ${STATISTICS_SRC} ${FUNCTIONS_SRC}
        src/clt_tester/clt_tester.cpp
        src/clt_tester/clt_tester.h)

#Adding Googletest
add_subdirectory(googletest)

# Add test executable
file(GLOB TEST_EXECUTABLES "test/*.cc")
add_executable(test_runner ${TEST_EXECUTABLES})
target_link_libraries(test_runner gtest_main gtest pthread ${SAMPLING_SRC} ${STATISTICS_SRC} )

# Define a custom target for running tests
add_custom_target(run_tests COMMAND ./test_runner DEPENDS test_runner)

