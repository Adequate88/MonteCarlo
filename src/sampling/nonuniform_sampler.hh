//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef NONUNIFORM_SAMPLER_HH
#define NONUNIFORM_SAMPLER_HH

#include "uniform_sampler.hh"

class nonuniform_sampler {

  public:
    nonuniform_sampler(int seed) : seed(seed), uniform_sampler(seed) {}

  protected:
    int seed;
    UniformSampler uniform_sampler;

};

#endif //NONUNIFORM_SAMPLER_HH
