#pragma once

#include "lexer/lexer_token.h"
#include "data_values.h"

namespace Compiler {

    class PostfixMember
    {
    public:
        enum Type
        {
            NONE,
            END,

            VALUE,
            FUNCCALL,
            LPAREN,
            RPAREN,
            COMMA,
            BINARYOP,
            UNARYOP,
        };

    private:
        Type mType;
        size_t mTokenIndex;

    public:
        PostfixMember(Type type = NONE, size_t tokenIndex = 0) :
            mType(type), mTokenIndex(tokenIndex) {}

        Type type() const { return mType; }
        size_t tokenIndex() const { return mTokenIndex; }
    };
}