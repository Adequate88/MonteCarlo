#include "pcsc.hh"
#include <iostream>
#include "sampling/uniform_sampler.hh"
#include "sampling/nonuniform_sampler.hh"
#include "sampling/normal_sampler.hh"


int main(int argc, char **argv) {

    UniformSampler sampler(20, -2,5);
    for (int i = 1; i < 30; i++) {
        std::cout << sampler.sample() << std::endl;
    }

    sampler.generateDistribution(1000,1000000);
    sampler.printDistribution();

}
