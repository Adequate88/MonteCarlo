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

void AbstractSampler::plotDistribution() const {

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
}

std::vector<double> AbstractSampler::getDistribution() const {
  return distribution_array;
}

double AbstractSampler::getPlotMinimum() {
    return plot_minimum_;
}

double AbstractSampler::getPlotMaximum() {
    return plot_maximum_;
}
