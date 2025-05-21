#include "lexer/lexer_token.h"

namespace Lexer {

    std::string toString(TokenType type)
    {
        switch (type)
        {
        case TokenType::NONE: return "NONE";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::INT: return "INT";
        case TokenType::ID: return "ID";
        case TokenType::STRING: return "STRING";
        case TokenType::PROGRAM: return "PROGRAM";
        case TokenType::CONST: return "CONST";
        case TokenType::VAR: return "VAR";
        case TokenType::BEGIN: return "BEGIN";
        case TokenType::END: return "END";
        case TokenType::DOT: return "DOT";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COLON: return "COLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULT: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::DIVINT: return "DIVINT";
        case TokenType::MOD: return "MOD";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::LESSEQUAL: return "LESSEQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATEREQUAL: return "GREATEREQUAL";
        case TokenType::NOTEQUAL: return "NOTEQUAL";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::INVALID: return "INVALID";
        case TokenType::ENDOFFILE: return "ENDOFFILE";
        }
        return "UNKNOWN"; // Fallback for any unhandled cases
    }


    bool operator==(const Token& l, const Token& r)
    {
        return l.type() == r.type() &&
            l.value() == r.value() &&
            l.startPos() == r.startPos() &&
            l.endPos() == r.endPos();
    }

}