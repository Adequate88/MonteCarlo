//
// Created by Alessandro Salo on 22.11.2024.
//
/**
 * @file normal_inverse_sampler.cc
 * @brief Implements the NormalInverseSampler class.
 *
 * Contains the implementation of the inverse transform method for generating
 * normal-distributed random samples.
 */

#include "normal_inverse_sampler.hh"
#include <cmath>
#include <iostream>

/**
 * @brief Constructor for standard normal distribution.
 *
 * Initializes the sampler with mean 0 and standard deviation 1.
 * @param seed Seed for the random number generator.
 */
NormalInverseSampler::NormalInverseSampler(int seed)
    : NormalSampler(seed, 0.0, 1.0) {}

/**
 * @brief Constructor for custom normal distribution.
 *
 * Initializes the sampler with specified mean and standard deviation.
 * @param seed Seed for the random number generator.
 * @param mu Mean of the distribution.
 * @param sigma Standard deviation of the distribution.
 */
NormalInverseSampler::NormalInverseSampler(int seed, double mu, double sigma)
    : NormalSampler(seed, mu, sigma) {}

/**
 * @brief Computes the quantile function for the normal distribution.
 *
 * Approximates the quantile (inverse cumulative distribution function) for a normal
 * distribution using the Beasley-Springer-Moro approximation.
 *
 * @param p Probability in the range (0,1).
 * @return The quantile value corresponding to the probability.
 * @throws std::invalid_argument if p is not in the range (0,1).
 */
double NormalInverseSampler::normalQuantile(double p) {
    if (p <= 0.0 || p >= 1.0) {
        throw std::invalid_argument("p must be in the range (0, 1)");
    }

    static const double a[] = {
        2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637
    };
    static const double b[] = {
        -8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833
    };
    static const double c[] = {
        0.3374754822726147, 0.9761690190917186, 0.1607979714918209,
        0.0276438810333863, 0.0038405729373609, 0.0003951896511919,
        0.0000321767881768, 0.0000002888167364, 0.0000003960315187
    };

    double x = p - 0.5;
    double r;

    if (std::abs(x) < 0.42) {
        // Beasley-Springer approximation
        r = x * x;
        return x * (((a[3] * r + a[2]) * r + a[1]) * r + a[0]) /
               ((((b[3] * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    } else {
        // Moro's approximation
        r = (x < 0.0) ? p : 1.0 - p;
        r = std::log(-std::log(r));
        double z = c[0];
        for (int i = 1; i < 9; ++i) {
            z += c[i] * std::pow(r, i);
        }
        return (x < 0.0) ? -z : z;
    }
}

/**
 * @brief Samples a random value from the normal distribution.
 *
 * Uses the quantile function and a uniformly distributed random sample
 * to generate a value from the normal distribution.
 * @return A random value from the normal distribution.
 */
double NormalInverseSampler::sample(){
    double u1 = uniform_sampler_.sample();
    return mu_ + sigma_*normalQuantile(u1);
}

