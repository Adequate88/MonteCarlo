#include <clt_tester.hh>
#include <cmath>
#include <fstream>
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
bool areFilesIdentical(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary | std::ios::ate);
    std::ifstream f2(file2, std::ios::binary | std::ios::ate);

    if (!f1.is_open() || !f2.is_open()) {
        return false; // One or both files cannot be opened
    }

    if (f1.tellg() != f2.tellg()) {
        return false; // Files have different sizes
    }

    f1.seekg(0);
    f2.seekg(0);

    return std::equal(std::istreambuf_iterator<char>(f1),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2));
}

TEST(MonteCarloTest, UniformSamplerTest) {
    std::string expression = "3*x + 2";
    Parser<double, double> parser(expression);
    auto ast = parser.parse();

    // Generate the function
    parser.generateFunction(ast.get());
    auto& f = *parser.getFinalFunction();

    // Sample from Normal distribution with mu = 0, sigma = 1
    int sample_num = 1e5;
    double epsilon = 1;
    int sample_mean_num = 1e3;
    int bins = 50;
    UniformSampler sampler(42, 0, 1); // a = 0, b = 1
    Statistics stats(sampler, sample_num);
    sampler.generateDistribution(bins, sample_num);
    sampler.plotDistribution();

    //Test CLT
    CltTester clt(sampler, f, sample_num);
    clt.generateDistribution(bins, sample_mean_num);
    clt.test();

    EXPECT_NEAR(stats.expectation(f), 3.5, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 0.75, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 50.75, 2*epsilon);

    EXPECT_NEAR(stats.moment(4, f), 206.2, 20*epsilon);

    bool pdf_plots_identical = areFilesIdentical("../test/figs/uniform_pdf_plot_test.png", "pdf_plot.png");
    EXPECT_TRUE(pdf_plots_identical);

    bool clt_plots_identical = areFilesIdentical("../test/figs/uniform_clt_plot_test.png", "clt_plot.png");
    EXPECT_TRUE(clt_plots_identical);

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
    int sample_num = 1e5;
    double epsilon = 1;
    NormalBoxMullerSampler sampler(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats(sampler, sample_num);
    int sample_mean_num = 1e3;
    int bins = 50;
    sampler.generateDistribution(bins, sample_num);
    sampler.plotDistribution();

    //Test CLT
    CltTester clt(sampler, f, sample_num);
    clt.generateDistribution(bins, sample_mean_num);
    clt.test();


    EXPECT_NEAR(stats.expectation(f), 2, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 9, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 62 , 2*epsilon);

    EXPECT_NEAR(stats.moment(4, f), 475, 20*epsilon);

    bool pdf_plots_identical = areFilesIdentical("../test/figs/box_muller_pdf_plot_test.png", "pdf_plot.png");
    EXPECT_TRUE(pdf_plots_identical);

    bool clt_plots_identical = areFilesIdentical("../test/figs/box_muller_clt_plot_test.png", "clt_plot.png");
    EXPECT_TRUE(clt_plots_identical);
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
    int sample_num = 1e5;
    double epsilon = 1;
    NormalInverseSampler sampler(42, 0, 1); // mu = 0, sigma = 1
    Statistics stats(sampler, sample_num);
    int sample_mean_num = 1e3;
    int bins = 50;
    sampler.generateDistribution(bins, sample_num);
    sampler.plotDistribution();

    //Test CLT
    CltTester clt(sampler, f, sample_num);
    clt.generateDistribution(bins, sample_mean_num);
    clt.test();

    EXPECT_NEAR(stats.expectation(f), 2, epsilon);
    // Check the variance (theoretical variance of the uniform distribution)
    EXPECT_NEAR(stats.variance(f), 9, epsilon);
    // Check the third moment (for uniform distribution, it should be 0 due to symmetry)
    EXPECT_NEAR(stats.moment(3, f), 62 , 2*epsilon);

    EXPECT_NEAR(stats.moment(4, f), 475, 20*epsilon);

    bool pdf_plots_identical = areFilesIdentical("../test/figs/normal_inverse_pdf_plot_test.png", "pdf_plot.png");
    EXPECT_TRUE(pdf_plots_identical);
    bool clt_plots_identical = areFilesIdentical("../test/figs/normal_inverse_clt_plot_test.png", "clt_plot.png");
    EXPECT_TRUE(clt_plots_identical);

    std::cout << "MonteCarloTest.NormalBoxMullerSamplerTest checked with tolerance " << epsilon << std::endl;
}
