#include "lexer/lexer.h"
#include <algorithm>
#include <unordered_map>


namespace Lexer {

    static const std::unordered_map<char, TokenType> specialSymbols = {
        { '+', TokenType::PLUS },
        { '-', TokenType::MINUS },
        { '*', TokenType::MULT },
        { '.', TokenType::DOT },
        { ',', TokenType::COMMA },
        { ';', TokenType::SEMICOLON },
        { ':', TokenType::COLON },
        { '(', TokenType::LPAREN },
        { ')', TokenType::RPAREN },
        { '/', TokenType::DIV },
        { '=', TokenType::EQUAL },
        { '<', TokenType::LESS },
        { '>', TokenType::GREATER }
    };

    static const std::unordered_map<std::string, TokenType> compositeSpecialSymbols = {
        { ":=", TokenType::ASSIGN },
        { "<=", TokenType::LESSEQUAL },
        { ">=", TokenType::GREATEREQUAL },
        { "<>", TokenType::NOTEQUAL },
    };

    static const std::unordered_map<std::string, TokenType> keywords = {
        { "program", TokenType::PROGRAM },
        { "const", TokenType::CONST },
        { "var", TokenType::VAR },
        { "begin", TokenType::BEGIN },
        { "end", TokenType::END },
        { "Read", TokenType::READ },
        { "Write", TokenType::WRITE },
        { "WriteLn", TokenType::WRITELN },
        { "True", TokenType::TRUE },
        { "False", TokenType::FALSE },
        { "if", TokenType::IF },
        { "then", TokenType::THEN },
        { "else", TokenType::ELSE },
        { "div", TokenType::DIVINT },
        { "mod", TokenType::MOD },
        { "and", TokenType::AND },
        { "or", TokenType::OR },
        { "not", TokenType::NOT },
    };

    void Lexer::generateTokens()
    {
        mTokens.clear();

        size_t index = 0;

        while (mTokens.size() == 0 ||
            (mTokens.back().type() != TokenType::ENDOFFILE &&
                mTokens.back().type() != TokenType::INVALID))
        {
            while (index < mText.size() && isspace(mText[index]))
                ++index;
            if (index >= mText.size())
            {
                // Выражение закончилось
                mTokens.push_back(Token(TokenType::ENDOFFILE, "", index, index));
            }
            else if (specialSymbols.find(mText[index]) != specialSymbols.end()) // найденный элемент находится в map
            {
                if (index + 1 < mText.size()) {
                    if (compositeSpecialSymbols.find(mText.substr(index, 2)) != compositeSpecialSymbols.end())
                    {
                        TokenType type = compositeSpecialSymbols.at(mText.substr(index, 2));
                        Token tok = Token(type, mText.substr(index, 2), index, index + 2);
                        mTokens.push_back(tok);
                        index += 2;
                    }
                    else
                    {
                        TokenType type = specialSymbols.at(mText[index]);
                        Token tok = Token(type, std::string(1, mText[index]), index, index + 1);
                        mTokens.push_back(tok);
                        ++index;
                    }
                }
                else
                {
                    TokenType type = specialSymbols.at(mText[index]);
                    Token tok = Token(type, std::string(1, mText[index]), index, index + 1);
                    mTokens.push_back(tok);
                    ++index;
                }
            }
            else if (isdigit(mText[index]))
            {
                // Найдено число
                std::string val;
                size_t delimeterCount = 0;

                while (index < mText.size() &&
                    (mText[index] == '.' || isdigit(mText[index])))
                {
                    val.push_back(mText[index++]);

                    if (val.back() == '.')
                    {
                        ++delimeterCount;
                        if (delimeterCount >= 2) break;
                    }
                }

                if (delimeterCount == 0)
                    // Целое число
                    mTokens.push_back(Token(TokenType::INT, val, index - val.size(), index));
                else if (delimeterCount == 1 && val.back() != '.')
                    // Число с плавающей точкой
                    mTokens.push_back(Token(TokenType::FLOAT, val, index - val.size(), index));
                else
                    // Лишняя точка приводит к занесению всего числа в некорректные
                    mTokens.push_back(Token(TokenType::INVALID, val,
                        index - val.size(), index));
            }
            else if (mText[index] == '\'')
            {
                // Найдена строка 
                std::string str;
                bool correctnessStr = false;
                bool nextSymbolIncluded = false;
                str.push_back(mText[index++]);
                while (index < mText.size())
                {
                    if (mText[index] == '\\')
                    {
                        nextSymbolIncluded = true;
                        str.push_back(mText[index++]);
                    }
                    else if (nextSymbolIncluded)
                    {
                        nextSymbolIncluded = false;
                        str.push_back(mText[index++]);
                    }
                    else if (mText[index] != '\'')
                        str.push_back(mText[index++]);
                    else
                    {
                        correctnessStr = true;
                        str.push_back(mText[index++]);
                        break;
                    }
                }

                if (correctnessStr)
                    mTokens.push_back(Token(TokenType::STRING, str, index - str.size(), index));
                else
                    mTokens.push_back(Token(TokenType::INVALID, str, index - str.size(), index));
            }
            else if (mText[index] == '_' || isalpha(mText[index]))
            {
                std::string id;
                while (index < mText.size() &&
                    (mText[index] == '_' || isalnum(mText[index])))
                    id.push_back(mText[index++]);

                if (keywords.find(id) != keywords.end())
                {
                    TokenType type = keywords.at(id);
                    Token tok = Token(type, id, index - id.size(), index);
                    mTokens.push_back(tok);
                }
                else
                    mTokens.push_back(Token(TokenType::ID, id, index - id.size(), index));

            }
            else
                mTokens.push_back(Token(TokenType::INVALID, std::string(1, mText[index]), index, index + 1));



        }
    }


}



