#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cmath> // For mathematical functions
// #include "gnuplot-iostream.h"
#include "sampling.hh"
#include "statistics/statistics.hh"
#include "functions/abstract_function.h"
#include "parse_tree.h"

#ifdef OUTPUT_PATH
std::string path = std::string(OUTPUT_PATH);  // Save results.txt in the binary directory
#endif

std::vector<int> parseMomentList(const std::string& momentList);
void saveResults(const std::string& outputFile, const std::string& results);

int main() {
    // Variables to hold configuration values
    std::string functionExpression;
    std::string samplerMethod;
    int numSamples = 0;
    int histogramBins = 0;
    int randomSeed = 42; // Default: 42
    bool computeExpectation = false;
    std::vector<int> statisticalMoments;
    bool verifyCLT = false;
    std::string outputFile;

    std::ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;

        // Parse key-value pairs
        std::string key, value;
        std::istringstream lineStream(line);
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            key.erase(key.find_last_not_of(" \t") + 1); // Trim trailing spaces
            value.erase(0, value.find_first_not_of(" \t")); // Trim leading spaces

            // Map keys to variables
            try {
                if (key == "function_expression") functionExpression = value;
                else if (key == "sampler_method") samplerMethod = value;
                else if (key == "number_of_samples") {
                    numSamples = std::stoi(value); // Try to convert to int
                }
                else if (key == "histogram_bins") {
                    histogramBins = std::stoi(value); // Try to convert to int
                }
                else if (key == "random_seed") {
                    randomSeed = std::stoi(value); // Try to convert to int
                }
                else if (key == "compute_expectation_value") computeExpectation = (value == "yes");
                else if (key == "statistical_moments") statisticalMoments = parseMomentList(value);
                else if (key == "verify_central_limit_theorem") verifyCLT = (value == "yes");
                else if (key == "output_file") {
                    #ifdef OUTPUT_PATH
                                        outputFile = std::string(OUTPUT_PATH) + "/" + value;  // Combine OUTPUT_PATH and the file name
                    #else
                                        outputFile = value;  // Fallback to just the file name if OUTPUT_PATH is not defined
                    #endif
                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid input for key '" << key << "'. Expected a valid number or format for value: '" << value << "'." << std::endl;
                return 1;  // Exit with an error code
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Error: Input value for key '" << key << "' is out of range: '" << value << "'." << std::endl;
                return 1;  // Exit with an error code
            }
        }
    }

    configFile.close();

    // Parsing Mathematical Expression to create function object.
    Parser<double, double> parser(functionExpression);
    auto ast = parser.parse();
    parser.generateFunction(ast.get());
    auto& f = *parser.getFinalFunction();

    // Create Sampler
    AbstractSampler* sampler = nullptr;

    if (samplerMethod == "uniform_distribution") {
        float minVal, maxVal;

        std::cout << "Enter the minimum value for uniform distribution: ";
        std::cin >> minVal;
        std::cout << "Enter the maximum value for uniform distribution: ";
        std::cin >> maxVal;

        sampler = new UniformSampler(randomSeed, minVal, maxVal);

    } else if (samplerMethod == "box_muller_transform") {
        float mean, stddev;

        std::cout << "Enter the mean for normal distribution (Box-Muller): ";
        std::cin >> mean;
        std::cout << "Enter the standard deviation for normal distribution (Box-Muller): ";
        std::cin >> stddev;

        sampler = new NormalBoxMullerSampler(randomSeed, mean, stddev);

    } else if (samplerMethod == "inverse_transform_sampling") {
        float mean, stddev;

        std::cout << "Enter the mean for normal distribution (Inverse Transform): ";
        std::cin >> mean;
        std::cout << "Enter the standard deviation for normal distribution (Inverse Transform): ";
        std::cin >> stddev;

        sampler = new NormalInverseSampler(randomSeed, mean, stddev);

    } else {
        std::cerr << "Error: Unknown sampler method: " << samplerMethod << std::endl;
        return 1;  // Exit with error
    }

    // Create Statistics Object
    Statistics stats(*sampler, numSamples);

    // Generate Distribution
    sampler->generateDistribution(histogramBins, numSamples);

    // Results Variable
    std::ostringstream results;

    // Perform selected actions
    if (histogramBins > 0) {
        results << "######################################################\n";
        results << "Drawing histogram...\n";
        sampler->plotDistribution();
    }

    if (computeExpectation) {
        results << "######################################################\n";
        double expectation = stats.expectation(f);
        results << "Expectation value: " << expectation << "\n";
    }

    if (!statisticalMoments.empty()) {
        results << "######################################################\n";
        results << "\nComputing statistical moments...\n\n";
        for (int i = 0; i < statisticalMoments.size(); i++) {
            results << "######################################################\n";
            results << "Moment " << statisticalMoments[i] << ": " << stats.moment(statisticalMoments[i], f) << "\n";
            results << "Central Moment " << statisticalMoments[i] << ": " << stats.central_moment(statisticalMoments[i], f) << "\n";
        }
    }

    if (verifyCLT) {
        results << "######################################################\n";
        results << "Verifying Central Limit Theorem...\n";
    }

    // Save results to the output file
    if (!outputFile.empty()) {
        saveResults(outputFile, results.str());
    }

    // Print results to console as well
    std::cout << results.str();

    return 0;

}

std::vector<int> parseMomentList(const std::string& momentList) {
    std::vector<int> moments;
    std::stringstream ss(momentList);
    std::string item;
    while (std::getline(ss, item, ',')) {
        moments.push_back(std::stoi(item));
    }
    return moments;
}

void saveResults(const std::string& outputFile, const std::string& results) {
    // Open the output file in write mode
    std::ofstream outFile(outputFile);

    // Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error: Could not open file " << outputFile << " for writing!" << std::endl;
        return;
    }

    // Write the results to the file
    outFile << results;

    // Close the file
    outFile.close();

    std::cout << "Results saved to " << outputFile << std::endl;
}