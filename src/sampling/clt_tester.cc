//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file clt_tester.cc
 * @brief Initializes functions for CLT Tester
 */

#include "clt_tester.hh"
#include "statistics.hh"
#include "gnuplot-iostream.h"
#include "primitive_functions.hh"
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>

/**
 * @brief Constructs a CltTester object.
 *
 * @param sampler Reference to an AbstractSampler used to generate random samples.
 * @param N Number of samples per iteration.
 */
CltTester::CltTester(AbstractSampler& sampler,AbstractFunction<double,double>& f, int N)
    : sampler_(sampler),f_(f), N_(N) {
    std::cout<< "CltTester object created" << std::endl;
}

void CltTester::generateDistribution(int bins, int n_samples){
    // Take multiple sets of N samples and calculate their means
    std::cout << "Generating distribution for the Central Limit Theorem..." << std::endl;
    for (int i = 0; i < n_samples; ++i) {
        Statistics stats(sampler_, N_);
        double sample_mean = stats.expectation(f_);
        sample_means_.push_back(sample_mean);
    }
    plot_minimum = *std::min_element(sample_means_.begin(), sample_means_.end());
    plot_maximum = *std::max_element(sample_means_.begin(), sample_means_.end());
    if (bins < 3) {
        std::cout << "bins must be at least 3. Automatically setting to 3." << std::endl;
        bins = 3;
    }
    this->distribution_array.resize(bins, 0); // Initialize the array with 0s
    //split the sample_means into bins and calculate the frequency of each bin, store it in distribution_array
    double minimum = plot_minimum;
    double maximum = plot_maximum;
    const double delta_x = (maximum - minimum) / static_cast<double>(bins);
    for (int i = 0; i < n_samples; i++) {
        const double current_sample = sample_means_[i] - minimum;
        int index;
        // Compute the index safely
        if (current_sample < 0) {
            index = 0;
        } else if (current_sample >= maximum - minimum) {
            index = bins - 1;
        } else {
            index = static_cast<int>(std::floor(current_sample / delta_x));
        }
        // Increment the corresponding bin by the fractional value
        this->distribution_array[index] += 1.0 / (n_samples * delta_x);
    }
  }

void CltTester::plotDistribution() const {

    const int bins = distribution_array.size();
    std::vector<double> bin_centers(bins, 0.0); // Midpoints of the bins

    const double delta_x = (plot_maximum - plot_minimum) / static_cast<double>(bins);

    for (int i = 0; i < bins; i++) {
        // Calculate the center of the i-th bin
        bin_centers[i] = plot_minimum + (i + 0.5) * delta_x;
    }

    // Combine bin centers and frequencies into a single vector
    std::vector<std::pair<double, double>> data;
    for (size_t i = 0; i < bin_centers.size(); ++i) {
        data.push_back(std::make_pair(bin_centers[i], distribution_array[i]));
    }

    // Initialize Gnuplot
    Gnuplot gp;

    // Send Gnuplot commands to set labels and title
    gp << "set xlabel 'x'\n";
    gp << "set ylabel 'pdf(x)'\n";
    gp << "set title 'Binned Distribution of Sample Means'\n";

    gp << "set boxwidth 0.8 relative\n";  // 0.8 relative to the bin spacing
    gp << "set style fill solid 1.0\n";   // Solid fill for bars

    gp << "set yrange [0:*]\n";  // Set the Y-axis to range
    gp << "set xrange [" << plot_minimum << ":" << plot_maximum << "]\n";  // Set the X-axis to range

    // Plot the data using lines
    gp << "plot '-' using 1:2 with boxes lc rgb '#3B429F' title 'Probability Density'\n";
    gp.send1d(data);  // Send the data for plotting
}

/**
 * @brief Tests the Central Limit Theorem.
 *
 * This method generates multiple sets of `N` samples, calculates their means, and computes
 * the mean and standard deviation of those sample means. The results are then printed
 * to the console, along with the expected standard deviation as per the CLT.
 */
void CltTester::test() const {
    // Plot the distribution as a histogram
    std::cout<< "Plotting the distribution as a histogram" << std::endl;
    this -> plotDistribution();
}
