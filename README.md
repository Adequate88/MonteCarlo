## Project Overview

This project provides a modular framework for sampling, statistical analysis, and testing mathematical properties such as the Central Limit Theorem. It includes:

    Sampling distributions (e.g., uniform, normal, and Box-Muller transforms).
    Statistical computations such as expectation, variance, and moments.
    A Central Limit Theorem tester.

## How to Compile the Program

To compile the project, follow these steps:

    Prerequisites:
        A C++17-compatible compiler (e.g., GCC, Clang, or MSVC).
        Make sure CMake is installed (optional, depending on the build system used).

    Compilation:
        Navigate to the root directory of the project.
        Compile with the following command:

        g++ -std=c++17 -o main src/sampling/*.cc src/statistics/*.cc src/functions/*.cc src/clt_tester/clt_tester.cc

        If using CMake, create a CMakeLists.txt file for streamlined compilation (this can be added if requested).

Typical Program Execution (Flow and Usage)
Program Flow:

    The user selects a sampler from the sampling module (e.g., UniformSampler, NormalBoxMullerSampler).
    The sampler generates random samples for statistical analysis.
    Use the Statistics class to calculate metrics like expectation, variance, and moments.
    Optionally, test properties like the Central Limit Theorem using the clt_tester.

Usage Example:

#include "sampling/uniform_sampler.hh"
#include "statistics/statistics.hh"

int main() {
    // Create a uniform sampler
    UniformSampler uniformSampler(42);  // Seed value: 42

    // Generate statistics with 1000 samples
    Statistics stats(uniformSampler, 1000);

    // Compute expectation and variance
    auto expectation = stats.expectation(IdentityFunction<double>());
    auto variance = stats.variance(IdentityFunction<double>());

    std::cout << "Expectation: " << expectation << "\n";
    std::cout << "Variance: " << variance << "\n";

    return 0;
}

## List of Features and Tests
Features:

    Sampling Module:
        Uniform Distribution (UniformSampler).
        Normal Distribution:
            Box-Muller Transform (NormalBoxMullerSampler).
            Inverse Transform (NormalInverseSampler).

    Statistics Module:
        Compute expectation, variance, and arbitrary moments.
        Supports user-defined functions through the AbstractFunction interface.

    Central Limit Theorem Tester:
        Validates the CLT property for a given sampler and statistical function.

    Flexible Function Composition:
        Compose, add, and multiply functions dynamically.

Tests:

    Test cases for UniformSampler and NormalSampler (using predefined inputs).
    Validation of computed statistics using known theoretical values.
    Verification of the Central Limit Theorem.

## TODOs and Perspectives
TODOs:

    Add support for more sampling distributions (e.g., exponential, Poisson).
    Implement a plotting module for visualizing histograms.
    Write additional unit tests for edge cases in statistics computations.

Perspectives:

    Expand the library to support higher-dimensional sampling.
    Integrate with external libraries (e.g., GNUPlot or Matplotlib) for advanced analysis.
    Extend the function wrapper to accept more complex user-defined functions.




