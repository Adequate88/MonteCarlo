//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef NORMAL_SAMPLER
#define NORMAL_SAMPLER

#include "uniform_sampler.hh"

class NormalSampler : public AbstractSampler{

  public:
    NormalSampler(int seed, double mu, double sigma) : seed(seed), uniform_sampler(seed), mu(mu), sigma(sigma) {};
    void generateDistribution(int bins, int n_samples) override;

protected:
    int seed;
    UniformSampler uniform_sampler;
    double mu, sigma;
};

#endif //NORMAL_SAMPLER
