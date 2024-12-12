//
// Created by Alessandro Salo on 27.11.2024.
//
/**
 * @file normal_sampler.cc
 * @brief Implements methods for the NormalSampler class.
 *
 * Contains the implementation of histogram generation for normal distributions.
 */

#include "normal_sampler.hh"
#include <iostream>
#include <cmath>

/**
 * @brief Generates a histogram of the normal distribution.
 *
 * Populates the `distribution_array` with bin frequencies based on generated samples.
 * Ensures that bins have a minimum size and samples are allocated appropriately.
 *
 * @param bins Number of bins in the histogram.
 * @param n_samples Number of samples to generate.
 */
void NormalSampler::generateDistribution(int bins, const int n_samples) {

    if (bins < 3) {
        std::cout << "bins must be at least 3. Automatically setting to 3." << std::endl;
        bins = 3;
    }

    this->distribution_array.resize(bins, 0); // Initialize the array with 0s

    double minimum = mu - 5*sigma;
    double maximum = mu + 5*sigma;

    const double delta_x = (maximum - minimum) / static_cast<double>(bins - 2);

    for (int i = 0; i < n_samples; i++) {
        const double current_sample = sample() - minimum;
        int index;

        // Compute the index safely
        if (current_sample < 0) {
            index = 0;
        } else if (current_sample > maximum - minimum) {
            index = bins - 1;
        } else {
            index = static_cast<int>(std::floor(current_sample / delta_x) + 1);
        }

        // Increment the corresponding bin by the fractional value
        this->distribution_array[index] += 1.0 / (n_samples * delta_x);
    }

}

