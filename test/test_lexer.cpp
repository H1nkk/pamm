#include <gtest/gtest.h>
#include "lexer/lexer.h"

TEST(LexerTest, can_store_expressions)
{
    Lexer::Lexer l("pol1 = x^2yz^333 + 3.023 + (pol2 * wxPol333) * 1.12x^33w");

    EXPECT_EQ(l.getText(), "pol1 = x^2yz^333 + 3.023 + (pol2 * wxPol333) * 1.12x^33w");
    EXPECT_EQ(l.getTextSize(), 56);
}

TEST(LexerTest, empty_string_returns_eof)
{
    Lexer::Lexer l("");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 1);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 0, 0));
}

TEST(LexerTest, can_parse_integers)
{
    Lexer::Lexer l(" 12321");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 2);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::INT, "12321", 1, 6));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 6, 6));
}

TEST(LexerTest, can_parse_floats)
{
    Lexer::Lexer l(" 123.21");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 2);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::FLOAT, "123.21", 1, 7));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 7, 7));
}

TEST(LexerTest, can_parse_operations)
{
    Lexer::Lexer l("+- *^ =,");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 7);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::PLUS, "+", 0, 1));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::MINUS, "-", 1, 2));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::MULT, "*", 3, 4));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::CARET, "^", 4, 5));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::ASSIGN, "=", 6, 7));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::COMMA, ",", 7, 8));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 8, 8));
}

TEST(LexerTest, can_parse_parenthesis)
{
    Lexer::Lexer l("() ) )(( ");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 7);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::LPAR, "(", 0, 1));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::RPAR, ")", 1, 2));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::RPAR, ")", 3, 4));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::RPAR, ")", 5, 6));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::LPAR, "(", 6, 7));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::LPAR, "(", 7, 8));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 9, 9));
}

TEST(LexerTest, can_parse_identifiers)
{
    Lexer::Lexer l("myPol wxp");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 3);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::ID, "myPol", 0, 5));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ID, "wxp", 6, 9));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 9, 9));
}

TEST(LexerTest, can_parse_monomials)
{
    Lexer::Lexer l("12.12x^39yz12w1 ");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 10);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::FLOAT, "12.12", 0, 5));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::X, "x", 5, 6));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::CARET, "^", 6, 7));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::INT, "39", 7, 9));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::Y, "y", 9, 10));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::Z, "z", 10, 11));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::INT, "12", 11, 13));
    EXPECT_EQ(l.getAllTokens()[7], Lexer::Token(Lexer::TokenType::W, "w", 13, 14));
    EXPECT_EQ(l.getAllTokens()[8], Lexer::Token(Lexer::TokenType::INT, "1", 14, 15));
    EXPECT_EQ(l.getAllTokens()[9], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 16, 16));
}

TEST(LexerTest, can_parse_variables)
{
    Lexer::Lexer l("xYz W xyzPol");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 6);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::X, "x", 0, 1));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::Y, "Y", 1, 2));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::Z, "z", 2, 3));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::W, "W", 4, 5));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::ID, "xyzPol", 6, 12));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 12, 12));
}

TEST(LexerTest, can_parse_functions)
{
    Lexer::Lexer l("calc(derx(10x+1) * dery(5.1y+1) + derz(z) - derw(w2) * intx(1.2) + (inty(12 + x) - 30*intz(22) * intw(2)), 10,2,3,4)");

    EXPECT_EQ(l.getTokensCount(), 64);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::CALC, "calc", 0, 4));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::DERX, "derx", 5, 9));
    EXPECT_EQ(l.getAllTokens()[10], Lexer::Token(Lexer::TokenType::DERY, "dery", 19, 23));
    EXPECT_EQ(l.getAllTokens()[18], Lexer::Token(Lexer::TokenType::DERZ, "derz", 34, 38));
    EXPECT_EQ(l.getAllTokens()[23], Lexer::Token(Lexer::TokenType::DERW, "derw", 44, 48));
    EXPECT_EQ(l.getAllTokens()[29], Lexer::Token(Lexer::TokenType::INTX, "intx", 55, 59));
    EXPECT_EQ(l.getAllTokens()[35], Lexer::Token(Lexer::TokenType::INTY, "inty", 68, 72));
    EXPECT_EQ(l.getAllTokens()[44], Lexer::Token(Lexer::TokenType::INTZ, "intz", 86, 90));
    EXPECT_EQ(l.getAllTokens()[49], Lexer::Token(Lexer::TokenType::INTW, "intw", 97, 101));
}

TEST(LexerTest, can_parse_expressions_stress_test)
{
    Lexer::Lexer l("pol1 = x^2yz^333 + 3.023 + (pol2 * wxPol333) * 1.12x^33w");

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 24);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::ID, "pol1", 0, 4));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ASSIGN, "=", 5, 6));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::X, "x", 7, 8));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::CARET, "^", 8, 9));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::INT, "2", 9, 10));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::Y, "y", 10, 11));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::Z, "z", 11, 12));
    EXPECT_EQ(l.getAllTokens()[7], Lexer::Token(Lexer::TokenType::CARET, "^", 12, 13));
    EXPECT_EQ(l.getAllTokens()[8], Lexer::Token(Lexer::TokenType::INT, "333", 13, 16));
    EXPECT_EQ(l.getAllTokens()[9], Lexer::Token(Lexer::TokenType::PLUS, "+", 17, 18));
    EXPECT_EQ(l.getAllTokens()[10], Lexer::Token(Lexer::TokenType::FLOAT, "3.023", 19, 24));
    EXPECT_EQ(l.getAllTokens()[11], Lexer::Token(Lexer::TokenType::PLUS, "+", 25, 26));
    EXPECT_EQ(l.getAllTokens()[12], Lexer::Token(Lexer::TokenType::LPAR, "(", 27, 28));
    EXPECT_EQ(l.getAllTokens()[13], Lexer::Token(Lexer::TokenType::ID, "pol2", 28, 32));
    EXPECT_EQ(l.getAllTokens()[14], Lexer::Token(Lexer::TokenType::MULT, "*", 33, 34));
    EXPECT_EQ(l.getAllTokens()[15], Lexer::Token(Lexer::TokenType::ID, "wxPol333", 35, 43));
    EXPECT_EQ(l.getAllTokens()[16], Lexer::Token(Lexer::TokenType::RPAR, ")", 43, 44));
    EXPECT_EQ(l.getAllTokens()[17], Lexer::Token(Lexer::TokenType::MULT, "*", 45, 46));
    EXPECT_EQ(l.getAllTokens()[18], Lexer::Token(Lexer::TokenType::FLOAT, "1.12", 47, 51));
    EXPECT_EQ(l.getAllTokens()[19], Lexer::Token(Lexer::TokenType::X, "x", 51, 52));
    EXPECT_EQ(l.getAllTokens()[20], Lexer::Token(Lexer::TokenType::CARET, "^", 52, 53));
    EXPECT_EQ(l.getAllTokens()[21], Lexer::Token(Lexer::TokenType::INT, "33", 53, 55));
    EXPECT_EQ(l.getAllTokens()[22], Lexer::Token(Lexer::TokenType::W, "w", 55, 56));
    EXPECT_EQ(l.getAllTokens()[23], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 56, 56));
}

TEST(LexerTest, can_read_one_by_one)
{
    Lexer::Lexer l("324243xyzw + myPpol ");

    EXPECT_EQ(l.getPos(), 0);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::INT, "324243", 0, 6));
    EXPECT_EQ(l.getPos(), 1);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::X, "x", 6, 7));
    EXPECT_EQ(l.getPos(), 2);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::Y, "y", 7, 8));
    EXPECT_EQ(l.getPos(), 3);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::Z, "z", 8, 9));
    EXPECT_EQ(l.getPos(), 4);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::W, "w", 9, 10));
    EXPECT_EQ(l.getPos(), 5);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::PLUS, "+", 11, 12));
    EXPECT_EQ(l.getPos(), 6);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ID, "myPpol", 13, 19));
    EXPECT_EQ(l.getPos(), 7);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 20, 20));
    EXPECT_EQ(l.getPos(), 7);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 20, 20));
    EXPECT_EQ(l.getPos(), 7);
}

TEST(LexerTest, can_use_seek)
{
    Lexer::Lexer l("324243xyzw + myPpol ");

    EXPECT_EQ(l.getPos(), 0);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::INT, "324243", 0, 6));
    EXPECT_EQ(l.getPos(), 1);
    l.seek(0);
    EXPECT_EQ(l.getPos(), 0);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::INT, "324243", 0, 6));
    EXPECT_EQ(l.getPos(), 1);
    l.seek(6);
    EXPECT_EQ(l.getPos(), 6);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ID, "myPpol", 13, 19));
    EXPECT_EQ(l.getPos(), 7);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 20, 20));
    l.seek(7);
    EXPECT_EQ(l.getPos(), 7);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 20, 20));
    EXPECT_EQ(l.getPos(), 7);
}

TEST(LexerTest, throw_on_seek_out_of_bounds)
{
    Lexer::Lexer l("324243xyzw + myPpol ");

    EXPECT_EQ(l.getTokensCount(), 8);
    EXPECT_ANY_THROW(l.seek(8));
}

TEST(LexerTest, error_on_unexpected_symbol)
{
    Lexer::Lexer l("324243xyzw + myP$pol + 300");

    EXPECT_EQ(l.hasError(), true);
    EXPECT_EQ(l.getTokensCount(), 8);
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::ID, "myP", 13, 16));
    EXPECT_EQ(l.getAllTokens()[7], Lexer::Token(Lexer::TokenType::INVALID, "$", 16, 17));
}