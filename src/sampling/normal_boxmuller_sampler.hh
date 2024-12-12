//
// Created by Alessandro Salo on 22.11.2024.
//

/**
 * @file normal_boxmuller_sampler.hh
 * @brief Defines the NormalBoxMullerSampler class.
 *
 * Implements a normal distribution sampler using the Box-Muller method.
 */

#ifndef NORMAL_BOXMULLER_SAMPLER_HH
#define NORMAL_BOXMULLER_SAMPLER_HH

#include "normal_sampler.hh"

/**
 * @class NormalBoxMullerSampler
 * @brief Generates samples from a normal distribution using the Box-Muller method.
 *
 * Extends the NormalSampler class to provide an implementation of the
 * Box-Muller transform for generating normal-distributed random numbers.
 */
class NormalBoxMullerSampler : public NormalSampler {

    private:
        double last_muller_; ///< Stores the secondary value from Box-Muller computation.
        bool use_last_muller_; ///< Indicates whether the stored value can be reused.
        const double two_pi_; ///< Constant value of 2π.

    public:
        /**
         * @brief Constructor for standard normal distribution.
         *
         * Initializes the sampler with mean 0 and standard deviation 1.
         * @param seed Seed for the random number generator.
         */
        NormalBoxMullerSampler(int seed);

        /**
         * @brief Constructor for custom normal distribution.
         *
         * Initializes the sampler with specified mean and standard deviation.
         * @param seed Seed for the random number generator.
         * @param mu Mean of the distribution.
         * @param sigma Standard deviation of the distribution.
         */
        NormalBoxMullerSampler(int seed, double mu, double sigma);

       /**
         * @brief Samples a random value from the normal distribution.
         *
         * Uses the Box-Muller transform to generate a normally distributed random value.
         * @return A random value from the normal distribution.
         */
        double sample() override;
};

#endif //NORMAL_BOXMULLER_SAMPLER_HH
