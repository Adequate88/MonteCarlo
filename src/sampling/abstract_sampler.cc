//
// Created by Alessandro Salo on 24.11.2024.
//
/**
 * @file abstract_sampler.cc
 * @brief Implements methods of the AbstractSampler class.
 *
 * This file contains the implementation of the non-virtual methods of
 * the `AbstractSampler` class, including printing the distribution data.
 */

#include "abstract_sampler.hh"
#include <iostream>


/**
 * @brief Prints the distribution array to the standard output.
 *
 * Iterates over the `distribution_array` and prints the value of each bin
 * in the format "Bin [index] : [value]".
 */
void AbstractSampler::printDistribution() {

    for (int i = 0; i < distribution_array.size(); i ++) {

        std::cout << "Bin " << i << " : "<< distribution_array[i] << std::endl;

    }

}
