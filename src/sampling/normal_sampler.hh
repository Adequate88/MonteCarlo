//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef NORMAL_SAMPLER_HH
#define NORMAL_SAMPLER_HH

#include "nonuniform_sampler.hh"

class normal_sampler : public nonuniform_sampler {

    private:
        double mu, sigma; // Mean and Standard Deviation
        double last_muller; // Variable to store extra Box-Muller Computations
        bool use_last_muller; // Boolean to check use of Box-Muller Computation
        const double two_pi;

    public:
        double BoxMullerSample();
        double normal_quantile(double p);
        double InverseTransformSample();

        normal_sampler(int seed); // Defines normal distribution with mean 0 and standard deviation 1
        normal_sampler(int seed, double mu, double sigma); // Defines normal distribution with custom mean and standard deviation

};

#endif //NORMAL_SAMPLER_HH
