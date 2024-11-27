//
// Created by Alessandro Salo on 22.11.2024.
//
#include <cmath>
#include "uniform_sampler.hh"

void UniformSampler::initialize(unsigned long long j) {
    u = j ^ v; int64();
    v = u; int64();
    w = v; int64();
}

UniformSampler::UniformSampler(unsigned long long j) : v(4101842887655102017LL), w(1), minimum(0.0), maximum(1.0) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
}

UniformSampler::UniformSampler(unsigned long long j, float a, float b) : v(4101842887655102017LL), w(1), minimum(a), maximum(b) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
}

unsigned long long UniformSampler::int64() {
    // Return 64-bit random integer. See text for explanation of method.
    u = u * 2862933555777941757LL + 7046029254386353087LL;
    v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
    w = 4294957665U*(w & 0xffffffff) + (w >> 32);
    unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
    return (x + v) ^ w;
}

double UniformSampler::sample() { return minimum + (maximum - minimum) * (5.42101086242752217E-20 * int64()); }
// Return random double-precision floating value in the range 0. to 1.

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

void UniformSampler::plotDistribution() {

    const int bins = distribution_array.size();

    std::vector<double> bin_centers(bins, 0.0); // Midpoints of the bins

    const double delta_x = (maximum - minimum) / static_cast<double>(bins);

    for (int i = 0; i < bins; i++) {
        bin_centers[i] = delta_x * static_cast<double>(i+1) * 0.5 + minimum;
    }

    // Combine bin centers and frequencies into a single vector
    std::vector<std::pair<double, double>> histogram_data;
    for (size_t i = 0; i < bins; ++i) {
        histogram_data.emplace_back(bin_centers[i], distribution_array[i]);
    }

    // Initialize Gnuplot
}
