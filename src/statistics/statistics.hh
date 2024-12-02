//
// Created by alixb1908 on 26/11/24.
//
#ifndef STATISTICS_HH
#define STATISTICS_HH

#include <vector>
#include <functional>
#include "../sampling/abstract_sampler.hh"
#include "../functions/abstract_function.h"

class Statistics {
public:
    // Constructor that accepts a sampler object
    Statistics(AbstractSampler& sampler, int num_samples);

    // Standard Destructor
    ~Statistics() = default;

    // Function to set data (can be used to set the random samples)
    void set_data(const std::vector<double>& samples);

    // Function to generate and add N new samples
    void gen_N_new_samples(int N);

    // Expectation value for an arbitrary function f
    [[nodiscard]] double expectation(const AbstractFunction<double,double>& f) const;

    // Variance for an arbitrary function f
    [[nodiscard]] double variance(const AbstractFunction<double,double>& f) const;

    // Generalized function to calculate any power moment for an arbitrary function f
    [[nodiscard]] double moment(int power, const AbstractFunction<double,double>& f) const;

    // Function to calculate central moments
    [[nodiscard]] double central_moment(int power, const AbstractFunction<double,double>& f) const;

    // Function to clear data
    void clear_data();

private:
    AbstractSampler& sampler;
    std::vector<double> data;
    unsigned int N;
};

#endif // STATISTICS_HH
