#pragma once

#include "lexer/lexer_token.h"

namespace Compiler {
    class PostfixMember
    {
    public:
        enum OpAssociativity
        {
            LEFTTORIGHT,
            RIGHTTOLEFT
        };

        explicit PostfixMember(Lexer::Token token = {}, int precedence = 0, bool prefixOp = false, OpAssociativity assoc = LEFTTORIGHT) :
            mToken(token), mPrecedence(precedence), mPrefixOp(prefixOp), mAssociativity(assoc)
        {}

        Lexer::Token token() const noexcept { return mToken; }
        Lexer::TokenType type() const noexcept { return mToken.type(); }
        int precedence() const noexcept { return mPrecedence; }
        bool isPrefixOp() const noexcept { return mPrefixOp; }
        OpAssociativity associativity() const noexcept { return mAssociativity; }

    private:
        Lexer::Token mToken;
        int mPrecedence;
        bool mPrefixOp;
        OpAssociativity mAssociativity;
    };
}