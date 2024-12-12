# Monte Carlo Statistics ReadMe

## Table of Contents

1. [Project Overview](#project-overview)
2. [How to Compile the Program](#how-to-compile-the-program)
    - [Prerequisites](#prerequisites)
    - [Compilation Steps](#compilation-steps)
    - [External Libraries](#external-libraries)
3. [Usage Instructions](#usage-instructions)
    - [Running the Program](#running-the-program)
    - [Configuration File Inputs](#configuration-file-inputs)
    - [Mathematical Expression Syntax](#mathematical-expression-syntax)
4. [Features](#features)
    - [Sampling Module](#sampling-module)
    - [Statistics Module](#statistics-module)
    - [Central Limit Theorem Tester](#central-limit-theorem-tester)
    - [Functions Module](#functions-module)
5. [Test Descriptions, Accuracy Thresholds, and Expected Outcomes](#test-descriptions-accuracy-thresholds-and-expected-outcomes)
6. [Acknowledgments](#acknowledgments)
7. [TODOs and Perspectives](#todos-and-perspectives)

---

## Project Overview

This project provides a modular framework for sampling, statistical analysis, and testing mathematical properties such as the Central Limit Theorem. It includes:

- Sampling distributions (e.g., uniform, normal, and Box-Muller transforms).
- A mathematical expression parser
- Statistical computations such as expectation, variance, and moments.
- Distribution Plotting and saving in build file
- A Central Limit Theorem tester.

Detailed code documentation is available by navigating to the `\html` directory and opening the `index.html` file. This will launch the Doxygen-generated documentation platform.

## How to Compile the Program

### Prerequisites

- A C++17-compatible compiler (e.g., GCC, Clang, or MSVC).
- CMake installed.

### Compilation Steps

1. **Navigate to the root directory of the project.**

2. **Compile using CMake:**

   Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```

   Generate build files with CMake:
   ```bash
   cmake ..
   ```
   
   Build the project:
   ```bash
   cmake --build .
   ```

3. **Alternatively, compile directly with g++ (if preferred):**

   Navigate to the root directory of the project:
   ```bash
   g++ -std=c++17 -o main src/sampling/*.cc src/statistics/*.cc src/functions/*.cc src/clt_tester/clt_tester.cc
   ```
   
### External Libraries

Ensure that the required external libraries (like boost) are installed in your system and that the submodules are updated. To perform the latter, run the following in the root directory:
```bash
git submodule update --init
```
This will ensure that the `gnuplot-iostream` and `googletest` directories are pulled and updated.



---

## Usage Instructions

### Running the Program

Once the program is compiled, follow these steps using the provided configuration file (config.txt):

1. Configure the config.txt file with all desired values (see below for description of parameters).
2. Run the executable.
3. Depending on the chosen sampler, you will be prompted to enter your desired parameters in the terminal.
4. The program will execute. The results based on the input you provided in the configuration will be output in the terminal and in a file (name of your choosing) in the root directory

### Configuration File Inputs

1. **`function_expression`**: Define the mathematical function `f(x)` to evaluate (e.g., `sin(x) + x^2`).
2. **`sampler_method`**: Choose the random number generator method (`inverse_transform_sampling`, `box_muller_transform`, `uniform_distribution`).
3. **`number_of_samples`**: Specify the number of random samples to generate.
4. **`random_seed`**: Set an integer seed for the random number generator (optional).
5. **`histogram_bins`**: Set the number of bins for the histogram (0 to disable).
6. **`compute_expectation_value`**: Set to `yes` to compute the expectation value of `f(x)`.
7. **`statistical_moments`**: Provide a list of statistical moments to compute (e.g., `1,2,3` for mean, variance, and skewness).
8. **`verify_central_limit_theorem`**: Set to `yes` to verify the Central Limit Theorem.
9. **`output_file`**: Specify the output file name for saving results.

### Mathematical Expression Syntax

The `function_expression` field in the configuration file accepts mathematical functions written in valid syntax. Here are the rules and supported operations:

1. **Basic Arithmetic Operations**:
    - Addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), and exponentiation (`^`).
    - Example: `x^2 + 3*x - 5`
    - Invalid Example: `3x - 5`

2. **Available Mathematical Functions**:
    - Trigonometric functions: `sin(x)`, `cos(x)`, `tan(x)`.
    - Exponential and logarithmic functions: `exp(x)`, `log(x)` (natural logarithm).

3. **Parentheses**:
    - Use parentheses `()` to indicate the order of operations and grouping.
    - Example: `(x + 2)^3`

4. **Variables**:
    - The variable `x` is used to define the input of the function.
    - Example: `x^3 + 2*x + 1`

6. **Combination of Functions and Operations**:
    - Combine any of the above functions and operators to define complex expressions.
    - Example: `sin(x) + x^2 * log(x)`


Ensure that all expressions follow proper syntax, including matching parentheses and valid function names. Unsupported functions or incorrect syntax will result in an error during execution.

---

## Features

### Sampling Module

Classes in this module are based on an AbstractSampler class. This abstract class provides the basic framework common to all other sampling classes, such as:

- `sample()` method to obtain a random value.
- `generateDistribution(int bins, int n_samples)` method to generate the probability density function of the sampler. This is then stored into a `std::vector<double>`.
- `printDistribution()` and `plotDistribution()` to visualize the distribution generated.

The currently available sampling methods found here are:
- **Uniform Distribution** Sampling (`UniformSampler`): Algorithm obtained from "Numerical Recipes"
- **Normal Distribution** (with parent class `NormalSampler`):
  - Box-Muller Transform (`NormalBoxMullerSampler`): Generating pairs of independent, normally distributed random variables from uniformly distributed random variables. Involves:
      1. Generate two independent random variables, `u1` and `u2`, uniformly distributed between 0 and 1.
      2. Apply the Box-Muller formulas to convert these variables into two independent random variables with a standard normal distribution (mean 0, variance 1):
         - z0 = sqrt(-2 * ln(u1)) * cos(2 * pi * u2)
         - z1 = sqrt(-2 * ln(u1)) * sin(2 * pi * u2) 
      3. By scaling and shifting (by mean/mu and standard deviation/sigma respectively), these can be adjusted to any desired mean and variance.
  - **Inverse Transform** (`NormalInverseSampler`): Generating normally distributed random variables using the inverse cumulative distribution function (CDF). Involves:
      1. Generate a uniform random variable, `u`, uniformly distributed between 0 and 1.
      2. Apply the inverse CDF (also known as the quantile function) to convert `u` into a value from a standard normal distribution (mean 0, variance 1).
      3. By scaling and shifting (by mean/mu and standard deviation/sigma respectively), the result can be adjusted to any desired mean and variance.

### Statistics Module

By taking in an AbstractSampler object, this module will compute the desired statistics whilst sampling from that sampler. The supported statistical methods take in a user-defined AbstractFunction object to compute:

- **Expectation**: Sums all obtained samples (after applying function `f(x)`) and then divides by number of samples
- **Variance**: Computes expectation of the `f(x)`. Then, for each value in data, the squared difference from the expectation is summed. The total is divided by number of samples.
- **Nth Moment**: Function `f(x)` is applied to each sample, and the difference from the mean is raised to the Nth power. The moments are computed by summing these values and dividing by number of samples.
- **Nth Central Moment**: Unlike the regular moment, this does not subtract the mean before raising to the Nth power. The sum of the values raised to the power is computed and divided by number of samples.

### Central Limit Theorem Tester (Included as an AbstractSampler Child)

The CLT Tester module validates the Central Limit Theorem (CLT) for a given sampler and statistical function by analyzing the distribution of sample means.

- **Sampling and Means**: Computes means from repeated samples using an `AbstractSampler` and a user-defined `AbstractFunction`.
- **Histogram Generation**: `generateDistribution(int bins, int n_samples)` creates a frequency distribution of sample means, normalized to a probability density function.
- **CLT Validation**: One can plot the obtained distribution histogram to verify that a normal distribution has been obtained.

### Functions Module

This template based module is comprised of two parts: the AbstractFunction class, meant to hold and represent functions to use in our statistics calculations; and a Parser class, capable of breaking down a string mathematical expression into its primitive functions and then constructing an AbstractFunction object to represent that expression.

1. **AbstractFunction Class**:
   - Serves as the base class for different types of mathematical functions.
   - Defines the `eval` method as a pure virtual function, requiring derived classes to implement their own evaluation logic.
   - Supports function composition, addition, multiplication, and exponentiation through overloaded operators (`|`, `+`, `*`, `^`).
   - **Children Function Classes**:
     - `ComposedFunction`: Represents composition of two functions.
     - `AddFunction`: Represents the addition of two functions.
     - `MultiplyFunction`: Represents the multiplication of two functions.
     - `ExponentialFunction`: Represents exponentiation of a function (supports both integer and floating-point powers).
     - `IdentityFunction`: Represents the identity function (returns the input as output).
     - `ConstantFunction`: Represents a constant function (always returns the same value).
     -  `FunctionWrapper`: Allows wrapping of standard C++ functions (e.g., lambdas, function pointers) as `AbstractFunction`.
2. **Parser Class**
     - **Tokenization**: Converts the input string into tokens representing numbers, operators, and functions.
     - **Parsing**: Builds the AST by respecting operator precedence (e.g., addition, multiplication, parentheses).
     - **Function Generation**: Generates callable functions from the AST nodes (e.g., `sin`, `cos`, operators like `+`, `*`).
     - **Final Function**: Allows retrieval of the final generated function after parsing and function generation.

---

## Test Descriptions, Accuracy Thresholds, and Expected Outcomes

1. `Uniform.ConsitencyCheck`
   - **Purpose**: Ensures samplers initialized with the same seed produce identical sequences of samples.
   - **Accuracy**: The samplers must match exactly on all 1000 samples.
   - **Expected Outcome**: The two samplers must produce the same sample values for each draw.

2. `Uniform.RangeCheck`
   - **Purpose**: Ensures uniform samplers generate samples within specified ranges.
   - **Accuracy**: No samples should fall outside the defined bounds.
   - **Expected Outcome**:
       - [0, 1] range: Samples between 0 and 1.
       - [-2, 4] range: Samples between -2 and 4.
       - [-0.0001, 0.001] range: Samples between -0.0001 and 0.001.

3. `Uniform.StatisticCheck`
   - **Purpose**: Validates statistical properties of uniform distributions.
   - **Accuracy Threshold**: 1% tolerance for statistical calculations (`epsilon = 1E-2`).
   - **Expected Outcome**:
       - Expectation close to mean `(max + min) / 2`.
       - Variance close to `(max - min)² / 12`.
       - Third moment near 0.
       - Central moment calculations close to expected values.


4. `Uniform.PDFCheck`
   - **Purpose**: Insures that PDF sums to 1.
   - **Accuracy Threshold**: 1 unit tolerance for statistical calculations (`epsilon = e-2`).


5. `NormalInverse.Consistency`
   - **Purpose**: Ensures normal inverse samplers with the same seed produce identical sequences.
   - **Accuracy**: The samplers must match exactly on all 1000 samples.
   - **Expected Outcome**: Identical sample sequences between samplers.

6. `NormalInverse.StatisticCheck`
   - **Purpose**: Validates statistical properties of normal distributions.
   - **Accuracy Threshold**: 1 unit tolerance for statistical calculations (`epsilon = 1`).
   - **Expected Outcome**:
       - Expectation close to `mu`.
       - Variance close to `sigma^2`.
       - Third moment near 0.
       - Central and fourth moments close to theoretical values.

7. `NormalInverse.PDFCheck`
   - **Purpose**: Insures that PDF sums to 1.
   - **Accuracy Threshold**: 1 unit tolerance for statistical calculations (`epsilon = e-2`).


8. `NormalBoxMuller.Consistency`
   - **Purpose**: Ensures Box-Muller normal samplers with the same seed produce identical sequences.
   - **Accuracy**: The samplers must match exactly on all 1000 samples.
   - **Expected Outcome**: Identical sample sequences between samplers.

9. `NormalBoxMuller.StatisticCheck`
   - **Purpose**: Validates statistical properties of normal distributions using the Box-Muller method.
   - **Accuracy Threshold**: 1 unit tolerance for statistical calculations (`epsilon = 1`).
   - **Expected Outcome**: Statistical properties (mean, variance, moments) should match expected values for the specified `mu` and `sigma`.

10. `NormalBoxMuller.PDFCheck`
   - **Purpose**: Insures that PDF sums to 1.
   - **Accuracy Threshold**: 1 unit tolerance for statistical calculations (`epsilon = e-2`).


11. `ParserTest.TokenizeSimpleExpression`
   - **Purpose**: Validates the tokenization of a simple arithmetic expression.
   - **Accuracy**: The tokenization process must correctly identify numbers, operators, and the end of the expression.
   - **Expected Outcome**: The tokenizer should output four tokens: two numbers, one operator, and one END token.

12. `ParserTest.ParseSimpleExpression`
    - **Purpose**: Verifies that a simple arithmetic expression is correctly parsed into an Abstract Syntax Tree (AST).
    - **Accuracy**: The AST must contain an operator node with two children, both numeric values.
    - **Expected Outcome**: The AST should be parsed with a root operator node (`+`) and two numeric children.

13. `ParserTest.GenerateFunction`
    - **Purpose**: Ensures that a parsed expression is correctly transformed into a functional form.
    - **Accuracy**: The resulting function must evaluate the expression correctly for a given input.
    - **Expected Outcome**: The function should evaluate to approximately `22045.7` when `x = 5.0`.

14. `MonteCarloTest.UniformSamplerTest`
    - **Purpose**: Tests the statistical properties (expectation, variance, 3rd and 4th moments) of the function `3*x + 2` sampled from a Uniform distribution between 0 and 1. Additionally checks that file is created correctly.
    - **Accuracy**: The sample statistics must match the theoretical values of expectation, variance, and higher moments.
    - **Expected Outcome**:
        - Expectation should be close to `3.5`.
        - Variance should be close to `0.75`.
        - The 3rd moment should be approximately `50.75`.
        - The 4th moment should be approximately `206.2`.

15. `MonteCarloTest.NormalBoxMullerTest`
    - **Purpose**: Tests the statistical properties (expectation, variance, 3rd and 4th moments) of the function `3*x + 2` sampled from a Normal distribution with `mu = 0` and `sigma = 1`. Additionally checks that file is created correctly.
    - **Accuracy**: The sample statistics must match the theoretical values of expectation, variance, and higher moments.
    - **Expected Outcome**:
        - Expectation should be close to `2`.
        - Variance should be close to `9`.
        - The 3rd moment should be approximately `62`.
        - The 4th moment should be approximately `475`.

16. `MonteCarloTest.NormalInverseTest`
    - **Purpose**: Tests the statistical properties (expectation, variance, 3rd and 4th moments) of the function `3*x + 2` sampled from a Normal distribution with `mu = 0` and `sigma = 1`. Additionally checks that file is created correctly.
    - **Accuracy**: The sample statistics must match the theoretical values of expectation, variance, and higher moments.
    - **Expected Outcome**:
        - Expectation should be close to `2`.
        - Variance should be close to `9`.
        - The 3rd moment should be approximately `62`.
        - The 4th moment should be approximately `475`.


---

## Acknowledgments

- **GoogleTest**: Thanks to the developers of GoogleTest for providing a testing framework. [Repository](https://github.com/google/googletest).
- **gnuplot-iostream**: Thanks to the creators of gnuplot-iostream. [Repository](https://github.com/dstahlke/gnuplot-iostream).
- **Numerical Recipes**: Algorithms in this project are inspired by and taken from *Numerical Recipes* (Press et al., 2007).

---

## TODOs and Perspectives

### TODOs

- Implement more samplers that handle different types of distributions (e.g. Exponential, Binomial, Poisson, etc.)
- Extend the parser to support further mathematical functions, such as hyperbolic trigonometric functions and special functions (e.g., gamma, beta).
- Integrate a GUI for easier configuration and visualization of results.
- Add support for multivariate distributions and corresponding statistical computations.
- Include additional tests for edge cases, such as handling undefined mathematical expressions or invalid configuration files.

### Perspectives

- Apply the Monte Carlo program to simulations in financial and physical systems.
- Extend to a large-scale statistics program: generating all properties (PDF, CDF, etc.) of discrete and continuous of a distribution. Capable of handling multiple variables and generating all statistical properties.

