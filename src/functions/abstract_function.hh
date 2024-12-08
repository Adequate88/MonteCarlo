//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file abstract_function.hh
 * @brief Defines abstract and concrete function classes for use in mathematical computations.
 *
 * This file contains the definition of the AbstractFunction class, along with various concrete
 * derived classes that represent different types of functions, including composed, added,
 * multiplied, exponential, constant, and identity functions.
 */
#ifndef ABSTRACT_FUNCTION_HH
#define ABSTRACT_FUNCTION_HH

#include <memory>

// Forward declarations for ComposedFunction and AddFunction
template <typename InputType, typename OutputType>
class ComposedFunction;

template <typename InputType, typename OutputType>
class AddFunction;

template <typename InputType, typename OutputType>
class MultiplyFunction;

template <typename InputType, typename OutputType>
class ExponentialFunction;

template <typename T>
class ConstantFunction;

/**
 * @brief Abstract base class for representing a function with specified input and output types.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class AbstractFunction {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~AbstractFunction() = default;

    /**
     * @brief Pure virtual method to evaluate the function.
     *
     * @param input The input value for the function.
     * @return The output value of the function.
     */
    virtual OutputType eval(const InputType& input) const = 0;

    /**
     * @brief Method to compose two functions.
     *
     * @param other The function to compose with.
     * @return A unique pointer to the composed function.
     */
    std::unique_ptr<AbstractFunction<InputType, OutputType>> compose(const AbstractFunction<OutputType, OutputType>& other) const {
        return std::make_unique<ComposedFunction<InputType, OutputType>>(*this, other);
    }

    /**
     * @brief Overload the '|' operator to compose two functions.
     *
     * @param other The function to compose with.
     * @return A unique pointer to the composed function.
     */
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator|(const AbstractFunction<OutputType, OutputType>& other) const {
        return std::make_unique<ComposedFunction<InputType, OutputType>>(*this, other);
    }

    /**
     * @brief Overload the '+' operator to add two functions.
     *
     * @param other The function to add.
     * @return A unique pointer to the resulting function.
     */
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator+(const AbstractFunction<InputType, OutputType>& other) const {
        return std::make_unique<AddFunction<InputType, OutputType>>(*this, other);
    }

    /**
     * @brief Overload the '*' operator to multiply two functions.
     *
     * @param other The function to multiply.
     * @return A unique pointer to the resulting function.
     */
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator*(const AbstractFunction<InputType, OutputType>& other) const {
        return std::make_unique<MultiplyFunction<InputType, OutputType>>(*this, other);
    }

    /**
     * @brief Overload the '^' operator to create an exponential function with an integer power.
     *
     * @param power The integer power to raise the function to.
     * @return A unique pointer to the resulting exponential function.
     */
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator^(int power) const {
        return std::make_unique<ExponentialFunction<InputType, OutputType>>(*this, power);
    }

    /**
     * @brief Overload the '^' operator to create an exponential function with a double power.
     *
     * @param power The double power to raise the function to.
     * @return A unique pointer to the resulting exponential function.
     */
    std::unique_ptr<AbstractFunction<double, double>> operator^(double power) const {
        return std::make_unique<ExponentialFunction<double, double>>(*this, power);
    }
};
#endif // ABSTRACT_FUNCTION_HH
