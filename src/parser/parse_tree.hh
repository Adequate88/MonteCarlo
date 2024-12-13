//
// Created by Alessandro Salo on 05.12.2024.
//

/**
 * @file parse_tree.hh
 * @brief Defines the classes and functions used for parsing mathematical expressions into an Abstract Syntax Tree (AST).
 *
 * This file contains a Parser class for converting mathematical expressions into an AST representation that can be used
 * for evaluation or other analyses.
 */
#ifndef PARSE_TREE_HH
#define PARSE_TREE_HH

#include <stack>
#include "primitive_functions.hh"
#include "tokenizer.hh"

/**
 * @brief Class that parses a mathematical expression into an Abstract Syntax Tree (AST).
 *
 * @tparam InputType The type of the input value.
 * @tparam OutputType The type of the output value.
 */
template <typename InputType, typename OutputType>
class Parser {
public:
    /**
     * @brief Constructs a Parser object and tokenizes the given expression.
     *
     * @param expression The mathematical expression to parse.
     */
    Parser(const std::string &expression)
        : expression_(expression), tokens_(tokenize(expression)), pos_(0) {}

    /**
     * @brief Parses the expression into an AST.
     *
     * @return A unique pointer to the root node of the AST.
     * @throws std::runtime_error If extra tokens remain after parsing.
     */
    std::unique_ptr<ASTNode> parse() {
        auto node = parseExpression();
        if (current().type_ != TokenType::END) {
            throw std::runtime_error("Extra tokens after valid expression");
        }
        return node;
    }

    /**
     * @brief Generates the function from the given AST node.
     *
     * Recursively traverses the AST and generates the corresponding function representation.
     *
     * @param node The root node of the AST.
     */
    void generateFunction(const ASTNode* node) {

        for (const auto &child : node->children_) {
            generateFunction(child.get());
        }

        if (node->children_.empty() && (node->node_type_ == NodeType::NUM || node->node_type_ == NodeType::VAR)) {
            createNumerics(node);
        } else if (node->node_type_ == NodeType::OP) {
            performOperation(node);
        } else if (node->node_type_ == NodeType::FUNC) {
            composeFunc(node);
        }
    }

    /**
     * @brief Retrieves the final generated function.
     *
     * @return A pointer to the final function represented by the AST.
     * @throws std::runtime_error If no function has been generated.
     */
    AbstractFunction<InputType, OutputType>* getFinalFunction() {
        if (function_stack_.empty()) {
            throw std::runtime_error("Function Stack is Empty. Run generateFunction() first.");
        }

        return function_stack_.top();

    }

private:
    std::string expression_; ///< The original mathematical expression.
    std::vector<Token> tokens_; ///< The tokens representing the expression.
    size_t pos_; ///< The current position in the tokens vector.
    std::stack<AbstractFunction<InputType, OutputType>*> function_stack_; ///< Stack used for function generation.
    std::vector<std::unique_ptr<AbstractFunction<InputType, OutputType>>> function_memory_; ///< Stores generated functions.

    /**
     * @brief Composes a function node.
     *
     * This method handles the composition of a function, creating a composed function
     * by using a function node from the AST and an argument function from the function stack.
     *
     * @param token The AST node representing the function to compose.
     * @return A unique pointer to the composed AbstractFunction.
     * @throws std::runtime_error If there are not enough arguments on the stack for composition.
     */
    void composeFunc(const ASTNode* token) {
        std::unique_ptr<AbstractFunction<InputType, OutputType>> func;

        if (token->value_ == "cos") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::cos(x); }
            );
        } else if (token->value_ == "sin") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::sin(x); }
            );
        } else if (token->value_ == "tan") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::tan(x); }
            );
        } else if (token->value_ == "exp") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::exp(x); }
            );
        } else if (token->value_ == "log") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) {
                    if (x <= 0) {
                        throw std::domain_error("Logarithm undefined for non-positive values");
                    }
                    return std::log(x);
                }
            );
        } else {
            throw std::invalid_argument("Unknown function: " + token->value_);
        }

        // Ensure there is an argument on the stack for the composed function
        if (function_stack_.empty()) {
            throw std::runtime_error("Not enough arguments on the stack for function: " + token->value_);
        }

        // Move the top argument off the stack before popping
        auto argPtr = function_stack_.top();
        function_stack_.pop();

        function_memory_.push_back(std::move(func));
        auto funcPtr = function_memory_.back().get();

        // Now create the composed function using both func and arg
        auto composed = std::make_unique<ComposedFunction<InputType, OutputType>>(*funcPtr, *argPtr);
        function_memory_.push_back(std::move(composed));
        function_stack_.push(function_memory_.back().get());
    }

    /**
     * @brief Generates a function by performing an operation.
     *
     * This method generates a function by applying an operator (e.g., addition, multiplication)
     * to two functions currently on the stack.
     *
     * @param token The AST node representing the operator.
     * @return A unique pointer to the generated AbstractFunction.
     * @throws std::invalid_argument If the operator is unsupported.
     */
    void performOperation(const ASTNode* token) {
        std::unique_ptr<AbstractFunction<InputType, OutputType>> newFunc;
        // Extract the right operand
        auto rightPtr = function_stack_.top();
        function_stack_.pop();

        auto leftPtr = function_stack_.top();
        function_stack_.pop();

        if (token->value_ == "+") {
            newFunc = std::make_unique<AddFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value_ == "-") {
            newFunc = std::make_unique<SubtractFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value_ == "*") {
            newFunc = std::make_unique<MultiplyFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value_ == "/") {
            newFunc = std::make_unique<DivideFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        }else if (token->value_ == "^") {
            newFunc = std::make_unique<ExponentialFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else {
            throw std::invalid_argument("Unsupported operator: " + token->value_);
        }

        function_memory_.push_back(std::move(newFunc));
        function_stack_.push(function_memory_.back().get());;

    }

    /**
     * @brief Generates a numeric or variable node for the function stack.
     *
     * This method handles the generation of nodes representing either numeric values or variables,
     * and pushes the corresponding AbstractFunction object onto the function stack.
     *
     * @param node The AST node to generate a function for.
     * @return A unique pointer to the generated AbstractFunction representing the numeric or variable.
     * @throws std::invalid_argument If the node type is unsupported.
     */
    void createNumerics(const ASTNode* token) {
        std::unique_ptr<AbstractFunction<InputType, OutputType>> func;

        if(token->node_type_ == NodeType::NUM) {
            InputType value;
            if constexpr (std::is_floating_point<InputType>::value) {
                value = static_cast<InputType>(std::stod(token->value_)); // Convert to floating-point
            } else if constexpr (std::is_integral<InputType>::value) {
                value = static_cast<InputType>(std::stoi(token->value_)); // Convert to integer
            } else {
                throw std::invalid_argument("Unsupported InputType for conversion");
            }
            func = std::make_unique<ConstantFunction<InputType>>(value);

        } else if(token->node_type_ == NodeType::VAR) {
            if (token->value_[0] == '-') {
                func = std::make_unique<NegativeIdentityFunction<InputType>>();
            } else {
                func = std::make_unique<IdentityFunction<InputType>>();
            }
        } else {
            throw std::invalid_argument("Invalid Token Found: " + token->value_);
        }

        function_memory_.push_back(std::move(func));
        // Push a raw pointer onto the stack
        function_stack_.push(function_memory_.back().get());
    }

    /**
     * @brief Retrieves the current token without advancing.
     *
     * This method returns the current token in the tokens vector, enabling the parser
     * to determine what element it is currently processing.
     *
     * @return The current token.
     */
    const Token& current() const {
        if (pos_ < tokens_.size()) return tokens_[pos_];
        static Token endToken{TokenType::END, ""};
        return endToken;
    }

    const Token& peek() const {
        if (pos_+1 < tokens_.size()) return tokens_[pos_+1];
        static Token endToken{TokenType::END, ""};
        return endToken;
    }

    void advance() {
        if (pos_ < tokens_.size()) pos_++;
    }

    void expect(TokenType t) {
        if (current().type_ != t) {
            throw std::runtime_error("Unexpected token type");
        }
    }

    /**
     * @brief Parses an expression with operator precedence.
     *
     * This method recursively parses operators and their operands to respect operator precedence,
     * ensuring the correct Abstract Syntax Tree (AST) structure.
     *
     * @param precedence The current precedence level for parsing.
     * @return A unique pointer to the root node representing the parsed expression.
     */
    std::unique_ptr<ASTNode> parseExpression() {
        auto node = parseTerm();
        while (current().type_ == TokenType::PLUS || current().type_ == TokenType::MINUS) {
            std::string op = current().value_;
            advance();
            auto right = parseTerm();
            node = makeOp(op, std::move(node), std::move(right));
        }
        return node;
    }

    /**
     * @brief Parses a term in an expression.
     *
     * This method parses terms that are multiplied or divided, respecting the multiplication and
     * division operator precedence. It recursively handles the left and right operands, building
     * the Abstract Syntax Tree (AST) accordingly.
     *
     * @return A unique pointer to the root node representing the parsed term.
     */
    std::unique_ptr<ASTNode> parseTerm() {
        auto node = parseFactor();
        while (current().type_ == TokenType::MUL || current().type_ == TokenType::DIV) {
            std::string op = current().value_;
            advance();
            auto right = parseFactor();
            node = makeOp(op, std::move(node), std::move(right));
        }
        return node;
    }

    /**
     * @brief Parses a factor in an expression.
     *
     * This method handles factors in an expression, which can be powers (exponentiation). It
     * recursively processes the right operand when a power operator (^) is encountered.
     *
     * @return A unique pointer to the root node representing the parsed factor.
     */
    std::unique_ptr<ASTNode> parseFactor() {
        auto node = parsePower();
        while (current().type_ == TokenType::POW) {
            advance(); // consume '^'
            auto right = parsePower();
            node = makeOp("^", std::move(node), std::move(right));
        }
        return node;
    }

    /**
     * @brief Parses a power in an expression.
     *
     * A power can be a function call, a variable, a number, or a parenthesized expression. This
     * method handles these cases by identifying the appropriate token type and creating the
     * corresponding AST node.
     *
     * @return A unique pointer to the root node representing the parsed power.
     */
    std::unique_ptr<ASTNode> parsePower() {
        if (current().type_ == TokenType::IDENT) {
            std::string name = current().value_;
            // Check if it's a function call
            if (peek().type_ == TokenType::LPAREN) {
                advance(); // consume IDENT
                advance(); // consume '('
                auto arg = parseExpression();
                expect(TokenType::RPAREN);
                advance(); // consume ')'
                return makeFunc(name, std::move(arg));
            } else {
                // Just a variable
                advance();
                return std::make_unique<ASTNode>(NodeType::VAR, name);
            }
        } else if (current().type_ == TokenType::NUMBER) {
            std::string val = current().value_;
            advance();
            return std::make_unique<ASTNode>(NodeType::NUM, val);
        } else if (current().type_ == TokenType::LPAREN) {
            advance(); // consume '('
            auto node = parseExpression();
            expect(TokenType::RPAREN);
            advance(); // consume ')'
            return node;
        } else if (current().value_ == "-") {
            std::string val = current().value_;
            advance();
            val += current().value_;
            if (current().type_ == TokenType::NUMBER) {
                advance();
                return std::make_unique<ASTNode>(NodeType::NUM, val);
            } else if (current().type_ == TokenType::IDENT) {
                advance();
                return std::make_unique<ASTNode>(NodeType::VAR, val);
            } else {
                throw std::runtime_error("Unexpected token while parsing negative");
            }

        } else {

            throw std::runtime_error("Unexpected token while parsing power");
        }
    }
};

#endif //PARSE_TREE_HH
