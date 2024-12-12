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
#include "abstract_function.hh"

/**
 * @brief Class that tests the Central Limit Theorem using an abstract sampler.
 *
 * This class takes in an abstract sampler and generates multiple sets of samples, calculates their
 * means, and then checks if the mean and standard deviation of these sample means conform to the
 * expected properties defined by the Central Limit Theorem.
 */
class CltTester : public AbstractSampler {
public:
    /**
     * @brief Constructs a CltTester object.
     *
     * @param sampler Reference to an AbstractSampler used to generate random samples.
     * @param f Reference to an AbstractFunction used to evaluate the samples.
     * @param N Number of samples per iteration.
     * @param num_iterations Number of iterations to perform.
     */
    CltTester(AbstractSampler& sampler, AbstractFunction<double,double>& f, int N);


    void generateDistribution(int bins, int n_samples) override;

    /**
     * @brief Tests the Central Limit Theorem.
     *
     * Wrapper function that plots the distribution of sample means to test the CLT.
     *
     * @params display Flag to display the plot.
     * @params filesave Flag to save the plot to a file.
     */
    void test(bool display, bool filesave) const;

    /**
    * @brief Plots the distribution of sample means as a histogram.
    *
    * Overridden from the AbstractSampler class. Changes some GnuPlot labels to be more specific to the CLT.
    *
    * @params display Flag to display the plot.
    * @params filesave Flag to save the plot to a file.
    */
    void plotDistribution(bool display, bool filesave) const override;


private:
    AbstractSampler& sampler_; ///< Reference to the sampler used to generate the samples.
    AbstractFunction<double,double>& f_; ///< Reference to the function used to evaluate the samples.
    int N_; ///< Number of samples per iteration.
    // int num_iterations_; ///< Number of iterations to perform.
    std::vector<double> sample_means_; ///< Vector to store the sample means.
};

#endif // CLT_TESTER_HH
