#include <gtest/gtest.h>
#include "lexer/lexer.h"
#include "expression_compiler/expression_compiler.h"
#include "expression_interpreter/expression_interpreter.h"

using namespace Intr;
using namespace Compiler;

TEST(ExprInterpreterTest, can_execute_single_monomial)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("123x54y12z12w").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("123x54y12z12w")));
}

//TEST(ExprInterpreterTest, can_execute_polynomial_name) {
//    Aggregator agg;
//    polynomial p = std::get<polynomial>(polynomial::fromString("123x54y12z12w"));
//    agg.addPolynomial("mypol", p);
//    ExpressionInterpreter intr(&agg);
//    auto res = intr.execute(
//        std::get<Program>(ExpressionCompiler().compileExpression(
//            Lexer::Lexer("mypol").getAllTokens()
//        ))
//    );
//
//    EXPECT_TRUE(std::holds_alternative<polynomial>(res));
//    EXPECT_EQ(std::get<polynomial>(res), p);
//}

TEST(ExprInterpreterTest, can_execute_sum)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("123x54y12z12w + (50 + 40x)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("123x54y12z12w + 40x + 50")));
}

TEST(ExprInterpreterTest, can_execute_subtraction)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("123x54y12z12w - (50 + 40x)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("123x54y12z12w - 40x - 50")));
}

TEST(ExprInterpreterTest, can_execute_multiplication)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("123x54y12z12w * (50 + 40x)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("6150x54y12z12w + 4920x55y12z12w")));
}

//TEST(ExprInterpreterTest, can_execute_assignment) {
//    Aggregator agg;
//    ExpressionInterpreter intr(&agg);
//    auto res = intr.execute(
//        std::get<Program>(ExpressionCompiler().compileExpression(
//            Lexer::Lexer("mypol = 123x54y12z12w * (50 + 40x)").getAllTokens()
//        ))
//    );
//
//    EXPECT_TRUE(std::holds_alternative<polynomial>(res));
//    EXPECT_EQ(std::get<polynomial>(res), std::get<polynomial>(polynomial::fromString("6150x54y12z12w + 4920x55y12z12w")));
//
//    EXPECT_EQ(agg.size(), 1);
//    EXPECT_EQ(agg.findPolynomial("mypol"), std::get<polynomial>(polynomial::fromString("6150x54y12z12w + 4920x55y12z12w")));
//}

TEST(ExprInterpreterTest, can_execute_calc)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("123x54y12z12w + calc(50 + 40x + y, 50.0, -10, -2, 30.112)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("123x54y12z12w + 2040")));
}

TEST(ExprInterpreterTest, can_execute_der)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("derx(3w^2 + 2x + z + 4y^3) + dery(3w^2 + 2x + z + 4y^3) + derz(3w^2 + 2x + z + 4y^3) + derw(3w^2 + 2x + z + 4y^3)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("3 + 12y2 + 6w")));
}

TEST(ExprInterpreterTest, can_execute_int)
{
    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    auto res = intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("intx(3w^2 + 2x + z + 4y^3) + inty(3w^2 + 2x + z + 4y^3) + intz(3w^2 + 2x + z + 4y^3) + intw(3w^2 + 2x + z + 4y^3)").getAllTokens()
        ))
    );

    EXPECT_TRUE(std::holds_alternative<Polynomial>(res));
    EXPECT_EQ(std::get<Polynomial>(res), std::get<Polynomial>(Polynomial::fromString("3xw2+x2+3xz+4xy3 + 3yw2+2xy+yz+y4 + 3zw2+0.5z2+4zy3 + w3+2xw+wz+4wy3")));
}

TEST(ExprInterpreterTest, throw_on_unknown_polynomial)
{

    Aggregator agg;
    ExpressionInterpreter intr(&agg);
    EXPECT_ANY_THROW(intr.execute(
        std::get<Program>(ExpressionCompiler().compileExpression(
            Lexer::Lexer("b = 2 * a").getAllTokens()
        ))
    ));
}