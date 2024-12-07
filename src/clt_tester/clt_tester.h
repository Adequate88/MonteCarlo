//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file clt_tester.h
 * @brief Defines the CltTester class, which tests the Central Limit Theorem (CLT) using sample data.
 *
 * This file contains the definition of the CltTester class. The CltTester class is used to
 * demonstrate and validate the Central Limit Theorem by generating multiple samples and
 * calculating the mean and standard deviation of those sample means.
 */
#ifndef CLT_TESTER_HH
#define CLT_TESTER_HH

#include "abstract_sampler.hh"
#include "statistics.hh"
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>
/**
 * @brief Class that tests the Central Limit Theorem using an abstract sampler.
 *
 * This class takes in an abstract sampler and generates multiple sets of samples, calculates their
 * means, and then checks if the mean and standard deviation of these sample means conform to the
 * expected properties defined by the Central Limit Theorem.
 */
class CltTester {
public:
    /**
     * @brief Constructs a CltTester object.
     *
     * @param sampler Reference to an AbstractSampler used to generate random samples.
     * @param N Number of samples per iteration.
     * @param num_iterations Number of iterations to perform.
     */
    CltTester(AbstractSampler& sampler, int N, int num_iterations) : sampler_(sampler), N_(N), num_iterations_(num_iterations) {}

    /**
     * @brief Tests the Central Limit Theorem.
     *
     * This method generates multiple sets of `N` samples, calculates their means, and computes
     * the mean and standard deviation of those sample means. The results are then printed
     * to the console, along with the expected standard deviation as per the CLT.
     */
    void test() const {
        std::vector<double> sample_means;

        // Take multiple sets of N samples and calculate their means
        for (int i = 0; i < num_iterations_; ++i) {
            Statistics stats(sampler_,N_);
            double sample_mean = stats.expectation(IdentityFunction<double>());
            sample_means.push_back(sample_mean);
        }

        // Use the Statistics class to calculate the mean and standard deviation of the sample means
        Statistics mean_stats(sampler_,0);
        mean_stats.set_data(sample_means);
        double mean_of_means = mean_stats.expectation(IdentityFunction<double>());
        double standard_deviation_of_means = std::sqrt(mean_stats.variance(IdentityFunction<double>()));

        // Output the results
        std::cout << "Mean of Sample Means: " << mean_of_means << std::endl;
        std::cout << "Mean of Original Distribution: " << 0 << std::endl;
        std::cout << "Standard Deviation of Sample Means: " << standard_deviation_of_means << std::endl;
        std::cout << "According to CLT, the Standard Deviation should be approximately: " << 1.0 / std::sqrt(N_) << std::endl;
    }

private:
    AbstractSampler& sampler_; ///< Reference to the sampler used to generate the samples.
    int N_; ///< Number of samples per iteration.
    int num_iterations_; ///< Number of iterations to perform.
};

#endif // CLT_TESTER_HH
