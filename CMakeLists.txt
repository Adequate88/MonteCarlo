cmake_minimum_required(VERSION 3.5)
project(PCSC-project)

set(CMAKE_CXX_STANDARD 17)

# find dependencies (Boost)

# adds the search path for includes
include_directories(gnuplot-iostream)

# Include directories for header files
include_directories(src/statistics src/sampling src/functions src/clt_tester src/parser)

# Add all source files in the sampling directory
file(GLOB SAMPLING_SRC "src/sampling/*.cc")
# Add all source files in the statistics directory
file(GLOB STATISTICS_SRC "src/statistics/*.cc")
# Add all source files in the functions directory
file(GLOB FUNCTIONS_SRC "src/functions/*.cc")
# Add all source files in the clt_tester directory
file(GLOB CLT_SRC "src/clt_tester/*.cc")
# Add all source files in the parser directory
file(GLOB PARSER_SRC "src/parser/*.cc")

# Copy the config.txt file to the build directory
configure_file(config.txt ${CMAKE_BINARY_DIR}/config.txt COPYONLY)

# Specify the path to the source directory (where config.txt is located)
set(OUTPUT_PATH "${CMAKE_SOURCE_DIR}")  # Output path is the source directory
add_definitions(-DOUTPUT_PATH="${OUTPUT_PATH}")

# Add all executables in the src directory (like main.cc) to the project
file(GLOB EXECUTABLES "src/*.cc")
add_executable(main ${EXECUTABLES} ${SAMPLING_SRC} ${STATISTICS_SRC} ${FUNCTIONS_SRC} ${CLT_SRC} ${PARSER_SRC})

#Adding Googletest
add_subdirectory(googletest)

# Add test executable
file(GLOB TEST_EXECUTABLES "test/*.cc")
add_executable(test_runner ${TEST_EXECUTABLES})
target_link_libraries(test_runner gtest_main gtest pthread ${SAMPLING_SRC} ${STATISTICS_SRC} )

add_custom_target(run_tests COMMAND ./test_runner DEPENDS test_runner)


# Add Boost libraries and pthread to the main executable


find_package(Boost REQUIRED COMPONENTS
        iostreams system filesystem
)

target_link_libraries(main PRIVATE
        Boost::iostreams
        Boost::system
        Boost::filesystem
        pthread
)


