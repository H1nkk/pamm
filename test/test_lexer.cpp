#include <gtest/gtest.h>
#include "lexer/lexer.h"

TEST(LexerTest, can_store_programs)
{
    Lexer::Lexer lexer("e : double = 2.71828;");
    EXPECT_EQ(lexer.getText(), "e : double = 2.71828;");
    EXPECT_EQ(lexer.getTextSize(), 21);
}

TEST(LexerTest, empty_string_returns_eof)
{
    Lexer::Lexer lexer("");
    EXPECT_EQ(lexer.hasError(), false);
    EXPECT_EQ(lexer.getTokensCount(), 1);
    EXPECT_EQ(lexer.getAllTokens()[0], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 0, 0));
}

TEST(LexerTest, can_parse_integers)
{
    Lexer::Lexer lexer(" 333  ");
    EXPECT_EQ(lexer.hasError(), false);
    EXPECT_EQ(lexer.getTokensCount(), 2);
    EXPECT_EQ(lexer.getAllTokens()[0], Lexer::Token(Lexer::TokenType::INT, "333", 1, 4));
    EXPECT_EQ(lexer.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 6, 6));
}

TEST(LexerTest, can_parse_floats)
{
    Lexer::Lexer lexer(" 33.3  ");
    EXPECT_EQ(lexer.hasError(), false);
    EXPECT_EQ(lexer.getTokensCount(), 2);
    EXPECT_EQ(lexer.getAllTokens()[0], Lexer::Token(Lexer::TokenType::FLOAT, "33.3", 1, 5));
    EXPECT_EQ(lexer.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 7, 7));
}

TEST(LexerText, can_parse_strings_literals_with_double_quotes)
{
    Lexer::Lexer lexer(" 'The number is \"very\" important.'  ");
    EXPECT_EQ(lexer.hasError(), false);
    EXPECT_EQ(lexer.getTokensCount(), 2);
    EXPECT_EQ(lexer.getAllTokens()[0], Lexer::Token(Lexer::TokenType::STRING, "'The number is \"very\" important.'", 1, 34));
    EXPECT_EQ(lexer.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 36, 36));
}

TEST(LexerText, can_parse_strings_literals_with_single_quotes)
{
    Lexer::Lexer lexer(" 'The number is \\'very\\' important.'  ");
    EXPECT_EQ(lexer.hasError(), false);
    EXPECT_EQ(lexer.getTokensCount(), 2);
    EXPECT_EQ(lexer.getAllTokens()[0], Lexer::Token(Lexer::TokenType::STRING, "'The number is \'very\' important.'", 1, 36));
    EXPECT_EQ(lexer.getAllTokens()[1], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 38, 38));
}

TEST(LexerTest, can_parse_operations)
{
    Lexer::Lexer l("+-* . , ; : / := < <= > >= <> = not div mod and or "); 

    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 21);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::PLUS, "+", 0, 1));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::MINUS, "-", 1, 2));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::MULT, "*", 2, 3));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::DOT, ".", 4, 5));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::COMMA, ",", 6, 7));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::SEMICOLON, ";", 8, 9));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::COLON, ":", 10, 11));
    EXPECT_EQ(l.getAllTokens()[7], Lexer::Token(Lexer::TokenType::DIV, "/", 12, 13));
    EXPECT_EQ(l.getAllTokens()[8], Lexer::Token(Lexer::TokenType::ASSIGN, ":=", 14, 16));
    EXPECT_EQ(l.getAllTokens()[9], Lexer::Token(Lexer::TokenType::LESS, "<", 17, 18));
    EXPECT_EQ(l.getAllTokens()[10], Lexer::Token(Lexer::TokenType::LESSEQUAL, "<=", 19, 21));
    EXPECT_EQ(l.getAllTokens()[11], Lexer::Token(Lexer::TokenType::GREATER, ">", 22, 23));
    EXPECT_EQ(l.getAllTokens()[12], Lexer::Token(Lexer::TokenType::GREATEREQUAL, ">=", 24, 26));
    EXPECT_EQ(l.getAllTokens()[13], Lexer::Token(Lexer::TokenType::NOTEQUAL, "<>", 27, 29));
    EXPECT_EQ(l.getAllTokens()[14], Lexer::Token(Lexer::TokenType::EQUAL, "=", 30, 31));
    EXPECT_EQ(l.getAllTokens()[15], Lexer::Token(Lexer::TokenType::NOT, "not", 32, 35));
    EXPECT_EQ(l.getAllTokens()[16], Lexer::Token(Lexer::TokenType::DIVINT, "div", 36, 39));
    EXPECT_EQ(l.getAllTokens()[17], Lexer::Token(Lexer::TokenType::MOD, "mod", 40, 43));
    EXPECT_EQ(l.getAllTokens()[18], Lexer::Token(Lexer::TokenType::AND, "and", 44, 47));
    EXPECT_EQ(l.getAllTokens()[19], Lexer::Token(Lexer::TokenType::OR, "or", 48, 50));
    EXPECT_EQ(l.getAllTokens()[20], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 51, 51));

}

TEST(LexerTest, can_parse_parenthesis)
{
    Lexer::Lexer l(" ( 23  )  ");
    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 4);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::LPAREN, "(", 1, 2));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::INT, "23", 3, 5));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::RPAREN, ")", 7, 8));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 10, 10));
}

TEST(LexerTest, can_parse_var_and_const_names)
{
    Lexer::Lexer l(" var   const  ");
    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 3);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::VAR, "var", 1, 4));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::CONST, "const", 7, 12));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 14, 14));
}

TEST(LexerTest, can_parse_keywords)
{
    Lexer::Lexer l("program begin end if then else");
    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 7);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::PROGRAM, "program", 0, 7));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::BEGIN, "begin", 8, 13));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::END, "end", 14, 17));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::IF, "if", 18, 20));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::THEN, "then", 21, 25));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::ELSE, "else", 26, 30));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 30, 30));
}

TEST(LexerTest, can_read_one_by_one)
{
    Lexer::Lexer l("e:double=2.71828;");
    EXPECT_EQ(l.hasError(), false);
    EXPECT_EQ(l.getTokensCount(), 7);
    EXPECT_EQ(l.getAllTokens()[0], Lexer::Token(Lexer::TokenType::ID, "e", 0, 1));
    EXPECT_EQ(l.getAllTokens()[1], Lexer::Token(Lexer::TokenType::COLON, ":", 1, 2));
    EXPECT_EQ(l.getAllTokens()[2], Lexer::Token(Lexer::TokenType::ID, "double", 2, 8));
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::EQUAL, "=", 8, 9));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::FLOAT, "2.71828", 9, 16));
    EXPECT_EQ(l.getAllTokens()[5], Lexer::Token(Lexer::TokenType::SEMICOLON, ";", 16, 17));
    EXPECT_EQ(l.getAllTokens()[6], Lexer::Token(Lexer::TokenType::ENDOFFILE, "", 17, 17));
}

TEST(LexerTest, can_use_seek)
{
    Lexer::Lexer l("e:double=2.71828;");
    l.seek(2);
    EXPECT_EQ(l.getPos(), 2);
    EXPECT_EQ(l.nextToken(), Lexer::Token(Lexer::TokenType::ID, "double", 2, 8));
    EXPECT_EQ(l.getPos(), 3);
}

TEST(LexerTest, throw_on_seek_out_of_bounds)
{
    Lexer::Lexer l("e:double=2.71828;");
    EXPECT_ANY_THROW(l.seek(8));
}

TEST(LexerTest, error_on_unexpected_symbol)
{
    Lexer::Lexer l("e:double=&2.71828;");
    EXPECT_EQ(l.hasError(), true);
    EXPECT_EQ(l.getTokensCount(), 5);
    EXPECT_EQ(l.getAllTokens()[3], Lexer::Token(Lexer::TokenType::EQUAL, "=", 8, 9));
    EXPECT_EQ(l.getAllTokens()[4], Lexer::Token(Lexer::TokenType::INVALID, "&", 9, 10));
}