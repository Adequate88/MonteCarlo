//
// Created by Alessandro Salo on 22.11.2024.
//
/**
 * @file normal_sampler.hh
 * @brief Defines the NormalSampler class.
 *
 * Provides methods for generating random samples from a normal distribution.
 */

#ifndef NORMAL_SAMPLER
#define NORMAL_SAMPLER

#include "uniform_sampler.hh"

/**
 * @class NormalSampler
 * @brief Abstract class for normal distribution sampling.
 *
 * Implements methods for generating and sampling from normal distributions.
 */
class NormalSampler : public AbstractSampler{

  public:
    /**
     * @brief Constructor for NormalSampler.
     *
     * Initializes the normal distribution sampler with a specified seed, mean, and standard deviation.
     * @param seed Seed for the random number generator.
     * @param mu Mean of the normal distribution.
     * @param sigma Standard deviation of the normal distribution.
     */
    NormalSampler(int seed, double mu, double sigma) : seed(seed), uniform_sampler(seed), mu(mu), sigma(sigma) {

        plot_minimum = mu - 5*sigma;
        plot_maximum = mu + 5*sigma;

    };

    /**
     * @brief Generates a histogram of the normal distribution.
     *
     * Populates the `distribution_array` with bins and corresponding frequencies.
     * @param bins Number of bins in the histogram.
     * @param n_samples Number of samples to generate.
     */
    void generateDistribution(int bins, int n_samples) override;

protected:
    int seed; ///< Seed for the random number generator.
    UniformSampler uniform_sampler; ///< Uniform sampler used internally.
    double mu; ///< Mean of the normal distribution.
    double sigma; ///< Standard deviation of the normal distribution.

};

#endif //NORMAL_SAMPLER
