#include <iostream>
#include <cmath>

#include "sampling.hh"
#include "statistics/statistics.hh"
#include "functions/abstract_function.h"

int main(int argc, char **argv) {
    // Create a UniformSampler instance
    NormalBoxMullerSampler sampler(42, 3, 2);
    double mu = 3.0;
    double sigma = 2.0;
    // Create a Statistics instance with the sampler
    Statistics stats(sampler, 100000); // Generate 1000 samples

    // Test expectation, variance, and moments with identity function
    auto identity = IdentityFunction<double>();

    // Generate and add new samples
    std::cout << "Expectation: " << stats.expectation(identity) << std::endl;
    std::cout << "Variance: " << stats.variance(identity) << std::endl;
    std::cout << "Third Moment: " << stats.moment(3, identity) << std::endl;
    std::cout << "Fourth Central Moment: " << stats.central_moment(3, identity) << std::endl;


    double expected_4th = std::pow(mu, 4) + 3*std::pow(sigma, 4) + 6*std::pow(epsilon*mu, 2);

    return 0;
}
