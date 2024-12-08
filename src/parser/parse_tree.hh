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
        : expression(expression), tokens(tokenize(expression)), pos(0) {}

    /**
     * @brief Parses the expression into an AST.
     *
     * @return A unique pointer to the root node of the AST.
     * @throws std::runtime_error If extra tokens remain after parsing.
     */
    std::unique_ptr<ASTNode> parse() {
        auto node = parseExpression();
        if (current().type != TokenType::END) {
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

        for (const auto &child : node->children) {
            generateFunction(child.get());
        }

        if (node->children.empty() && (node->nodeType == NodeType::NUM || node->nodeType == NodeType::VAR)) {
            createNumerics(node);
        } else if (node->nodeType == NodeType::OP) {
            performOperation(node);
        } else if (node->nodeType == NodeType::FUNC) {
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
        if (functionStack.empty()) {
            throw std::runtime_error("Function Stack is Empty. Run generateFunction() first.");
        }

        return functionStack.top();

    }

private:
    std::string expression; ///< The original mathematical expression.
    std::vector<Token> tokens; ///< The tokens representing the expression.
    size_t pos; ///< The current position in the tokens vector.
    std::stack<AbstractFunction<InputType, OutputType>*> functionStack; ///< Stack used for function generation.
    std::vector<std::unique_ptr<AbstractFunction<InputType, OutputType>>> functionMemory; ///< Stores generated functions.

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

        if (token->value == "cos") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::cos(x); }
            );
        } else if (token->value == "sin") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::sin(x); }
            );
        } else if (token->value == "tan") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::tan(x); }
            );
        } else if (token->value == "exp") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) { return std::exp(x); }
            );
        } else if (token->value == "log") {
            func = std::make_unique<FunctionWrapper<InputType, OutputType>>(
                [](InputType x) {
                    if (x <= 0) {
                        throw std::domain_error("Logarithm undefined for non-positive values");
                    }
                    return std::log(x);
                }
            );
        } else {
            throw std::invalid_argument("Unknown function: " + token->value);
        }

        // Ensure there is an argument on the stack for the composed function
        if (functionStack.empty()) {
            throw std::runtime_error("Not enough arguments on the stack for function: " + token->value);
        }

        // Move the top argument off the stack before popping
        auto argPtr = functionStack.top();
        functionStack.pop();

        functionMemory.push_back(std::move(func));
        auto funcPtr = functionMemory.back().get();

        // Now create the composed function using both func and arg
        auto composed = std::make_unique<ComposedFunction<InputType, OutputType>>(*funcPtr, *argPtr);
        functionMemory.push_back(std::move(composed));
        functionStack.push(functionMemory.back().get());
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
        auto rightPtr = functionStack.top();
        functionStack.pop();

        auto leftPtr = functionStack.top();
        functionStack.pop();

        if (token->value == "+") {
            newFunc = std::make_unique<AddFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value == "-") {
            newFunc = std::make_unique<SubtractFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value == "*") {
            newFunc = std::make_unique<MultiplyFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else if (token->value == "/") {
            newFunc = std::make_unique<DivideFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        }else if (token->value == "^") {
            newFunc = std::make_unique<ExponentialFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else {
            throw std::invalid_argument("Unsupported operator: " + token->value);
        }

        functionMemory.push_back(std::move(newFunc));
        functionStack.push(functionMemory.back().get());;

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

        if(token->nodeType == NodeType::NUM) {
            InputType value;
            if constexpr (std::is_floating_point<InputType>::value) {
                value = static_cast<InputType>(std::stod(token->value)); // Convert to floating-point
            } else if constexpr (std::is_integral<InputType>::value) {
                value = static_cast<InputType>(std::stoi(token->value)); // Convert to integer
            } else {
                throw std::invalid_argument("Unsupported InputType for conversion");
            }
            func = std::make_unique<ConstantFunction<InputType>>(value);

        } else if(token->nodeType == NodeType::VAR) {
            if (token->value[0] == '-') {
                func = std::make_unique<NegativeIdentityFunction<InputType>>();
            } else {
                func = std::make_unique<IdentityFunction<InputType>>();
            }
        } else {
            throw std::invalid_argument("Invalid Token Found: " + token->value);
        }

        functionMemory.push_back(std::move(func));
        // Push a raw pointer onto the stack
        functionStack.push(functionMemory.back().get());
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
        if (pos < tokens.size()) return tokens[pos];
        static Token endToken{TokenType::END, ""};
        return endToken;
    }

    const Token& peek() const {
        if (pos+1 < tokens.size()) return tokens[pos+1];
        static Token endToken{TokenType::END, ""};
        return endToken;
    }

    void advance() {
        if (pos < tokens.size()) pos++;
    }

    void expect(TokenType t) {
        if (current().type != t) {
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
        while (current().type == TokenType::PLUS || current().type == TokenType::MINUS) {
            std::string op = current().value;
            advance();
            auto right = parseTerm();
            node = makeOp(op, std::move(node), std::move(right));
        }
        return node;
    }

    // Term := Factor { ('*' | '/') Factor }
    std::unique_ptr<ASTNode> parseTerm() {
        auto node = parseFactor();
        while (current().type == TokenType::MUL || current().type == TokenType::DIV) {
            std::string op = current().value;
            advance();
            auto right = parseFactor();
            node = makeOp(op, std::move(node), std::move(right));
        }
        return node;
    }

    // Factor := Power { '^' Power }
    std::unique_ptr<ASTNode> parseFactor() {
        auto node = parsePower();
        while (current().type == TokenType::POW) {
            advance(); // consume '^'
            auto right = parsePower();
            node = makeOp("^", std::move(node), std::move(right));
        }
        return node;
    }

    // Power := FunctionCall | Variable | Number | '(' Expression ')'
    std::unique_ptr<ASTNode> parsePower() {
        if (current().type == TokenType::IDENT) {
            std::string name = current().value;
            // Check if it's a function call
            if (peek().type == TokenType::LPAREN) {
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
        } else if (current().type == TokenType::NUMBER) {
            std::string val = current().value;
            advance();
            return std::make_unique<ASTNode>(NodeType::NUM, val);
        } else if (current().type == TokenType::LPAREN) {
            advance(); // consume '('
            auto node = parseExpression();
            expect(TokenType::RPAREN);
            advance(); // consume ')'
            return node;
        } else if (current().value == "-") {
            std::string val = current().value;
            advance();
            val += current().value;
            if (current().type == TokenType::NUMBER) {
                advance();
                return std::make_unique<ASTNode>(NodeType::NUM, val);
            } else if (current().type == TokenType::IDENT) {
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
