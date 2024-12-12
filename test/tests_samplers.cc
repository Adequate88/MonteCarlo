#include <cmath>
#include <gtest/gtest.h>
#include "sampling.hh"
#include "statistics.hh"
#include <cmath>
#include <gtest/gtest.h>
#include "sampling.hh"
#include "statistics.hh"
#include "parse_tree.hh"

/* ==========================================================
 * TEMPLATED TEST FUNCTIONS
 * ==========================================================
 */

/**
 * @brief Tests the consistency of a sampler by ensuring that two samplers
 *        initialized with the same seed produce identical sequences of samples.
 *
 * @tparam Sampler The type of sampler being tested.
 * @param seed The seed used to initialize the samplers.
 */
template <typename Sampler>
void testSamplerConsistency(int seed) {
    Sampler sampler1(seed);
    Sampler sampler2(seed);

    for (int i = 0; i < 1000; i++) {
        EXPECT_EQ(sampler1.sample(), sampler2.sample());
    }
}

/**
 * @brief Verifies statistical properties of a uniform distribution sampler.
 *
 * @tparam StatsType The type of statistics calculator being used.
 * @param stats A statistics object that has collected samples from the uniform sampler.
 * @param min The minimum value of the uniform distribution.
 * @param max The maximum value of the uniform distribution.
 * @param epsilon The tolerance for statistical comparisons.
 */
template <typename StatsType>
void CheckUniformStats(const StatsType& stats, double min, double max, double epsilon) {
    auto func = IdentityFunction<double>();

    // Check the expectation value (mean of the uniform distribution)
    EXPECT_NEAR(stats.expectation(func), (max + min) / 2.0, epsilon);

    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(func), (max - min) * (max - min) / 12.0, epsilon);

    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    double expected_moment = ((max * max * max * max) - (min * min * min * min)) / (4 * (max - min));
    EXPECT_NEAR(stats.moment(3, func), expected_moment, epsilon);

    // Check the third central moment
    EXPECT_NEAR(stats.centralMoment(3, func), 0.0, epsilon);
}

/**
 * @brief Verifies statistical properties of a normal distribution sampler.
 *
 * @tparam StatsType The type of statistics calculator being used.
 * @param stats A statistics object that has collected samples from the normal sampler.
 * @param mu The mean of the normal distribution.
 * @param sigma The standard deviation of the normal distribution.
 * @param epsilon The tolerance for statistical comparisons.
 */
template <typename StatsType>
void CheckNormalStats(const StatsType& stats, double mu, double sigma, double epsilon) {
    auto func = IdentityFunction<double>();

    // Check the expectation value (mean of the normal distribution)
    EXPECT_NEAR(stats.expectation(func), mu, epsilon);

    // Check the variance (theoretical variance is sigma^2)
    EXPECT_NEAR(stats.variance(func), sigma * sigma, epsilon);

    // Check the third moment (should be 0 for a normal distribution due to symmetry)
    EXPECT_NEAR(stats.moment(3, func), std::pow(mu, 3) + 3 * mu * sigma * sigma, epsilon);

    // Check the third central moment (theoretical third central moment)
    EXPECT_NEAR(stats.centralMoment(3, func), 0, epsilon);

    // Check the fourth moment (theoretical value for normal distribution)
    double expected_4th = std::pow(mu, 4) + 3 * std::pow(sigma, 4) + 6 * std::pow(sigma * mu, 2);
    EXPECT_NEAR(stats.moment(4, func),expected_4th , epsilon);

    // Check the fourth central moment (theoretical value)
    EXPECT_NEAR(stats.centralMoment(4, func), 3 * std::pow(sigma, 4), epsilon);
}

/* ==========================================================
 * UNIFORM DISTRIBUTION TESTS
 * ==========================================================
 */

TEST(Uniform, Consistency) {
    // Ensures that uniform samplers initialized with the same seed are consistent.
    testSamplerConsistency<UniformSampler>(42);
}

TEST(Uniform, RangeCheck) {
    // Define three uniform samplers with different ranges
    UniformSampler sampler1(42); // [0,1] range
    UniformSampler sampler2(42, -2, 4); // [-2,4] range
    UniformSampler sampler3(42, -0.0001, 0.001); // [-0.0001, 0.001] range

    for (int i = 0; i < 10000; i++) {
        // Check bounds for each sampler
        EXPECT_LE(0, sampler1.sample());
        EXPECT_GE(1, sampler1.sample());

        EXPECT_LE(-2, sampler2.sample());
        EXPECT_GE(4, sampler2.sample());

        EXPECT_LE(-0.0001, sampler3.sample());
        EXPECT_GE(0.001, sampler3.sample());
    }
}

TEST(Uniform, StatisticCheck) {
    int sample_num = 1000000;
    double epsilon = 1E-2;

    // Generate samples and calculate statistics
    UniformSampler sampler1(42); // [0,1] range
    Statistics stats1(sampler1, sample_num);

    UniformSampler sampler2(42, -2, 4); // [-2,4] range
    Statistics stats2(sampler2, sample_num);

    UniformSampler sampler3(42, -0.0001, 0.001); // [-0.0001,0.001] range
    Statistics stats3(sampler3, sample_num);

    // Validate statistical properties for each sampler
    CheckUniformStats(stats1, 0, 1, epsilon);
    CheckUniformStats(stats2, -2, 4, epsilon);
    CheckUniformStats(stats3, -0.0001, 0.001, epsilon);

    std::cout << "Uniform.StatisticCheck checked with tolerance " << epsilon << std::endl;
}

TEST(Uniform, PDFCheck) {
    int sample_num = 1000000;
    int bins = 1000;
    double epsilon = 1E-2;


    // Generate samples and calculate statistics
    UniformSampler sampler(42); // [0,1] range
    sampler.generateDistribution(bins, sample_num);

    std::vector<double> pdf = sampler.getDistribution();

    double sum = 0;
    double delta_x = (sampler.getMaximum() - sampler.getMinimum()) / static_cast<double>(bins);
    for (int i = 0; i < bins; i++) {
        sum += pdf[i]*delta_x;
    }
    EXPECT_NEAR(sum,1 , epsilon);

}

/* ==========================================================
 * NORMAL INVERSE SAMPLER TESTS
 * ==========================================================
 */

TEST(NormalInverse, Consistency) {
    // Ensures that normal inverse samplers initialized with the same seed are consistent.
    testSamplerConsistency<NormalInverseSampler>(42);
}

TEST(NormalInverse, StatisticCheck) {
    int sample_num = 1000000;
    double epsilon = 1;

    // Generate samples and calculate statistics for three different Gaussians
    NormalInverseSampler sampler1(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats1(sampler1, sample_num);

    NormalInverseSampler sampler2(42, 3, 2); // mu = 3, sigma = 2
    Statistics stats2(sampler2, sample_num);

    NormalInverseSampler sampler3(42, -1.5, 0.5); // mu = -1.5, sigma = 0.5
    Statistics stats3(sampler3, sample_num);

    // Validate statistical properties
    CheckNormalStats(stats1, 0, 1, epsilon);
    CheckNormalStats(stats2, 3, 2, epsilon);
    CheckNormalStats(stats3, -1.5, 0.5, epsilon);

    std::cout << "NormalInverse.StatisticCheck checked with tolerance " << epsilon << std::endl;
}

TEST(NormalInverse, PDFCheck) {
    int sample_num = 1000000;
    int bins = 1000;
    double epsilon = 1E-2;


    // Generate samples and calculate statistics
    NormalInverseSampler sampler(42); // [0,1] range
    sampler.generateDistribution(bins, sample_num);

    std::vector<double> pdf = sampler.getDistribution();

    double sum = 0;
    double delta_x = (sampler.getPlotMaximum() - sampler.getPlotMinimum()) / static_cast<double>(bins);
    for (int i = 0; i < bins; i++) {
        sum += pdf[i]*delta_x;
    }
    EXPECT_NEAR(sum,1 , epsilon);

}

/* ==========================================================
 * NORMAL BOX-MULLER SAMPLER TESTS
 * ==========================================================
 */

TEST(NormalMuller, Consistency) {
    // Ensures that normal Box-Muller samplers initialized with the same seed are consistent.
    testSamplerConsistency<NormalBoxMullerSampler>(42);
}

TEST(NormalMuller, StatisticCheck) {
    int sample_num = 1000000;
    double epsilon = 1;

    // Generate samples and calculate statistics for three different Gaussians
    NormalBoxMullerSampler sampler1(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats1(sampler1, sample_num);

    NormalBoxMullerSampler sampler2(42, 3, 2); // mu = 3, sigma = 2
    Statistics stats2(sampler2, sample_num);

    NormalBoxMullerSampler sampler3(42, -1.5, 0.5); // mu = -1.5, sigma = 0.5
    Statistics stats3(sampler3, sample_num);

    // Validate statistical properties
    CheckNormalStats(stats1, 0, 1, epsilon);
    CheckNormalStats(stats2, 3, 2, epsilon);
    CheckNormalStats(stats3, -1.5, 0.5, epsilon);

    std::cout << "NormalBoxMuller.StatisticCheck checked with tolerance " << epsilon << std::endl;
}

TEST(NormalMuller, PDFCheck) {
    int sample_num = 1000000;
    int bins = 1000;
    double epsilon = 1E-2;


    // Generate samples and calculate statistics
    NormalBoxMullerSampler sampler(42); // [0,1] range
    sampler.generateDistribution(bins, sample_num);

    std::vector<double> pdf = sampler.getDistribution();

    double sum = 0;
    double delta_x = (sampler.getPlotMaximum() - sampler.getPlotMinimum()) / static_cast<double>(bins);
    for (int i = 0; i < bins; i++) {
        sum += pdf[i]*delta_x;
    }
    EXPECT_NEAR(sum,1 , epsilon);

}