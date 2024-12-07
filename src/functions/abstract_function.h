//
// Created by alixb1908 on 28/11/24.
//
/**
 * @file abstract_function.h
 * @brief Defines abstract and concrete function classes for use in mathematical computations.
 *
 * This file contains the definition of the AbstractFunction class, along with various concrete
 * derived classes that represent different types of functions, including composed, added,
 * multiplied, exponential, constant, and identity functions.
 */
#ifndef ABSTRACT_FUNCTION_HH
#define ABSTRACT_FUNCTION_HH

#include <memory>
#include <limits>
#include <stdexcept>
#include <functional>
#include <cmath>



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

        if ((f_result > 0 && g_result > std::numeric_limits<OutputType>::max() - f_result) ||
            (f_result < 0 && g_result < std::numeric_limits<OutputType>::lowest() - f_result)) {
            //throw std::overflow_error("Overflow occurred during addition.");
        }

        return f_result + g_result;
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

        if (f_result != 0 && (g_result > std::numeric_limits<OutputType>::max() / f_result ||
            g_result < std::numeric_limits<OutputType>::lowest() / f_result)) {
            //throw std::overflow_error("Overflow occurred during multiplication.");
        }

        return f_result * g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_; ///< Function 1
    const AbstractFunction<InputType, OutputType>& g_; ///< Function 2
};


/**
 * @brief Class representing an exponential function.
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */template <typename InputType, typename OutputType>
class ExponentialFunction : public AbstractFunction<InputType, OutputType> {
public:
    /**
     * @brief Constructs an exponential function with an integer power.
     *
     * @param baseFunction The base function to be raised to a power.
     * @param power The integer power.
     */
    ExponentialFunction(const AbstractFunction<InputType, OutputType>& baseFunction, int power)
        : baseFunction_(baseFunction), power_(static_cast<double>(power)), isIntegerPower_(true) {}

    /**
     * @brief Constructs an exponential function with a double power.
     *
     * @param baseFunction The base function to be raised to a power.
     * @param power The double power.
     */
    ExponentialFunction(const AbstractFunction<double, double>& baseFunction, double power)
        : baseFunction_(baseFunction), power_(power), isIntegerPower_(false) {}

    /**
     * @brief Evaluates the exponential function.
     *
     * @param x The input value for the function.
     * @return The result of the base function raised to the specified power.
     */
    OutputType eval(const InputType& x) const override {
        if (isIntegerPower_) {
            // Use repeated multiplication for integer power
            if (static_cast<int>(power_) == 1) {
                return baseFunction_.eval(x);
            }
            OutputType baseValue = baseFunction_.eval(x);
            OutputType result = baseFunction_.eval(x);
            for (int i = 0; i < static_cast<int>(power_); ++i) {
                result *= baseValue;
            }
            return result;
        }
        // Use std::pow for double power
        const double baseValue = baseFunction_.eval(static_cast<double>(x));
        return static_cast<OutputType>(std::pow(baseValue, power_));
    }

private:
    const AbstractFunction<InputType, OutputType>& baseFunction_; ///< The base function being exponentiated.
    double power_; ///< The power to which the base function is raised.
    bool isIntegerPower_; ///< Flag to indicate if the power is an integer or a floating point number.
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
    private:
        T constant; ///< The constant value of the function.
};

#endif // ABSTRACT_FUNCTION_HH
