#include "lexer/lexer.h"
#include <algorithm>
#include <unordered_map>


namespace Lexer {

    static const std::unordered_map<char, TokenType> specialSymbols = {
        { '+', TokenType::PLUS },
        { '-', TokenType::MINUS },
        { '*', TokenType::MULT },
        { '^', TokenType::CARET },
        { '(', TokenType::LPAR },
        { ')', TokenType::RPAR },
        { '=', TokenType::ASSIGN },
        { ',', TokenType::COMMA }
    };

    static const std::unordered_map<std::string, TokenType> functionNames = {
        { "calc", TokenType::CALC },
        { "derx", TokenType::DERX },
        { "dery", TokenType::DERY },
        { "derz", TokenType::DERZ },
        { "derw", TokenType::DERW },
        { "intx", TokenType::INTX },
        { "inty", TokenType::INTY },
        { "intz", TokenType::INTZ },
        { "intw", TokenType::INTW },
    };

    void Lexer::generateTokens()
    {
        mTokens.clear();

        size_t index = 0;

        while (mTokens.size() == 0 ||
            (mTokens.back().type() != TokenType::ENDOFFILE &&
                mTokens.back().type() != TokenType::INVALID))
        {
            // Пропустить все пробелы
            while (index < mText.size() && isspace(mText[index]))
                ++index;

            if (index >= mText.size())
            {
                // Выражение закончилось
                mTokens.push_back(Token(TokenType::ENDOFFILE, "", index, index));
            } else if (specialSymbols.find(mText[index]) != specialSymbols.end())
            {
                // Найден оператор
                TokenType type = specialSymbols.at(mText[index]);
                Token tok = Token(type, std::string(1, mText[index]), index, index + 1);
                mTokens.push_back(tok);
                ++index;
            } else if (mText[index] == '_' || isalpha(mText[index]))
            {
                // Найден идентикатор или переменные монома
                std::string id;
                while (index < mText.size() &&
                    (mText[index] == '_' || isalnum(mText[index])))
                    id.push_back(mText[index++]);

                if (!std::all_of(id.begin(), id.end(), [](char c)
                    {
                        char lc = tolower(c);
                        return lc == 'x' || lc == 'y' || lc == 'z' || lc == 'w' || isdigit(lc);
                    }))
                {
                    // Идентификатор - название полинома или функции
                    TokenType type = TokenType::ID;
                    auto funcIter = functionNames.find(id);
                    if (funcIter != functionNames.end())
                        type = funcIter->second;

                    mTokens.push_back(Token(type, id, index - id.size(), index));
                } else
                    {
                        // Добавляем переменные монома по отдельности
                        size_t start = index - id.size();
                        for (size_t i = 0; i < id.size(); i++)
                        {
                            char lc = tolower(id[i]);
                            if (lc == 'x')
                                mTokens.push_back(Token(TokenType::X, std::string(1, id[i]),
                                    start + i, start + i + 1));
                            else if (lc == 'y')
                                mTokens.push_back(Token(TokenType::Y, std::string(1, id[i]),
                                    start + i, start + i + 1));
                            else if (lc == 'z')
                                mTokens.push_back(Token(TokenType::Z, std::string(1, id[i]),
                                    start + i, start + i + 1));
                            else if (lc == 'w')
                                mTokens.push_back(Token(TokenType::W, std::string(1, id[i]),
                                    start + i, start + i + 1));
                            else if (isdigit(lc))
                            {
                                size_t len = 0;
                                for (; i + len < id.size() && isdigit(id[i + len]); ++len);
                                mTokens.push_back(Token(TokenType::INT, id.substr(i, len), start + i, start + i + len));
                                i += len - 1;
                            }
                        }
                    }
            } else if (mText[index] == '.' || isdigit(mText[index]))
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
                else if (delimeterCount == 1 && val.size() > 1)
                    // Число с плавающей точкой (не просто точка)
                    mTokens.push_back(Token(TokenType::FLOAT, val, index - val.size(), index));
                else
                    // Лишняя точка
                    mTokens.push_back(Token(TokenType::INVALID, std::string(1, val.back()),
                        index - 1, index));
            } else
            {
                // Такой лексемы не существует
                mTokens.push_back(Token(TokenType::INVALID, std::string(1, mText[index]), index, index + 1));
            }
        }
    }
}

