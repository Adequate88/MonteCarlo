//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef NORMAL_INVERSE_SAMPLER_HH
#define NORMAL_INVERSE_SAMPLER_HH

#include "normal_sampler.hh"

class NormalInverseSampler : public NormalSampler {

    public:
        double sample() override; // Return random double-precision floating value within pre-defined interval
        double normal_quantile(double p);
        NormalInverseSampler(int seed); // Defines normal distribution with mean 0 and standard deviation 1
        NormalInverseSampler(int seed, double mu, double sigma); // Defines normal distribution with custom mean and standard deviation
};

#endif //NORMAL_INVERSE_SAMPLER_HH
