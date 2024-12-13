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
#include <gnuplot-iostream.h>

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

/**
 * @brief Plots the distribution of values using Gnuplot.
 *
 * This function calculates the bin centers for the distribution and visualizes the
 * probability density function (PDF) as a histogram-like plot. It can either display
 * the plot on-screen or save it as a PNG file. At least one of these options must be enabled.
 *
 * @param display A boolean flag indicating whether to display the plot on-screen.
 * @param filesave A boolean flag indicating whether to save the plot to a file as a PNG image.
 *
 * @throws std::invalid_argument If both `display` and `filesave` are set to false.
 */
void AbstractSampler::plotDistribution(bool display, bool filesave) const {
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
        gp << "set output 'pdf_plot.png'\n";
    }
    // Send Gnuplot commands to set labels and title
    gp << "set xlabel 'x'\n";
    gp << "set ylabel 'pdf(x)'\n";
    gp << "set title 'Binned Probability Density Function'\n";

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
        gp << "set output 'pdf_plot.png'\n";
        gp << "plot '-' using 1:2 with boxes lc rgb '#3B429F' title 'Probability Density'\n";
        gp.send1d(data);  // Send the data for plotting
    }
}

/**
 * @brief Retrieves the distribution array.
 *
 * This method returns a copy of the distribution array, which contains the binned
 * frequency values that represent the probability distribution.
 *
 * @return A vector of doubles representing the distribution array.
 */
std::vector<double> AbstractSampler::getDistribution() const {
  return distribution_array;
}

/**
 * @brief Retrieves the minimum value for the plot range.
 *
 * This method returns the minimum value used to define the range of the plot.
 *
 * @return The minimum value of the plot range.
 */
double AbstractSampler::getPlotMinimum() {
    return plot_minimum_;
}

/**
 * @brief Retrieves the maximum value for the plot range.
 *
 * This method returns the maximum value used to define the range of the plot.
 *
 * @return The maximum value of the plot range.
 */
double AbstractSampler::getPlotMaximum() {
    return plot_maximum_;
}
