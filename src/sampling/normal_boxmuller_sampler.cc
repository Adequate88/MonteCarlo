//
// Created by Alessandro Salo on 22.11.2024.
//

#include "normal_boxmuller_sampler.hh"
#include <cmath>
#include <iostream>

NormalBoxMullerSampler::NormalBoxMullerSampler(int seed)
    : NormalSampler(seed, 0.0, 1.0),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

NormalBoxMullerSampler::NormalBoxMullerSampler(int seed, double mu, double sigma)
    : NormalSampler(seed, mu, sigma),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

double NormalBoxMullerSampler::sample(){

    if(use_last_muller){
        use_last_muller = false;
        return last_muller;
    }

    double u1,u2;
    do
    {
        u1 = uniform_sampler.sample();
    }
    while (u1 == 0.0);
    u2 = uniform_sampler.sample();

    double mag = sigma * sqrt(-2.0 * log(u1));
    double z0  = mag * cos(two_pi * u2) + mu;
    double z1  = mag * sin(two_pi * u2) + mu;

    last_muller = z1;
    use_last_muller = true;

    return z0;
}

