//
// Created by alixb1908 on 26/11/24.
//
/**
 * @file statistics.hh
 * @brief Defines the Statistics class for statistical analysis of samples.
 *
 * This file contains the declaration of the Statistics class, which provides methods
 * to compute various statistical properties such as expectation, variance, and moments.
 */
#ifndef STATISTICS_HH
#define STATISTICS_HH

#include <vector>
#include <functional>
#include "abstract_sampler.hh"
#include "abstract_function.hh"

/**
 * @class Statistics
 * @brief Computes statistical metrics for a set of samples.
 *
 * The Statistics class provides methods to compute statistical properties like expectation,
 * variance, and moments for an arbitrary function applied to the sample data.
 */
class Statistics {
public:
    /**
     * @brief Constructs a Statistics object with an initial set of samples.
     *
     * Initializes the Statistics object by sampling `num_samples` from the provided sampler.
     * @param sampler Reference to an AbstractSampler object used to generate the samples.
     * @param num_samples Number of samples to initially generate.
     */
    Statistics(AbstractSampler& sampler, int num_samples);

    /**
     * @brief Standard destructor.
     */
    ~Statistics() = default;

    /**
     * @brief Sets the sample data.
     *
     * Replaces the current sample data with the provided vector of samples.
     * @param samples Vector of sample values.
     */
    void set_data(const std::vector<double>& samples);

    /**
     * @brief Generates and adds new samples to the dataset.
     *
     * Generates `N` new samples using the associated sampler and appends them to the existing data.
     * @param N Number of new samples to generate.
     */
    void gen_N_new_samples(int N);

    /**
     * @brief Computes the expectation value of an arbitrary function.
     *
     * Calculates the mean value of the function applied to the samples.
     * @param f Reference to an AbstractFunction object representing the function to evaluate.
     * @return The expectation value.
     */
    [[nodiscard]] double expectation(const AbstractFunction<double,double>& f) const;

    /**
     * @brief Computes the variance of an arbitrary function.
     *
     * Calculates the variance of the function applied to the samples.
     * @param f Reference to an AbstractFunction object representing the function to evaluate.
     * @return The variance.
     */
    [[nodiscard]] double variance(const AbstractFunction<double,double>& f) const;

     /**
      * @brief Computes a power moment of an arbitrary function.
      *
      * Calculates the `power`-th moment of the function applied to the samples.
      * @param power The order of the moment to compute.
      * @param f Reference to an AbstractFunction object representing the function to evaluate.
      * @return The computed moment.
      */
    [[nodiscard]] double moment(int power, const AbstractFunction<double,double>& f) const;

    /**
    * @brief Computes a central moment of an arbitrary function.
    *
    * Calculates the central `power`-th moment of the function applied to the samples.
    * @param power The order of the central moment to compute.
    * @param f Reference to an AbstractFunction object representing the function to evaluate.
    * @return The computed central moment.
    */
    [[nodiscard]] double central_moment(int power, const AbstractFunction<double,double>& f) const;

    /**
    * @brief Clears all sample data.
    *
    * Resets the sample dataset and the sample count to zero.
    */
    void clear_data();

private:
    AbstractSampler& sampler;  ///< Reference to the sampler used to generate samples.
    std::vector<double> data; ///< Container for the sample data.
    unsigned int N; ///< Number of samples in the dataset.
};

#endif // STATISTICS_HH
