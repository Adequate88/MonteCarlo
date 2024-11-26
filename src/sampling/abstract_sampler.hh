//
// Created by Alessandro Salo on 23.11.2024.
//

#ifndef ABSTRACT_SAMPLER_HH
#define ABSTRACT_SAMPLER_HH

#include <vector>

class AbstractSampler {
    /**
    */
    protected:
        ~AbstractSampler() = default;
        std::vector<double> distribution_array; // Array to store distribution based on bins
    public:
        virtual double sample() {return 0.0;}// Return random double-precision sample
        virtual void generateDistribution(int bins, int n_samples) {}
        void printDistribution();
};

#endif //ABSTRACT_SAMPLER_HH
