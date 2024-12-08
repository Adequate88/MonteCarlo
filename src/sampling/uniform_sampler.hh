//
// Created by Alessandro Salo on 22.11.2024.
//
/**
 * @file uniform_sampler.hh
 * @brief Defines the UniformSampler class.
 *
 * Provides methods for generating random samples from a uniform distribution.
 */

#ifndef UNIFORM_SAMPLER_HH
#define UNIFORM_SAMPLER_HH

#include "abstract_sampler.hh"

/**
 * @class UniformSampler
 * @brief Generates random samples from a uniform distribution.
 *
 * Extends the AbstractSampler class to provide a uniform distribution sampler.
 */
class UniformSampler : public AbstractSampler {
    /** Implementation of the highest quality recommended generator. The constructor is called with
    an integer seed and creates an instance of the generator. The member functions int64, doub,
    and int32 return the next values in the random sequence, as a variable type indicated by their
    names. The period of the generator is  3:138  1057.
    */
    private:
        unsigned long long u{},v,w; ///< Internal state variables for random number generation.
        double minimum, maximum; ///< Range of the uniform distribution.
        void initialize(unsigned long long j); ///< Initializes the state variables

        /**
        * @brief Generates a random 64-bit integer.
        *
        * Implements a high-quality random number generator.
        * @return A 64-bit random integer.
        */
        unsigned long long int64();

    public:
        /**
         * @brief Destructor for UniformSampler.
         */
        virtual ~UniformSampler() = default;

        /**
         * @brief Constructor for default uniform distribution.
         *
         * Initializes the sampler with a range of [0,1].
         * @param j Seed for the random number generator.
         */
        explicit UniformSampler(unsigned long long j);

       /**
        * @brief Constructor for custom uniform distribution.
        *
        * Initializes the sampler with a specified range.
        * @param j Seed for the random number generator.
        * @param a Lower bound of the range.
        * @param b Upper bound of the range.
        */
        UniformSampler(unsigned long long j, float a, float b);

       /**
        * @brief Generates a random sample from the uniform distribution.
        *
        * @return A random double-precision value within the specified range.
        */
        double sample() override;

        /**
         * @brief Generates a histogram of the uniform distribution.
         *
         * Populates the distribution array with samples.
         * @param bins Number of bins for the histogram.
         * @param n_samples Number of samples to generate.
         */
        void generateDistribution(int bins, int n_samples) override;

};
#endif //UNIFORM_SAMPLER_HH
