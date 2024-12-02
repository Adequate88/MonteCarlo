//
// Created by Alessandro Salo on 24.11.2024.
//
#include "abstract_sampler.hh"
#include <iostream>

void AbstractSampler::printDistribution() {

    for (int i = 0; i < distribution_array.size(); i ++) {

        std::cout << "Bin " << i << " : "<< distribution_array[i] << std::endl;

    }

}
