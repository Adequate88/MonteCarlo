cmake_minimum_required(VERSION 3.0)
project(PCSC-project)

set(CMAKE_CXX_STANDARD 17)

# Include directories for header files
include_directories(src/statistics src/sampling)

# Add all source files in the sampling directory
file(GLOB SAMPLING_SRC "src/sampling/*.cc")
# Add all source files in the statistics directory
file(GLOB STATISTICS_SRC "src/statistics/*.cc")

# Create a library from all source files in the src directory, including sampling and statistics
add_library(pcsc src/pcsc.cc ${SAMPLING_SRC} ${STATISTICS_SRC})

# Add all executables in the src directory (like main.cc) to the project
file(GLOB EXECUTABLES "src/*.cc")
add_executable(main ${EXECUTABLES})
target_link_libraries(main pcsc)

# Add test executable
add_executable(test_pcsc test/test.cc)
target_link_libraries(test_pcsc gtest_main gtest pthread pcsc)

# Define a custom target for running tests
add_custom_target(test ./test_pcsc DEPENDS test_pcsc)
