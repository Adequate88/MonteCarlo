//
// Created by alixb1908 on 28/11/24.
//

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

// Abstract base class for representing a function with specified input and output types
template <typename InputType, typename OutputType>
class AbstractFunction {
public:
    // Virtual destructor
    virtual ~AbstractFunction() = default;

    // Pure virtual method to evaluate the function
    virtual OutputType eval(const InputType& input) const = 0;

    // Method to compose two functions
    std::unique_ptr<AbstractFunction<InputType, OutputType>> compose(const AbstractFunction<OutputType, OutputType>& other) const {
        return std::make_unique<ComposedFunction<InputType, OutputType>>(*this, other);
    }

    // Overload the '|' operator to compose two functions
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator|(const AbstractFunction<OutputType, OutputType>& other) const {
        return std::make_unique<ComposedFunction<InputType, OutputType>>(*this, other);
    }

    // Overload the '+' operator to add two functions
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator+(const AbstractFunction<InputType, OutputType>& other) const {
        return std::make_unique<AddFunction<InputType, OutputType>>(*this, other);
    }

    // Overload the '*' operator to multiply two functions
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator*(const AbstractFunction<InputType, OutputType>& other) const {
        return std::make_unique<MultiplyFunction<InputType, OutputType>>(*this, other);
    }

    // Overload the '^' operator to create an exponential function with an integer power
    // (not to be confused with the bitwise XOR operator)
    std::unique_ptr<AbstractFunction<InputType, OutputType>> operator^(int power) const {
        return std::make_unique<ExponentialFunction<InputType, OutputType>>(*this, power);
    }

    // Overload the '^' operator to create an exponential function with a double power
    std::unique_ptr<AbstractFunction<double, double>> operator^(double power) const {
        return std::make_unique<ExponentialFunction<double, double>>(*this, power);
    }
};

// Separate class for composing two functions
template <typename InputType, typename OutputType>
class ComposedFunction : public AbstractFunction<InputType, OutputType> {
public:
    ComposedFunction(const AbstractFunction<InputType, OutputType>& first, const AbstractFunction<OutputType, OutputType>& second)
        : first_(first), second_(second) {}

    OutputType eval(const InputType& input) const override {
        OutputType intermediate = second_.eval(input);
        return first_.eval(intermediate);
    }

private:
    const AbstractFunction<InputType, OutputType>& first_;
    const AbstractFunction<OutputType, OutputType>& second_;
};

// Separate class for adding two functions
template <typename InputType, typename OutputType>
class AddFunction : public AbstractFunction<InputType, OutputType> {
public:
    AddFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

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
    const AbstractFunction<InputType, OutputType>& f_;
    const AbstractFunction<InputType, OutputType>& g_;
};

// Separate class for multiplying two functions
template <typename InputType, typename OutputType>
class MultiplyFunction : public AbstractFunction<InputType, OutputType> {
public:
    MultiplyFunction(const AbstractFunction<InputType, OutputType>& f, const AbstractFunction<InputType, OutputType>& g)
        : f_(f), g_(g) {}

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
    const AbstractFunction<InputType, OutputType>& f_;
    const AbstractFunction<InputType, OutputType>& g_;
};


// Exponential Function Class with Overloaded Constructors
template <typename InputType, typename OutputType>
class ExponentialFunction : public AbstractFunction<InputType, OutputType> {
public:
    // Constructor for integer power (using repeated multiplication)
    ExponentialFunction(const AbstractFunction<InputType, OutputType>& baseFunction, int power)
        : baseFunction_(baseFunction), power_(static_cast<double>(power)), isIntegerPower_(true) {}

    // Constructor for double power (using std::pow)
    ExponentialFunction(const AbstractFunction<double, double>& baseFunction, double power)
        : baseFunction_(baseFunction), power_(power), isIntegerPower_(false) {}

    // Evaluate the function at a given x value
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
    const AbstractFunction<InputType, OutputType>& baseFunction_;
    double power_;
    bool isIntegerPower_;
};

// FunctionWrapper class that wraps a regular C++ function and creates an AbstractFunction object
template <typename InputType, typename OutputType>
class FunctionWrapper : public AbstractFunction<InputType, OutputType> {
public:
    FunctionWrapper(const std::function<OutputType(InputType)>& func) : func_(func) {}

    OutputType eval(const InputType& input) const override {
        return func_(input);
    }

private:
    std::function<OutputType(InputType)> func_{};
};

// Identity function class
template <typename T>
class IdentityFunction : public AbstractFunction<T, T> {
public:
    T eval(const T& input) const override {
        return input;
    }
};

template <typename T>
class ConstantFunction : public AbstractFunction<T, T> {
    public:
        explicit ConstantFunction(T input) : constant(input) {}
        T eval(const T& input) const override {
            return constant;
        }

        T eval() const{
            return constant;
        }
    private:
        T constant;
};

#endif // ABSTRACT_FUNCTION_HH
