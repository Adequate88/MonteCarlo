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

/**
 * @brief Generates a distribution for the Central Limit Theorem.
 *
 * This method takes multiple sets of N samples, calculates their means, and stores these sample means
 * in the `sample_means_` vector. It then divides the sample means into bins and calculates the frequency
 * of each bin to populate the `distribution_array`. The plot range (minimum and maximum) is also determined
 * from the sample means.
 *
 * @param bins The number of bins to divide the distribution into.
 * @param n_samples The number of samples to generate for the distribution.
 */
void CltTester::generateDistribution(int bins, int n_samples){
    // Take multiple sets of N samples and calculate their means
    std::cout << "Generating distribution for the Central Limit Theorem..." << std::endl;
    for (int i = 0; i < n_samples; ++i) {
        Statistics stats(sampler_, N_);
        double sample_mean = stats.expectation(f_);
        sample_means_.push_back(sample_mean);
    }
    plot_minimum_ = *std::min_element(sample_means_.begin(), sample_means_.end());
    plot_maximum_ = *std::max_element(sample_means_.begin(), sample_means_.end());
    if (bins < 3) {
        std::cout << "bins must be at least 3. Automatically setting to 3." << std::endl;
        bins = 3;
    }
    this->distribution_array.resize(bins, 0); // Initialize the array with 0s
    //split the sample_means into bins and calculate the frequency of each bin, store it in distribution_array
    double minimum = plot_minimum_;
    double maximum = plot_maximum_;
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

  /**
 * @brief Plots the distribution as a histogram.
 *
 * This method plots the binned distribution of sample means using Gnuplot. The user can either display
 * the plot on the screen or save it as a file. If neither `display` nor `filesave` is true, an exception
 * is thrown. The plot includes labels, title, and a customized range for the X and Y axes.
 *
 * @param display A flag indicating whether to display the plot.
 * @param filesave A flag indicating whether to save the plot to a file.
 * @throws std::invalid_argument If both `display` and `filesave` are false.
 */
void CltTester::plotDistribution(bool display, bool filesave) const {
    if (!display && !filesave) {
        // throw an error if both display and filesave are false
        throw std::invalid_argument("At least one of display or filesave must be true.");
    }
    const int bins = distribution_array.size();
    std::vector<double> bin_centers(bins, 0.0); // Midpoints of the bins

    const double delta_x = (plot_maximum_ - plot_minimum_) / static_cast<double>(bins);

    for (int i = 0; i < bins; i++) {
        // Calculate the center of the i-th bin
        bin_centers[i] = plot_minimum_ + (i + 0.5) * delta_x;
    }

    // Combine bin centers and frequencies into a single vector
    std::vector<std::pair<double, double>> data;
    for (size_t i = 0; i < bin_centers.size(); ++i) {
        data.push_back(std::make_pair(bin_centers[i], distribution_array[i]));
    }

    // Initialize Gnuplot
    Gnuplot gp;

    if (!display && filesave) {
        gp << "set terminal png size 800,600\n";
        gp << "set output 'clt_plot.png'\n";
    }

    // Send Gnuplot commands to set labels and title
    gp << "set xlabel 'x'\n";
    gp << "set ylabel 'pdf(x)'\n";
    gp << "set title 'Binned Distribution of Sample Means'\n";

    gp << "set boxwidth 0.8 relative\n";  // 0.8 relative to the bin spacing
    gp << "set style fill solid 1.0\n";   // Solid fill for bars

    gp << "set yrange [0:*]\n";  // Set the Y-axis to range
    gp << "set xrange [" << plot_minimum_ << ":" << plot_maximum_ << "]\n";  // Set the X-axis to range

    // Plot the data using lines
    gp << "plot '-' using 1:2 with boxes lc rgb '#3B429F' title 'Probability Density'\n";
    gp.send1d(data);  // Send the data for plotting

    if (display && filesave) {
        // Configure terminal and output file
        gp << "set terminal png size 800,600\n";
        gp << "set output 'clt_plot.png'\n";
        gp << "plot '-' using 1:2 with boxes lc rgb '#3B429F' title 'Probability Density'\n";
        gp.send1d(data);  // Send the data for plotting
    }
}

/**
 * @brief Tests the distribution by plotting it as a histogram.
 *
 * This method calls the `plotDistribution` function to visualize the distribution as a histogram. It takes
 * the same `display` and `filesave` flags to control whether the plot is displayed or saved.
 *
 * @param display A flag indicating whether to display the plot.
 * @param filesave A flag indicating whether to save the plot to a file.
 */
void CltTester::test(bool display, bool filesave) const {
    // Plot the distribution as a histogram
    std::cout<< "Plotting the distribution as a histogram" << std::endl;
    this -> plotDistribution(display, filesave);
}
