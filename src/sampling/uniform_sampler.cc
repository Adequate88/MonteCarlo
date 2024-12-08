//
// Created by Alessandro Salo on 22.11.2024.
//
/**
 * @file uniform_sampler.cc
 * @brief Implements methods for the UniformSampler class.
 *
 * Contains functionality for uniform distribution sampling, including initialization
 * and histogram generation.
 */

#include <cmath>
#include "uniform_sampler.hh"
#include <gnuplot-iostream.h>

/**
 * @brief Initializes the random number generator state.
 *
 * Sets up the internal state variables for the random number generator.
 *
 * @param j Seed for the random number generator.
 */
void UniformSampler::initialize(unsigned long long j) {
    u = j ^ v; int64();
    v = u; int64();
    w = v; int64();
}

/**
 * @brief Constructor for default uniform distribution.
 *
 * Initializes the sampler with a range of [0,1].
 *
 * @param j Seed for the random number generator.
 */
UniformSampler::UniformSampler(unsigned long long j) : v(4101842887655102017LL), w(1), minimum(0.0), maximum(1.0) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
    plot_maximum = maximum;
    plot_minimum = minimum;
}

/**
 * @brief Constructor for custom uniform distribution.
 *
 * Initializes the sampler with a specified range [a, b].
 *
 * @param j Seed for the random number generator.
 * @param a Lower bound of the distribution range.
 * @param b Upper bound of the distribution range.
 */
UniformSampler::UniformSampler(unsigned long long j, float a, float b) : v(4101842887655102017LL), w(1), minimum(a), maximum(b) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
    plot_maximum = maximum;
    plot_minimum = minimum;
}

/**
 * @brief Generates a random 64-bit integer.
 *
 * Implements a high-quality random number generator with a large period.
 *
 * @return A 64-bit random integer.
 */
unsigned long long UniformSampler::int64() {
    // Return 64-bit random integer. See text for explanation of method.
    u = u * 2862933555777941757LL + 7046029254386353087LL;
    v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
    w = 4294957665U*(w & 0xffffffff) + (w >> 32);
    unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
    return (x + v) ^ w;
}

/**
 * @brief Generates a random sample from the uniform distribution.
 *
 * @return A double-precision value within the range [minimum, maximum].
 */
double UniformSampler::sample() { return minimum + (maximum - minimum) * (5.42101086242752217E-20 * int64()); }

/**
 * @brief Generates a histogram of the uniform distribution.
 *
 * Populates the `distribution_array` with bin frequencies.
 *
 * @param bins Number of bins for the histogram.
 * @param n_samples Number of samples to generate.
 */
void UniformSampler::generateDistribution(const int bins, const int n_samples) {
    this->distribution_array.resize(bins, 0); // Initialize the array with 0s

    const double delta_x = (maximum - minimum) / static_cast<double>(bins);

    for (int i = 0; i < n_samples; i++) {
        const double current_sample = sample() - minimum;

        // Compute the index safely
        const int index = std::min(static_cast<int>(std::floor(current_sample / delta_x)), bins - 1);

        // Increment the corresponding bin by the fractional value
        this->distribution_array[index] += 1.0 / (n_samples * delta_x);
    }
}
