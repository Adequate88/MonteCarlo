//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef NORMAL_BOXMULLER_SAMPLER_HH
#define NORMAL_BOXMULLER_SAMPLER_HH

#include "normal_sampler.hh"

class NormalBoxMullerSampler : public NormalSampler {

    private:
        double last_muller; // Variable to store extra Box-Muller Computations
        bool use_last_muller; // Boolean to check use of Box-Muller Computation
        const double two_pi;

    public:
        double sample() override; // Return random double-precision floating value within pre-defined interval
        double normal_quantile(double p);
        NormalBoxMullerSampler(int seed); // Defines normal distribution with mean 0 and standard deviation 1
        NormalBoxMullerSampler(int seed, double mu, double sigma); // Defines normal distribution with custom mean and standard deviation
};

#endif //NORMAL_BOXMULLER_SAMPLER_HH
