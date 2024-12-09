#include <clt_tester.hh>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cmath> // For mathematical functions

#include "gnuplot-iostream.h"
#include "sampling.hh"
#include "statistics.hh"
#include "abstract_function.hh"
#include "parse_tree.hh"

#ifdef OUTPUT_PATH
/// If OUTPUT_PATH is defined, set the path where results will be saved (e.g., binary directory).
std::string path = std::string(OUTPUT_PATH);
#endif

/// Function declarations
/// Parses a comma-separated list of statistical moments.
/// @param momentList A string representing the list of moments (e.g., "1,2,3").
/// @return A vector of integers representing the parsed moments.
std::vector<int> parseMomentList(const std::string& momentList);

/// Saves the results to a specified output file.
/// @param outputFile The path of the output file.
/// @param results The string containing the results to be saved.
void saveResults(const std::string& outputFile, const std::string& results);

int main() {
    // Variables to hold configuration values from the config file
    std::string functionExpression; ///< Mathematical function expression (e.g., "x^2 + 3*x")
    std::string samplerMethod; ///< The sampling method (e.g., "uniform_distribution")
    int numSamples = 0;            ///< Number of samples to generate
    int histogramBins = 0;         ///< Number of bins for histogram
    int randomSeed = 42;           ///< Default random seed value (42)
    bool computeExpectation = false;  ///< Flag to compute expectation value
    std::vector<int> statisticalMoments;  ///< Vector to store statistical moments
    bool verifyCLT = false;        ///< Flag to verify Central Limit Theorem (CLT)
    int cltsamples = 0;
    std::string outputFile;        ///< Output file for saving results

    /// Open the configuration file "config.txt"
    std::ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file!" << std::endl;
        return 1;  ///< Exit with error code 1 if file cannot be opened
    }

    /// Read and parse each line in the configuration file
    std::string line;
    while (std::getline(configFile, line)) {
        // Skip empty lines and comments (lines starting with '#')
        if (line.empty() || line[0] == '#') continue;

        // Parse key-value pairs in the format "key=value"
        std::string key, value;
        std::istringstream lineStream(line);
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            // Trim whitespace from key and value
            key.erase(key.find_last_not_of(" \t") + 1); // Remove trailing spaces
            value.erase(0, value.find_first_not_of(" \t")); // Remove leading spaces

            /// Map each key to its corresponding variable
            try {
                if (key == "function_expression") functionExpression = value;
                else if (key == "sampler_method") samplerMethod = value;
                else if (key == "number_of_samples") {
                    numSamples = std::stoi(value);  ///< Convert to int
                }
                else if (key == "histogram_bins") {
                    histogramBins = std::stoi(value);  ///< Convert to int
                }
                else if (key == "random_seed") {
                    randomSeed = std::stoi(value);  ///< Convert to int
                }
                else if (key == "compute_expectation_value") computeExpectation = (value == "yes");
                else if (key == "statistical_moments") statisticalMoments = parseMomentList(value);  ///< Parse moment list
                else if (key == "verify_central_limit_theorem") verifyCLT = (value == "yes");
                else if (key == "number_of_sample_means") {
                    cltsamples = std::stoi(value);  ///< Convert to int
                }
                else if (key == "output_file") {
                    #ifdef OUTPUT_PATH
                    outputFile = std::string(OUTPUT_PATH) + "/" + value;  ///< Combine OUTPUT_PATH with the file name
                    #else
                    outputFile = value;  ///< Use just the file name if OUTPUT_PATH is not defined
                    #endif
                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid input for key '" << key << "'. Expected a valid number or format for value: '" << value << "'." << std::endl;
                return 1;  ///< Exit with error code if conversion fails
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Error: Input value for key '" << key << "' is out of range: '" << value << "'." << std::endl;
                return 1;  ///< Exit with error code if input is out of range
            }
        }
    }

    configFile.close();  ///< Close the configuration file after reading

    /// Parse the mathematical expression to create a function object using the Parser
    Parser<double, double> parser(functionExpression);
    auto ast = parser.parse();            ///< Parse the expression into an abstract syntax tree (AST)
    parser.generateFunction(ast.get());  ///< Generate the function from the AST
    auto& f = *parser.getFinalFunction(); ///< Get the final function

    // Create Sampler based on the selected method
    AbstractSampler* sampler = nullptr;
    if (samplerMethod == "uniform_distribution") {
        // Uniform distribution sampler: Ask user for min and max values
        float minVal, maxVal;
        std::cout << "Enter the minimum value for uniform distribution: ";
        std::cin >> minVal;
        std::cout << "Enter the maximum value for uniform distribution: ";
        std::cin >> maxVal;

        sampler = new UniformSampler(randomSeed, minVal, maxVal);  ///< Create UniformSampler instance
    } else if (samplerMethod == "box_muller_transform") {
        // Box-Muller transform: Normal distribution sampler
        float mean, stddev;
        std::cout << "Enter the mean for normal distribution (Box-Muller): ";
        std::cin >> mean;
        std::cout << "Enter the standard deviation for normal distribution (Box-Muller): ";
        std::cin >> stddev;

        sampler = new NormalBoxMullerSampler(randomSeed, mean, stddev);  ///< Create NormalBoxMullerSampler instance
    } else if (samplerMethod == "inverse_transform_sampling") {
        // Inverse transform sampling: Normal distribution sampler
        float mean, stddev;
        std::cout << "Enter the mean for normal distribution (Inverse Transform): ";
        std::cin >> mean;
        std::cout << "Enter the standard deviation for normal distribution (Inverse Transform): ";
        std::cin >> stddev;

        sampler = new NormalInverseSampler(randomSeed, mean, stddev);  ///< Create NormalInverseSampler instance
    } else {
        std::cerr << "Error: Unknown sampler method: " << samplerMethod << std::endl;
        return 1;  ///< Exit with error code if an unknown sampler method is specified
    }

    // Create Statistics object using the selected sampler and number of samples
    Statistics stats(*sampler, numSamples);

    // Generate Distribution using the sampler
    sampler->generateDistribution(histogramBins, numSamples);

    // Variable to store the results
    std::ostringstream results;

    // Perform selected actions based on user input
    if (histogramBins > 0) {
        results << "######################################################\n";
        results << "Drawing histogram...\n";
        sampler->plotDistribution();  ///< Plot the distribution as a histogram
    }

    if (computeExpectation) {
        results << "######################################################\n";
        double expectation = stats.expectation(f);  ///< Compute expectation value
        results << "Expectation value: " << expectation << "\n";
    }

    if (!statisticalMoments.empty()) {
        results << "######################################################\n";
        results << "\nComputing statistical moments...\n\n";
        for (int i = 0; i < statisticalMoments.size(); i++) {
            results << "######################################################\n";
            results << "Moment " << statisticalMoments[i] << ": " << stats.moment(statisticalMoments[i], f) << "\n";  ///< Compute raw moment
            results << "Central Moment " << statisticalMoments[i] << ": " << stats.central_moment(statisticalMoments[i], f) << "\n";  ///< Compute central moment
        }
    }

    if (verifyCLT) {
        results << "######################################################\n";
        results << "Verifying Central Limit Theorem...\n";
        std::cout << "Verifying Central Limit Theorem...\n" << std::endl;
        CltTester clt(*sampler, f, numSamples);
        clt.generateDistribution(histogramBins, cltsamples);
        clt.test();  ///< Test the Central Limit Theorem
    }

    // Save results to the output file, if defined
    if (!outputFile.empty()) {
        saveResults(outputFile, results.str());
    }

    // Print the results to the console
    std::cout << results.str();

    return 0;  ///< Exit successfully
}

/// Parses a comma-separated list of statistical moments (e.g., "1,2,3")
/// @param momentList A string representing the list of moments (e.g., "1,2,3").
/// @return A vector of integers representing the parsed moments.
std::vector<int> parseMomentList(const std::string& momentList) {
    std::vector<int> moments;
    std::stringstream ss(momentList);
    std::string item;
    while (std::getline(ss, item, ',')) {
        moments.push_back(std::stoi(item));  ///< Convert each item to an integer and store in the moments vector
    }
    return moments;
}

/// Saves results to an output file
/// @param outputFile The path of the output file.
/// @param results The string containing the results to be saved.
void saveResults(const std::string& outputFile, const std::string& results) {
    /// Open the output file in write mode
    std::ofstream outFile(outputFile);

    /// Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputFile << " for writing!" << std::endl;
        return;
    }

    /// Write the results to the file
    outFile << results;

    /// Close the file
    outFile.close();

    std::cout << "Results saved to " << outputFile << std::endl;  ///< Inform the user that results were saved
}
