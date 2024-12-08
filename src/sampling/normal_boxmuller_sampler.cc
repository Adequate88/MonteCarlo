//
// Created by Alessandro Salo on 22.11.2024.
//

/**
 * @file normal_boxmuller_sampler.cc
 * @brief Implements the NormalBoxMullerSampler class.
 *
 * Contains the implementation of the Box-Muller transform for generating
 * normal-distributed random samples.
 */
/**
 * @file normal_boxmuller_sampler.cc
 * @brief Implements the NormalBoxMullerSampler class.
 *
 * Contains the implementation of the Box-Muller transform for generating
 * normal-distributed random samples.
 */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "normal_boxmuller_sampler.hh"
#include <cmath>
#include <iostream>


/**
 * @brief Constructor for standard normal distribution.
 *
 * Initializes the sampler with mean 0 and standard deviation 1.
 * @param seed Seed for the random number generator.
 */
NormalBoxMullerSampler::NormalBoxMullerSampler(int seed)
    : NormalSampler(seed, 0.0, 1.0),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

/**
 * @brief Constructor for custom normal distribution.
 *
 * Initializes the sampler with specified mean and standard deviation.
 * @param seed Seed for the random number generator.
 * @param mu Mean of the distribution.
 * @param sigma Standard deviation of the distribution.
 */
NormalBoxMullerSampler::NormalBoxMullerSampler(int seed, double mu, double sigma)
    : NormalSampler(seed, mu, sigma),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

/**
 * @brief Samples a random value from the normal distribution.
 *
 * Uses the Box-Muller transform to generate a normally distributed random value.
 * @return A random value from the normal distribution.
 */
double NormalBoxMullerSampler::sample(){

    if(use_last_muller){
        use_last_muller = false;
        return last_muller;
    }

    double u1,u2;
    do
    {
        u1 = uniform_sampler.sample();
    }
    while (u1 == 0.0);
    u2 = uniform_sampler.sample();

    double mag = sigma * sqrt(-2.0 * log(u1));
    double z0  = mag * cos(two_pi * u2) + mu;
    double z1  = mag * sin(two_pi * u2) + mu;

    last_muller = z1;
    use_last_muller = true;

    return z0;
}

