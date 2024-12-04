//
// Created by Alessandro Salo on 23.11.2024.
//
/**
 * @file abstract_sampler.hh
 * @brief Defines the AbstractSampler class for sampling distributions.
 *
 * This file contains the abstract base class `AbstractSampler`, which provides
 * an interface for generating and sampling from distributions.
 */
#ifndef ABSTRACT_SAMPLER_HH
#define ABSTRACT_SAMPLER_HH

#include <vector>
/**
 * @class AbstractSampler
 * @brief Abstract base class for sampling and distribution generation.
 *
 * The `AbstractSampler` class provides a virtual interface for generating
 * distributions and sampling values. Derived classes should override
 * the virtual methods to implement specific sampling logic.
 */
class AbstractSampler {
    /**
    */
    protected:
        /**
         * @brief Virtual destructor for safe polymorphic behavior.
         */
        ~AbstractSampler() = default;

        /**
        * @brief Array to store distribution data based on bins.
        */
        std::vector<double> distribution_array; // Array to store distribution based on bins
    public:
        /**
        * @brief Generates a random sample.
        *
        * This method should be overridden by derived classes to implement
        * specific sampling logic.
        *
        * @return A random sample as a double-precision floating-point number.
        */
        virtual double sample(){return 0.0;}// Return random double-precision sample


         /**
         * @brief Generates a distribution based on bins and sample count.
         *
         * This method should be overridden by derived classes to populate
         * the `distribution_array`.
         *
         * @param bins Number of bins in the distribution.
         * @param n_samples Number of samples to generate.
         */
        virtual void generateDistribution(int bins, int n_samples) {}

        /**
        * @brief Prints the distribution to the standard output.
        *
        * This method iterates over the `distribution_array` and prints
        * each bin's data.
        */
        void printDistribution();
};

#endif //ABSTRACT_SAMPLER_HH
