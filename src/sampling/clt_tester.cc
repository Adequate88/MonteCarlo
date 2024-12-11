//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file clt_tester.cc
 * @brief Initializes functions for CLT Tester
 */

#include "clt_tester.hh"
#include "statistics.hh"
#include "gnuplot-iostream.h"
#include "primitive_functions.hh"
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>

/**
 * @brief Constructs a CltTester object.
 *
 * @param sampler Reference to an AbstractSampler used to generate random samples.
 * @param N Number of samples per iteration.
 */
CltTester::CltTester(AbstractSampler& sampler,AbstractFunction<double,double>& f, int N)
    : sampler_(sampler),f_(f), N_(N) {
    std::cout<< "CltTester object created" << std::endl;
}

void CltTester::generateDistribution(int bins, int n_samples){
    // Take multiple sets of N samples and calculate their means
    for (int i = 0; i < n_samples; ++i) {
        Statistics stats(sampler_, N_);
        double sample_mean = stats.expectation(f_);
        sample_means_.push_back(sample_mean);
    }
    plot_minimum = *std::min_element(sample_means_.begin(), sample_means_.end());
    plot_maximum = *std::max_element(sample_means_.begin(), sample_means_.end());
    if (bins < 3) {
        std::cout << "bins must be at least 3. Automatically setting to 3." << std::endl;
        bins = 3;
    }
    this->distribution_array.resize(bins, 0); // Initialize the array with 0s
    //split the sample_means into bins and calculate the frequency of each bin, store it in distribution_array
    double minimum = plot_minimum;
    double maximum = plot_maximum;
    const double delta_x = (maximum - minimum) / static_cast<double>(bins);
    for (int i = 0; i < n_samples; i++) {
        const double current_sample = sample_means_[i] - minimum;
        int index;
        index = static_cast<int>(std::floor(current_sample / delta_x) + 1);
        // Increment the corresponding bin by the fractional value
        this->distribution_array[index] += 1.0 / (n_samples * delta_x);
    }
  }

/**
 * @brief Tests the Central Limit Theorem.
 *
 * This method generates multiple sets of `N` samples, calculates their means, and computes
 * the mean and standard deviation of those sample means. The results are then printed
 * to the console, along with the expected standard deviation as per the CLT.
 */
void CltTester::test() const {
    // Plot the distribution as a histogram
    std::cout<< "Plotting the distribution as a histogram" << std::endl;
    this -> plotDistribution();

    // Use the Statistics class to calculate the mean and standard deviation of the sample means
    // Statistics mean_stats(sampler_, 0);
    // mean_stats.set_data(sample_means);
    // double mean_of_means = mean_stats.expectation(IdentityFunction<double>());
    // double standard_deviation_of_means = std::sqrt(mean_stats.variance(IdentityFunction<double>()));
    //
    // // Output the results
    // std::cout << "Mean of Sample Means: " << mean_of_means << std::endl;
    // std::cout << "Mean of Original Distribution: " << 0 << std::endl;
    // std::cout << "Standard Deviation of Sample Means: " << standard_deviation_of_means << std::endl;
    // std::cout << "According to CLT, the Standard Deviation should be approximately: " << 1.0 / std::sqrt(N_) << std::endl;
}
