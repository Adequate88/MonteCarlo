//
// Created by alixb1908 on 26/11/24.
//
/**
 * @file statistics.cc
 * @brief Implements the Statistics class.
 *
 * Contains the implementation of statistical methods for sample analysis,
 * including expectation, variance, and moment calculations.
 */

#include "statistics.hh"
#include <numeric>
#include <cmath>


/**
 * @brief Constructs a Statistics object with an initial set of samples.
 *
 * Samples `num_samples` values from the provided sampler and stores them.
 * @param sampler_ref Reference to an AbstractSampler object used to generate the samples.
 * @param num_samples Number of samples to initially generate.
 */
Statistics::Statistics(AbstractSampler& sampler_ref, int num_samples) : sampler(sampler_ref), N(num_samples) {
    for (int i = 0; i < num_samples; ++i) {
        data.push_back(sampler_ref.sample());
    }
}

/**
 * @brief Sets the sample data.
 *
 * Replaces the current sample data with the provided vector of samples.
 * @param samples Vector of sample values.
 */
void Statistics::set_data(const std::vector<double>& samples) {
    data = samples;
    N = samples.size();
}

/**
 * @brief Generates and adds new samples to the dataset.
 *
 * Generates `N` new samples using the associated sampler and appends them to the existing data.
 * @param N Number of new samples to generate.
 */
void Statistics::gen_N_new_samples(int N) {
    for (int i = 0; i < N; ++i) {
        data.push_back(sampler.sample());
    }
    this->N += N;
}

/**
 * @brief Computes the expectation value of an arbitrary function.
 *
 * Calculates the mean value of the function applied to the samples.
 * @param f Reference to an AbstractFunction object representing the function to evaluate.
 * @return The expectation value.
 */
double Statistics::expectation(const  AbstractFunction<double,double>& f) const {
    double sum = 0.0;
    for (double value : data) {
        sum += f.eval(value);
    }
    return sum / N;
}

/**
 * @brief Computes the variance of an arbitrary function.
 *
 * Calculates the variance of the function applied to the samples.
 * @param f Reference to an AbstractFunction object representing the function to evaluate.
 * @return The variance.
 */
double Statistics::variance(const AbstractFunction<double,double>& f) const {
    const double exp_value = expectation(f);
    double sum = 0.0;
    for (const double value : data) {
        const double diff = f.eval(value) - exp_value;
        sum += diff * diff;
    }
    return sum / N;
}

/**
 * @brief Computes a power moment of an arbitrary function.
 *
 * Calculates the `power`-th moment of the function applied to the samples.
 * @param power The order of the moment to compute.
 * @param f Reference to an AbstractFunction object representing the function to evaluate.
 * @return The computed moment.
 */
double Statistics::moment(int power, const AbstractFunction<double,double>& f) const {

    double sum = 0.0;
    for (double value : data) {
        sum += std::pow(f.eval(value), power);
    }
    return sum / N;
}

/**
 * @brief Computes a central moment of an arbitrary function.
 *
 * Calculates the central `power`-th moment of the function applied to the samples.
 * @param power The order of the central moment to compute.
 * @param f Reference to an AbstractFunction object representing the function to evaluate.
 * @return The computed central moment.
 */
double Statistics::central_moment(int power, const AbstractFunction<double,double>& f) const {
    const double mean_value = expectation(f);
    double sum = 0.0;
    for (const double value : data) {
        sum += std::pow(f.eval(value)- mean_value, power);
    }
    return sum / N;
}

/**
 * @brief Clears all sample data.
 *
 * Resets the sample dataset and the sample count to zero.
 */
void Statistics::clear_data() {
    data.clear();
    N = 0;
}

/**
 * @brief Change sampler attribute
 *
 * Change the sampler attribute to the new sampler
 */
void Statistics::change_sampler(AbstractSampler& new_sampler) {
    sampler = new_sampler;
}






