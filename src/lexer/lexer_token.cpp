#include "lexer/lexer_token.h"

namespace Lexer {

    std::string toString(TokenType type)
    {
        return "";
    }

    bool operator==(const Token& l, const Token& r)
    {
        return l.type() == r.type() &&
            l.value() == r.value() &&
            l.startPos() == r.startPos() &&
            l.endPos() == r.endPos();
    }

}