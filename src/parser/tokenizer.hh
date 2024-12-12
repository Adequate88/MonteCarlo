//
// Created by Alessandro Salo on 08.12.2024.
//

/**
 * @file tokenizer.hh
 * @brief Defines the token and node classes, and helper functions for Abstract Syntax Tree (AST).
 *
 * This file contains definitions for tokens, AST nodes, helper functions for creating AST nodes
 */
#ifndef TOKENIZER_HH
#define TOKENIZER_HH

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

//-------------------------------------------------------
// Token definition
//-------------------------------------------------------

/**
 * @brief Enum representing the types of tokens in a mathematical expression.
 */
enum class TokenType {
    NUMBER,     ///< Numeric value
    IDENT,      ///< Variable or function name
    PLUS,       ///< Addition operator
    MINUS,      ///< Subtraction operator
    MUL,        ///< Multiplication operator
    DIV,        ///< Division operator
    POW,        ///< Power operator
    LPAREN,     ///< Left parenthesis '('
    RPAREN,     ///< Right parenthesis ')'
    END         ///< End of the expression
};
/**
 * @brief Struct representing a token in the expression.
 */
struct Token {
    TokenType type_;        ///< Type of the token
    std::string value_;     ///< Value of the token (e.g., number or symbol)
};

/**
 * @brief Enum representing the types of AST nodes.
 */
enum class NodeType {
    NUM,    ///< Numeric node
    VAR,    ///< Variable node
    FUNC,   ///< Function node
    OP      ///< Operator node
};

/**
 * @brief Class representing a node in the Abstract Syntax Tree (AST).
 */
class ASTNode {
public:
    /**
     * @brief Constructs an ASTNode with a given type and value.
     *
     * @param t The type of the node.
     * @param val The value of the node, such as operator symbol, function name, or numeric string.
     */
    ASTNode(NodeType t, const std::string &val) : node_type_(t), value_(val) {}

    NodeType node_type_; ///< The type of the node.
    std::string value_; ///< The value of the node (operator, variable name, function name, or numeric string).
    std::vector<std::unique_ptr<ASTNode>> children_; ///< Children nodes for functions and operators.
};

/**
 * @brief Creates a function node in the AST.
 *
 * @param name The name of the function.
 * @param arg The argument for the function.
 * @return A unique pointer to the created function node.
 */
inline std::unique_ptr<ASTNode> makeFunc(const std::string &name, std::unique_ptr<ASTNode> arg) {
    auto node = std::make_unique<ASTNode>(NodeType::FUNC, name);
    node->children_.push_back(std::move(arg));
    return node;
}

/**
 * @brief Creates an operator node in the AST.
 *
 * @param op The operator symbol.
 * @param left The left operand.
 * @param right The right operand.
 * @return A unique pointer to the created operator node.
 */
inline std::unique_ptr<ASTNode> makeOp(const std::string &op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) {
    auto node = std::make_unique<ASTNode>(NodeType::OP, op);
    node->children_.push_back(std::move(left));
    node->children_.push_back(std::move(right));
    return node;
}


/**
 * @brief Tokenizes a given mathematical expression string.
 *
 * @param expr The mathematical expression as a string.
 * @return A vector of tokens representing the expression.
 * @throws std::runtime_error If an unexpected character is found in the expression.
 */
inline std::vector<Token> tokenize(const std::string &expr) {
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

#endif //TOKENIZER_HH
