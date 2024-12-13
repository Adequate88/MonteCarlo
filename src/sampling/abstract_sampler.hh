//
// Created by Alessandro Salo on 23.11.2024.
//
/**
 * @file abstract_sampler.hh
 * @brief Defines the AbstractSampler class for sampling distributions.
 *
 * This file contains the abstract base class `AbstractSampler`, which provides
 * an interface for generating and sampling from distributions.
 */
#ifndef ABSTRACT_SAMPLER_HH
#define ABSTRACT_SAMPLER_HH

#include <vector>
/**
 * @class AbstractSampler
 * @brief Abstract base class for sampling and distribution generation.
 *
 * The `AbstractSampler` class provides a virtual interface for generating
 * distributions and sampling values. Derived classes should override
 * the virtual methods to implement specific sampling logic.
 */
class AbstractSampler {
    protected:
        /**
        * @brief Array to store distribution data based on bins.
        */
        std::vector<double> distribution_array; ///< Vector containing emperical pdf in bins
        double plot_minimum_; ///< Minimum value for plot (helps with bin evaluation)
        double plot_maximum_; ///< Maximum value for plot (helps with bin evaluation)
    public:
        /**
        * @brief Virtual destructor for safe polymorphic behavior.
        */
        virtual ~AbstractSampler() = default;
        /**
        * @brief Generates a random sample.
        *
        * This method should be overridden by derived classes to implement
        * specific sampling logic.
        *
        * @return A random sample as a double-precision floating-point number.
        */
        virtual double sample(){return 0.0;}// Return random double-precision sample


         /**
         * @brief Generates a distribution based on bins and sample count.
         *
         * This method should be overridden by derived classes to populate
         * the `distribution_array`.
         *
         * @param bins Number of bins in the distribution.
         * @param n_samples Number of samples to generate.
         */
        virtual void generateDistribution(int bins, int n_samples) {}

        /**
        * @brief Prints the distribution to the standard output.
        *
        * This method iterates over the `distribution_array` and prints
        * each bin's data.
        */
        virtual void printDistribution();

        /**
         * @brief Displays and saves image file of plot of pdf distribution
         *
         * @param display flag to display the plot
         * @param filesave flag to save the plot to a file
         */
        virtual void plotDistribution(bool display, bool filesave) const;

        /**
        * @brief Returns the distribution array
        */
        std::vector<double> getDistribution() const;

        /**
        * @brief Gets the minimum value of the plot range.
        *
        * This function returns the minimum value that defines the lower boundary of the plot's X-axis range.
        *
        * @return The minimum value of the plot range.
        */
        double getPlotMinimum();

       /**
        * @brief Gets the maximum value of the plot range.
        *
        * This function returns the maximum value that defines the upper boundary of the plot's X-axis range.
        *
        * @return The maximum value of the plot range.
        */
        double getPlotMaximum();

};

#endif //ABSTRACT_SAMPLER_HH
