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
 * MONTE CARLO TESTS
 * ==========================================================
 */

TEST(MonteCarloTest, UniformSamplerTest) {
    std::string expression = "3*x + 2";
    Parser<double, double> parser(expression);
    auto ast = parser.parse();

    // Generate the function
    parser.generateFunction(ast.get());
    auto& f = *parser.getFinalFunction();

    // Sample from Normal distribution with mu = 0, sigma = 1
    int sample_num = 10000000;
    double epsilon = 0.1;
    UniformSampler sampler(42, 0, 1); // a = 0, b = 1
    Statistics stats(sampler, sample_num);

    EXPECT_NEAR(stats.expectation(f), 3.5, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 0.75, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 50.75, epsilon);

    EXPECT_NEAR(stats.moment(4, f), 206.2, epsilon);

    std::cout << "MonteCarloTest.UniformSamplerTest checked with tolerance " << epsilon << std::endl;
}


TEST(MonteCarloTest, NormalBoxMullerTest) {
    std::string expression = "3*x + 2";
    Parser<double, double> parser(expression);
    auto ast = parser.parse();

    // Generate the function
    parser.generateFunction(ast.get());
    auto& f = *parser.getFinalFunction();

    // Sample from Normal distribution with mu = 0, sigma = 1
    int sample_num = 10000000;
    double epsilon = 0.1;
    NormalBoxMullerSampler sampler(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats(sampler, sample_num);

    EXPECT_NEAR(stats.expectation(f), 2, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 9, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 62 , epsilon);

    EXPECT_NEAR(stats.moment(4, f), 475, epsilon);

    std::cout << "MonteCarloTest.NormalBoxMullerSamplerTest checked with tolerance " << epsilon << std::endl;
}

TEST(MonteCarloTest, NormalInverseTest) {
    std::string expression = "3*x + 2";
    Parser<double, double> parser(expression);
    auto ast = parser.parse();

    // Generate the function
    parser.generateFunction(ast.get());
    auto& f = *parser.getFinalFunction();

    // Sample from Normal distribution with mu = 0, sigma = 1
    int sample_num = 10000000;
    double epsilon = 1;
    NormalInverseSampler sampler(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats(sampler, sample_num);

    EXPECT_NEAR(stats.expectation(f), 2, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 9, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 62 , epsilon);

    EXPECT_NEAR(stats.moment(4, f), 475, epsilon);

    std::cout << "MonteCarloTest.NormalBoxMullerSamplerTest checked with tolerance " << epsilon << std::endl;
}
