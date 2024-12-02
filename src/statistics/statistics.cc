//
// Created by alixb1908 on 26/11/24.
//
#include "statistics.hh"
#include <numeric>
#include <cmath>


// Constructor
Statistics::Statistics(AbstractSampler& sampler_ref, int num_samples) : sampler(sampler_ref), N(num_samples) {
    for (int i = 0; i < num_samples; ++i) {
        data.push_back(sampler_ref.sample());
    }
}

// Function to set data (can be used to set the random samples)
void Statistics::set_data(const std::vector<double>& samples) {
    data = samples;
    N = samples.size();
}

// Function to generate and add N new samples
void Statistics::gen_N_new_samples(int N) {
    for (int i = 0; i < N; ++i) {
        data.push_back(sampler.sample());
    }
    this->N += N;
}

// Expectation value for an arbitrary function f
double Statistics::expectation(const  AbstractFunction<double,double>& f) const {
    double sum = 0.0;
    for (double value : data) {
        sum += f.eval(value);
    }
    return sum / N;
}

// Variance for an arbitrary function f
double Statistics::variance(const AbstractFunction<double,double>& f) const {
    const double exp_value = expectation(f);
    double sum = 0.0;
    for (const double value : data) {
        const double diff = f.eval(value) - exp_value;
        sum += diff * diff;
    }
    return sum / N;
}

// Generalized function to calculate any power moment for an arbitrary function f
double Statistics::moment(int power, const AbstractFunction<double,double>& f) const {
    const double mean_value = expectation(f);
    double sum = 0.0;
    for (double value : data) {
        sum += std::pow(f.eval(value) - mean_value, power);
    }
    return sum / N;
}

// Function to calculate central moments
double Statistics::central_moment(int power, const AbstractFunction<double,double>& f) const {
    double sum = 0.0;
    for (const double value : data) {
        sum += std::pow(f.eval(value), power);
    }
    return sum / N;
}

// Function to clear data
void Statistics::clear_data() {
    data.clear();
    N = 0;
}






