//
// Created by alixb1908 on 28/11/24.
// Restructured by Alessandro Salo on 08.12.2024.
//
/**
 * @file primitive_functions.hh
 * @brief Defines and initializes all primitive function types necessary for constructing more complex functions.
 */

#ifndef PRIMITIVE_FUNCTIONS_HH
#define PRIMITIVE_FUNCTIONS_HH

#include "abstract_function.hh"
#include <cmath>
#include <stdexcept>
#include <functional>

/**
 * @brief Class representing the composition of two functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class ComposedFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs a composed function from two given functions.
     *
     * @param first The first function to apply.
     * @param second The second function to apply to the result of the first function.
     */
    ComposedFunction(const AbstractFunction<InputType, OutputType>& first, const AbstractFunction<OutputType, OutputType>& second)
        : first_(first), second_(second) {}

    /**
     * @brief Evaluates the composed function.
     *
     * @param input The input value for the function.
     * @return The output value after applying both functions.
     */
    OutputType eval(const InputType& input) const override {
        OutputType intermediate = second_.eval(input);
        return first_.eval(intermediate);
    }

private:
    const AbstractFunction<InputType, OutputType>& first_; ///< Function 1
    const AbstractFunction<OutputType, OutputType>& second_; ///< Function 2 (Argument)
};

/**
 * @brief Class representing the addition of two functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class AddFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs an addition of two functions.
     *
     * @param f The first function.
     * @param g The second function.
     */
    AddFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

    /**
     * @brief Evaluates the addition of the two functions.
     *
     * @param input The input value for the function.
     * @return The sum of the two functions' evaluations.
     */
    OutputType eval(const InputType& input) const override {
        OutputType f_result = f_.eval(input);
        OutputType g_result = g_.eval(input);

        return f_result + g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_; ///< Function 1
    const AbstractFunction<InputType, OutputType>& g_; ///< Function 2
};

/**
 * @brief Class representing the subtraction of two functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class SubtractFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs a subtraction of two functions.
     *
     * @param f The first function.
     * @param g The second function.
     */
    SubtractFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

    /**
     * @brief Evaluates the subtraction of the two functions.
     *
     * @param input The input value for the function.
     * @return The result of subtracting the second function's evaluation from the first's.
     */
    OutputType eval(const InputType& input) const override {
        OutputType f_result = f_.eval(input);
        OutputType g_result = g_.eval(input);

        return f_result - g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_; ///< Function 1
    const AbstractFunction<InputType, OutputType>& g_; ///< Function 2
};


/**
 * @brief Class representing the multiplication of two functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class MultiplyFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs a multiplication of two functions.
     *
     * @param f The first function.
     * @param g The second function.
     */
    MultiplyFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

    /**
     * @brief Evaluates the multiplication of the two functions.
     *
     * @param input The input value for the function.
     * @return The product of the two functions' evaluations.
     */
    OutputType eval(const InputType& input) const override {
        OutputType f_result = f_.eval(input);
        OutputType g_result = g_.eval(input);

        return f_result * g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_; ///< Function 1
    const AbstractFunction<InputType, OutputType>& g_; ///< Function 2
};

/**
 * @brief Class representing the division of two functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class DivideFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs a division of two functions.
     *
     * @param f The numerator function.
     * @param g The denominator function.
     */
    DivideFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

    /**
     * @brief Evaluates the division of the two functions.
     *
     * @param input The input value for the function.
     * @return The result of dividing the numerator function's evaluation by the denominator's.
     * @throws std::runtime_error If the denominator evaluates to zero.
     */
    OutputType eval(const InputType& input) const override {
        OutputType f_result = f_.eval(input);
        OutputType g_result = g_.eval(input);

        if (g_result == static_cast<OutputType>(0)) {
            throw std::runtime_error("Division by zero occurred in DivideFunction.");
        }

        return f_result / g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_; ///< Numerator function
    const AbstractFunction<InputType, OutputType>& g_; ///< Denominator function
};



/**
 * @brief Class representing an exponential function where both the base and power are functions.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class ExponentialFunction : public AbstractFunction<InputType, OutputType> {
public:

    /**
     * @brief Constructs an exponential function with a base and power, both represented as functions.
     *
     * @param baseFunction The base function to be exponentiated.
     * @param powerFunction The function representing the power to which the base is raised.
     */
    ExponentialFunction(const AbstractFunction<InputType, OutputType>& baseFunction, const AbstractFunction<InputType, OutputType>& powerFunction)
        : base_function_(baseFunction), power_function_(powerFunction) {}

    /**
     * @brief Evaluates the exponential function by raising the base function's value to the power function's value.
     *
     * @param x The input value for the function.
     * @return The result of evaluating the base function and raising it to the power determined by the power function.
     */
    OutputType eval(const InputType& x) const override {
        // Use std::pow for double power
        const auto baseValue = base_function_.eval(x);
        const auto powerValue = power_function_.eval(x);
        return static_cast<OutputType>(std::pow(baseValue, powerValue));
    }

private:
    const AbstractFunction<InputType, OutputType>& base_function_; ///< The base function being exponentiated.
    const AbstractFunction<InputType, OutputType>& power_function_;  ///< The power function providing the exponent.
};

/**
 * @brief Wrapper class that adapts a standard C++ function to an AbstractFunction object.
 *
 * This class allows you to wrap a regular C++ function (such as a lambda or function pointer)
 * and use it as an AbstractFunction object. This is particularly useful for integrating custom
 * functions into a broader framework that expects functions derived from AbstractFunction.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class FunctionWrapper : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs a FunctionWrapper from a given C++ function.
     *
     * @param func A function object, lambda, or function pointer that takes an InputType and returns an OutputType.
     */
    FunctionWrapper(const std::function<OutputType(InputType)>& func) : func_(func) {}

    /**
     * @brief Evaluates the wrapped function.
     *
     * @param input The input value for the function.
     * @return The output value produced by the wrapped function.
     */
    OutputType eval(const InputType& input) const override {
        return func_(input);
    }

private:
    std::function<OutputType(InputType)> func_{}; ///< The wrapped C++ function.
};


/**
 * @brief Class representing an identity function.
 *
 * @tparam T The type of the input and output value.
 */
template <typename T>
class IdentityFunction : public AbstractFunction<T, T> {
public:
    /**
     * @brief Evaluates the identity function.
     *
     * @param input The input value for the function.
     * @return The same value as the input.
     */
    T eval(const T& input) const override {
        return input;
    }
};

/**
 * @brief Class representing a negative identity function.
 *
 * @tparam T The type of the input and output value.
 */
template <typename T>
class NegativeIdentityFunction : public AbstractFunction<T, T> {
public:
    /**
     * @brief Evaluates the negative of identity function.
     *
     * @param input The input value for the function.
     * @return The same value as the input.
     */
    T eval(const T& input) const override {
        return -1*input;
    }
};

/**
 * @brief Class representing a constant function.
 *
 * @tparam T The type of the value.
 */
template <typename T>
class ConstantFunction : public AbstractFunction<T, T> {
    public:
         /**
         * @brief Constructs a constant function.
         *
         * @param value The constant value that the function represents.
         */
        explicit ConstantFunction(T input) : constant(input) {}

        /**
         * @brief Evaluates the constant function.
         *
         * @param input The input value for the function (not used).
         * @return The constant value.
         */
        T eval(const T& input) const override {
            return constant;
        }

        T eval() const{
            return constant;
        }

        T getConstant() const {
            return constant;
        }
    private:
        T constant; ///< The constant value of the function.
};

#endif // PRIMITIVE_FUNCTIONS_HH
