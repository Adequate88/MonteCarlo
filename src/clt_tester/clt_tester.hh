//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file clt_tester.hh
 * @brief Defines the CltTester class, which tests the Central Limit Theorem (CLT) using sample data.
 *
 * This file contains the definition of the CltTester class. The CltTester class is used to
 * demonstrate and validate the Central Limit Theorem by generating multiple samples and
 * calculating the mean and standard deviation of those sample means.
 */
#ifndef CLT_TESTER_HH
#define CLT_TESTER_HH

#include "abstract_sampler.hh"


/**
 * @brief Class that tests the Central Limit Theorem using an abstract sampler.
 *
 * This class takes in an abstract sampler and generates multiple sets of samples, calculates their
 * means, and then checks if the mean and standard deviation of these sample means conform to the
 * expected properties defined by the Central Limit Theorem.
 */
class CltTester {
public:
    /**
     * @brief Constructs a CltTester object.
     *
     * @param sampler Reference to an AbstractSampler used to generate random samples.
     * @param N Number of samples per iteration.
     * @param num_iterations Number of iterations to perform.
     */
    CltTester(AbstractSampler& sampler, int N, int num_iterations);

    /**
     * @brief Tests the Central Limit Theorem.
     *
     * This method generates multiple sets of `N` samples, calculates their means, and computes
     * the mean and standard deviation of those sample means. The results are then printed
     * to the console, along with the expected standard deviation as per the CLT.
     */
    void test() const;

private:
    AbstractSampler& sampler_; ///< Reference to the sampler used to generate the samples.
    int N_; ///< Number of samples per iteration.
    int num_iterations_; ///< Number of iterations to perform.
};

#endif // CLT_TESTER_HH
