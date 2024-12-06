//
// Created by Alessandro Salo on 05.12.2024.
//
//
// Created by Alessandro Salo on 05.12.2024.
//
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <stack>

#ifndef PARSE_TREE_HH
#define PARSE_TREE_HH

//-------------------------------------------------------
// Token definition
//-------------------------------------------------------
enum class TokenType {
    NUMBER,
    IDENT,    // variable or function name
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

enum class NodeType {
    NUM,
    VAR,
    FUNC,
    OP
};

class ASTNode {
public:
    NodeType nodeType;
    std::string value; // operator symbol, variable name, function name, or numeric string
    std::vector<std::unique_ptr<ASTNode>> children;

    ASTNode(NodeType t, const std::string &val) : nodeType(t), value(val) {}

};

// Helper function to create a function node
std::unique_ptr<ASTNode> makeFunc(const std::string &name, std::unique_ptr<ASTNode> arg) {
    auto node = std::make_unique<ASTNode>(NodeType::FUNC, name);
    node->children.push_back(std::move(arg));
    return node;
}

// Helper to create an operator node
std::unique_ptr<ASTNode> makeOp(const std::string &op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) {
    auto node = std::make_unique<ASTNode>(NodeType::OP, op);
    node->children.push_back(std::move(left));
    node->children.push_back(std::move(right));
    return node;
}

std::vector<Token> tokenize(const std::string &expr) {
    std::vector<Token> tokens;
    size_t pos = 0;

    auto skipWhitespace = [&]( ) {
        while (pos < expr.size() && std::isspace(static_cast<unsigned char>(expr[pos]))) {
            pos++;
        }
    };

    auto readNumber = [&]( ) {
        size_t start = pos;
        bool seenDot = false;
        while (pos < expr.size() && (std::isdigit(static_cast<unsigned char>(expr[pos])) || expr[pos] == '.')) {
            if (expr[pos] == '.') {
                if (seenDot) {
                    throw std::runtime_error("Invalid number with multiple decimal points");
                }
                seenDot = true;
            }
            pos++;
        }
        tokens.push_back({TokenType::NUMBER, expr.substr(start, pos - start)});
    };

    auto readIdent = [&]( ) {
        size_t start = pos;
        while (pos < expr.size() && std::isalpha(static_cast<unsigned char>(expr[pos]))) {
            pos++;
        }
        tokens.push_back({TokenType::IDENT, expr.substr(start, pos - start)});
    };

    while (pos < expr.size()) {
        skipWhitespace();
        if (pos >= expr.size()) break;

        char c = expr[pos];

        if (std::isalpha(static_cast<unsigned char>(c))) {
            // Identifier (function or variable)
            readIdent();
        } else if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            // Number
            readNumber();
        } else {
            // Single-char tokens
            switch (c) {
                case '+':
                    tokens.push_back({TokenType::PLUS, "+"});
                    pos++;
                    break;
                case '-':
                    tokens.push_back({TokenType::MINUS, "-"});
                    pos++;
                    break;
                case '*':
                    tokens.push_back({TokenType::MUL, "*"});
                    pos++;
                    break;
                case '/':
                    tokens.push_back({TokenType::DIV, "/"});
                    pos++;
                    break;
                case '^':
                    tokens.push_back({TokenType::POW, "^"});
                    pos++;
                    break;
                case '(':
                    tokens.push_back({TokenType::LPAREN, "("});
                    pos++;
                    break;
                case ')':
                    tokens.push_back({TokenType::RPAREN, ")"});
                    pos++;
                    break;
                default:
                    throw std::runtime_error(std::string("Unexpected character: ") + c);
            }
        }
    }

    tokens.push_back({TokenType::END, ""});
    return tokens;
}

//-------------------------------------------------------
// Parser class
//-------------------------------------------------------
template <typename InputType, typename OutputType>
class Parser {
public:
    // New constructor: Takes a string, tokenizes it, and uses the resulting tokens
    Parser(const std::string &expression)
        : expression(expression), tokens(tokenize(expression)), pos(0)
    {}

    std::unique_ptr<ASTNode> parse() {
        auto node = parseExpression();
        if (current().type != TokenType::END) {
            throw std::runtime_error("Extra tokens after valid expression");
        }
        return node;
    }

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

    AbstractFunction<InputType, OutputType>* getFinalFunction() {
        if (functionStack.empty()) {
            throw std::runtime_error("Function Stack is Empty. Run generateFunction() first.");
        }

        return functionStack.top();

    }

    void printFunctionMemory() const {
        std::cout << "functionMemory contents:\n";
        if (functionMemory.empty()) {
            std::cout << "  (empty)\n\n";
            return;
        }

        for (size_t i = 0; i < functionMemory.size(); ++i) {
            // Print the pointer address of each stored function
            std::cout << "  functionMemory[" << i << "] = " << functionMemory[i].get() << "\n";
        }
        std::cout << std::endl;
    }

    void printFunctionStack() const {
        std::cout << "functionStack contents (top to bottom):\n";

        if (functionStack.empty()) {
            std::cout << "  (empty)\n\n";
            return;
        }

        // Copy the stack so we can iterate without destroying it
        auto tempStack = functionStack;
        while (!tempStack.empty()) {
            AbstractFunction<InputType, OutputType>* funcPtr = tempStack.top();
            tempStack.pop();
            // Print the pointer address
            std::cout << "  " << funcPtr << "\n";
        }
        std::cout << std::endl;
    }

private:
    std::string expression;
    std::vector<Token> tokens;
    size_t pos;
    std::stack<AbstractFunction<InputType, OutputType>*> functionStack;
    std::vector<std::unique_ptr<AbstractFunction<InputType, OutputType>>> functionMemory;

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


    void performOperation(const ASTNode* token) {
        std::unique_ptr<AbstractFunction<InputType, OutputType>> newFunc;
        // Extract the right operand
        auto rightPtr = functionStack.top();
        functionStack.pop();

        auto leftPtr = functionStack.top();
        functionStack.pop();

        if (token->value == "+") {
            newFunc = std::make_unique<AddFunction<InputType, OutputType>>(*leftPtr, *rightPtr);

        } else if (token->value == "*") {
            newFunc = std::make_unique<MultiplyFunction<InputType, OutputType>>(*leftPtr, *rightPtr);
        } else {
            throw std::invalid_argument("Unsupported operator: " + token->value);
        }

        functionMemory.push_back(std::move(newFunc));
        functionStack.push(functionMemory.back().get());;

    }

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
            func = std::make_unique<IdentityFunction<InputType>>();

        } else {
            throw std::invalid_argument("Invalid Token Found: " + token->value);
        }

        functionMemory.push_back(std::move(func));
        // Push a raw pointer onto the stack
        functionStack.push(functionMemory.back().get());
    }


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

    // Grammar:
    // Expression := Term { ('+' | '-') Term }
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
        } else {
            throw std::runtime_error("Unexpected token while parsing power");
        }
    }
};

#endif //PARSE_TREE_HH
