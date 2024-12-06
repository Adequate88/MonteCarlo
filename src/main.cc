#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cmath> // For mathematical functions
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

    return 0;


}
