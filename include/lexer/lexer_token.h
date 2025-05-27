#pragma once
#include <string>

namespace Lexer {
    enum class TokenType
    {
        NONE,
        FLOAT, INT, ID, STRING,
        PROGRAM,
        CONST, VAR, BEGIN, END, DOT, COMMA, SEMICOLON, COLON,
        READ, WRITE, WRITELN, TRUE, FALSE,
        LPAREN, RPAREN,
        IF, THEN, ELSE,
        ASSIGN, PLUS, MINUS, MULT, DIV, DIVINT, MOD,
        EQUAL, LESS, GREATER, LESSEQUAL, GREATEREQUAL, NOTEQUAL,
        AND, OR, NOT,

        INVALID,
        ENDOFFILE
    };

    std::string toString(TokenType type);

    class Token final
    {
    public:
        Token(TokenType type = TokenType::NONE, const std::string& value = "",
            size_t start = 0, size_t end = 0)
            : mType(type), mValue(value), mStart(start), mEnd(end)
        {}

        TokenType type() const { return mType; }
        std::string value() const { return mValue; }
        size_t startPos() const { return mStart; }
        size_t endPos() const { return mEnd; }

    private:
        TokenType mType;
        std::string mValue;
        size_t mStart, mEnd;
    };

    bool operator==(const Token& l, const Token& r);
}