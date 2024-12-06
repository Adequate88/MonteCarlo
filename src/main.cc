#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cmath> // For mathematical functions
#include "gnuplot-iostream.h"
#include "sampling.hh"
#include "statistics/statistics.hh"
#include "functions/abstract_function.h"
#include "parse_tree.h"

int main() {

    std::string expr = "exp(4*x) + cos(2) + sin(3+x)*5";
    Parser<double, double> parser(expr);
    auto ast = parser.parse();
    parser.generateFunction(ast.get());

    auto final = parser.getFinalFunction();

    //parser.printFunctionMemory();
    //parser.printFunctionStack();

    double testValue = 2;
    double result = final->eval(testValue);

    std::cout << result << std::endl;

    //Testing gnuplot
    Gnuplot gp;

    // Create sample data for plotting (sine wave)
    std::vector<std::pair<double, double>> data;
    for(double x = 0; x < 10; x += 0.1) {
        data.emplace_back(x, std::sin(x));
    }

    // Plot the data using Gnuplot
    gp << "set title 'Sine Wave'\n";
    gp << "plot '-' with lines title 'sin(x)'\n";
    gp.send1d(data);

    std::cout << "Plotting complete. Press enter to exit..." << std::endl;
    std::cin.get(); // Wait for user input before closing

    return 0;


}
