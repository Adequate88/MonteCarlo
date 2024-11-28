//
// Created by alixb1908 on 28/11/24.
//

#ifndef CLT_TESTER_HH
#define CLT_TESTER_HH

#include "abstract_sampler.hh"
#include "statistics.hh"
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>

class CltTester {
public:
    // Constructor that takes in an abstract sampler and the number of samples N
    CltTester(AbstractSampler& sampler, int N, int num_iterations) : sampler_(sampler), N_(N), num_iterations_(num_iterations) {}

    // Method to test the Central Limit Theorem
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
    AbstractSampler& sampler_;
    int N_;
    int num_iterations_;
};

#endif // CLT_TESTER_HH
