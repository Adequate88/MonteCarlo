#include <iostream>
#include "sampling/uniform_sampler.hh"
#include "sampling/nonuniform_sampler.hh"
#include "sampling/normal_sampler.hh"
#include "statistics/statistics.hh"
#include "functions/abstract_function.h"

int main(int argc, char **argv) {
    // Create a UniformSampler instance
    UniformSampler sampler(20, -2, 5);

    // Create a Statistics instance with the sampler
    Statistics stats(sampler, 1000); // Generate 1000 samples

    // Test expectation, variance, and moments with identity function
    auto identity = IdentityFunction<double>();

    std::cout << "Expectation: " << stats.expectation(identity) << std::endl;
    std::cout << "Variance: " << stats.variance(identity) << std::endl;
    std::cout << "Third Moment: " << stats.moment(3, identity) << std::endl;
    std::cout << "Fourth Central Moment: " << stats.central_moment(4, identity) << std::endl;

    // Generate and add new samples
    stats.gen_N_new_samples(500);
    std::cout << "Updated Expectation after adding 500 more samples: " << stats.expectation(identity) << std::endl;

    return 0;
}
