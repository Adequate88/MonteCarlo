#include <cmath>
#include <gtest/gtest.h>
#include "sampling.hh"
#include "statistics.hh"
#include <cmath>
#include <gtest/gtest.h>
#include "sampling.hh"
#include "statistics.hh"
#include "parse_tree.hh"

/* ==========================================================
 * TESTS for Parser
 * ==========================================================
 */


TEST(ParserTest, TokenizeSimpleExpression) {
    std::string expression = "3 + 5";
    std::vector<Token> tokens = tokenize(expression);

    ASSERT_EQ(tokens.size(), 4); // 3 tokens + 1 END token
    EXPECT_EQ(tokens[0].type_, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].type_, TokenType::PLUS);
    EXPECT_EQ(tokens[2].type_, TokenType::NUMBER);
    EXPECT_EQ(tokens[3].type_, TokenType::END);
}

TEST(ParserTest, ParseSimpleExpression) {
    std::string expression = "3 + 5";
    Parser<int, double> parser(expression);
    auto ast = parser.parse();

    // Expect an AST with an operator node (PLUS) and two numeric children
    EXPECT_EQ(ast->node_type_, NodeType::OP);
    EXPECT_EQ(ast->value_, "+");
    EXPECT_EQ(ast->children_.size(), 2);
    EXPECT_EQ(ast->children_[0]->node_type_, NodeType::NUM);
    EXPECT_EQ(ast->children_[1]->node_type_, NodeType::NUM);
}

TEST(ParserTest, GenerateFunction) {
    std::string expression = "3.2*x + exp(x+5) + 5*sin(x*3)";
    Parser<double, double> parser(expression);
    auto ast = parser.parse();

    // Generate the function
    parser.generateFunction(ast.get());

    // Get the final function and check if it matches the expected result
    auto finalFunc = parser.getFinalFunction();
    EXPECT_NEAR(finalFunc->eval(5.0), 22045.7, 0.1); // Allow a small tolerance, e.g., 0.1
}
