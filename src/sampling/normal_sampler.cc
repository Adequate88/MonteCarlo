//
// Created by Alessandro Salo on 22.11.2024.
//

#include "normal_sampler.hh"
#include <cmath>
#include <iostream>

normal_sampler::normal_sampler(int seed)
    : nonuniform_sampler(seed),
      mu(0.0), sigma(1.0),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

normal_sampler::normal_sampler(int seed, double mu, double sigma)
    : nonuniform_sampler(seed),
      mu(mu), sigma(sigma),
      last_muller(0.0), use_last_muller(false), two_pi(2.0*M_PI) {}

double normal_sampler::BoxMullerSample(){

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

// Approximation of the normal quantile function (Beasley-Springer-Moro)
double normal_sampler::normal_quantile(double p) {
    if (p <= 0.0 || p >= 1.0) {
        throw std::invalid_argument("p must be in the range (0, 1)");
    }

    static const double a[] = {
        2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637
    };
    static const double b[] = {
        -8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833
    };
    static const double c[] = {
        0.3374754822726147, 0.9761690190917186, 0.1607979714918209,
        0.0276438810333863, 0.0038405729373609, 0.0003951896511919,
        0.0000321767881768, 0.0000002888167364, 0.0000003960315187
    };

    double x = p - 0.5;
    double r;

    if (std::abs(x) < 0.42) {
        // Beasley-Springer approximation
        r = x * x;
        return x * (((a[3] * r + a[2]) * r + a[1]) * r + a[0]) /
               ((((b[3] * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    } else {
        // Moro's approximation
        r = (x < 0.0) ? p : 1.0 - p;
        r = std::log(-std::log(r));
        double z = c[0];
        for (int i = 1; i < 9; ++i) {
            z += c[i] * std::pow(r, i);
        }
        return (x < 0.0) ? -z : z;
    }
}

double normal_sampler::InverseTransformSample(){
    double u1 = uniform_sampler.sample();
    return normal_quantile(u1);
}


