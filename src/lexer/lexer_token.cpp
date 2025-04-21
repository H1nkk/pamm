#include "lexer/lexer_token.h"

namespace Lexer {

    std::string toString(TokenType type)
    {
        switch (type)
        {
        case TokenType::NONE: return "TokenType::NONE";
        case TokenType::FLOAT: return "TokenType::FLOAT";
        case TokenType::INT: return "TokenType::INT";
        case TokenType::ID: return "TokenType::ID";
        case TokenType::LPAR: return "TokenType::LPAR";
        case TokenType::RPAR: return "TokenType::RPAR";
        case TokenType::X: return "TokenType::X";
        case TokenType::Y: return "TokenType::Y";
        case TokenType::Z: return "TokenType::Z";
        case TokenType::W: return "TokenType::W";
        case TokenType::CALC: return "TokenType::CALC";
        case TokenType::DERX: return "TokenType::DERX";
        case TokenType::DERY: return "TokenType::DERY";
        case TokenType::DERZ: return "TokenType::DERZ";
        case TokenType::DERW: return "TokenType::DERW";
        case TokenType::INTX: return "TokenType::INTX";
        case TokenType::INTY: return "TokenType::INTY";
        case TokenType::INTZ: return "TokenType::INTZ";
        case TokenType::INTW: return "TokenType::INTW";
        case TokenType::PLUS: return "TokenType::PLUS";
        case TokenType::MINUS: return "TokenType::MINUS";
        case TokenType::MULT: return "TokenType::MULT";
        case TokenType::CARET: return "TokenType::CARET";
        case TokenType::COMMA: return "TokenType::COMMA";
        case TokenType::INVALID: return "TokenType::INVALID";
        case TokenType::ASSIGN: return "TokenType::ASSIGN";
        case TokenType::ENDOFFILE: return "TokenType::ENDOFFILE";
        default: return "UNKNOWN";
        }
    }

    bool operator==(const Token& l, const Token& r)
    {
        return l.type() == r.type() &&
            l.value() == r.value() &&
            l.startPos() == r.startPos() &&
            l.endPos() == r.endPos();
    }

}