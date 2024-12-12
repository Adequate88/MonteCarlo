//
// Created by Alessandro Salo on 22.11.2024.
//
/**
 * @file normal_inverse_sampler.hh
 * @brief Defines the NormalInverseSampler class.
 *
 * Implements a normal distribution sampler using the inverse transform method.
 */

#ifndef NORMAL_INVERSE_SAMPLER_HH
#define NORMAL_INVERSE_SAMPLER_HH

#include "normal_sampler.hh"

/**
 * @class NormalInverseSampler
 * @brief Generates samples from a normal distribution using the inverse transform method.
 *
 * Extends the NormalSampler class to provide an implementation of the inverse
 * transform method for generating normal-distributed random numbers.
 */
class NormalInverseSampler : public NormalSampler {

    public:
        /**
         * @brief Constructor for standard normal distribution.
         *
         * Initializes the sampler with mean 0 and standard deviation 1.
         * @param seed Seed for the random number generator.
         */
        NormalInverseSampler(int seed);

       /**
        * @brief Constructor for custom normal distribution.
        *
        * Initializes the sampler with specified mean and standard deviation.
        * @param seed Seed for the random number generator.
        * @param mu Mean of the distribution.
        * @param sigma Standard deviation of the distribution.
        */
        NormalInverseSampler(int seed, double mu, double sigma);

       /**
        * @brief Samples a random value from the normal distribution.
        *
        * Uses the inverse transform method to generate a normally distributed random value.
        * @return A random value from the normal distribution.
        */
        double sample() override;

       /**
        * @brief Computes the quantile function for the normal distribution.
        *
        * Calculates the quantile corresponding to a given probability using the
        * Beasley-Springer-Moro approximation.
        * @param p Probability in the range (0,1).
        * @return The quantile value corresponding to the probability.
        */
        double normalQuantile(double p);


};

#endif //NORMAL_INVERSE_SAMPLER_HH
