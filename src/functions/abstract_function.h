//
// Created by alixb1908 on 28/11/24.
//

#ifndef ABSTRACT_FUNCTION_HH
#define ABSTRACT_FUNCTION_HH

#include <memory>
#include <limits>
#include <stdexcept>
#include <functional>


// Forward declarations for ComposedFunction and AddFunction
template <typename InputType, typename OutputType>
class ComposedFunction;

template <typename InputType, typename OutputType>
class AddFunction;

template <typename InputType, typename OutputType>
class MultiplyFunction;

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
};

// Separate class for composing two functions
template <typename InputType, typename OutputType>
class ComposedFunction : public AbstractFunction<InputType, OutputType> {
public:
    ComposedFunction(const AbstractFunction<InputType, OutputType>& first, const AbstractFunction<OutputType, OutputType>& second)
        : first_(first), second_(second) {}

    OutputType eval(const InputType& input) const override {
        OutputType intermediate = first_.eval(input);
        return second_.eval(intermediate);
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
            throw std::overflow_error("Overflow occurred during addition.");
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
            throw std::overflow_error("Overflow occurred during multiplication.");
        }

        return f_result * g_result;
    }

private:
    const AbstractFunction<InputType, OutputType>& f_;
    const AbstractFunction<InputType, OutputType>& g_;
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

#endif // ABSTRACT_FUNCTION_HH


