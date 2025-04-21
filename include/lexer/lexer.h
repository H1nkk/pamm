#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include "lexer_token.h"

namespace Lexer {
    class Lexer final
    {
    private:
        std::string mText;
        std::vector<Token> mTokens;
        size_t mPos;

        void generateTokens();

    public:
        /// <param name="str">Выражение для разбиения на лексемы</param>
        Lexer(const std::string& str) : mText(str), mPos(0)
        {
            generateTokens();
        }

        /// <summary>
        /// Получить обрабатываемое выражение
        /// </summary>
        const std::string& getText() const
        {
            return mText;
        }
        /// <summary>
        /// Получить длину обрабатываемого выражения
        /// </summary>
        size_t getTextSize() const
        {
            return mText.size();
        }

        /// <summary>
        /// Получить все лексемы
        /// </summary>
        const std::vector<Token>& getAllTokens()
        {
            return mTokens;
        }

        /// <summary>
        /// Получить количество лексем в выражении
        /// </summary>
        size_t getTokensCount() const
        {
            return mTokens.size();
        }

        /// <summary>
        /// Получить текущую позицию в списке лексем
        /// </summary>
        size_t getPos() const
        {
            return mPos;
        }

        /// <summary>
        /// Перейти к лексеме по индексу
        /// </summary>
        /// <param name="pos">Индекс лексемы</param>
        size_t seek(size_t pos)
        {
            if (pos >= mTokens.size())
                throw std::out_of_range(__FUNCTION__ ": pos must be between zero and lexem amount.");

            mPos = pos;
        }
        /// <summary>
        /// Получить токен
        /// Позиция в списке сдвигается на следующую лексему (за исключением последней лексемы
        /// </summary>
        /// <returns></returns>
        Token nextToken()
        {
            Token tok = mTokens[mPos];
            if (mPos < mTokens.size() - 1) ++mPos;
            return tok;
        }

        /// <summary>
        /// Проверить, содержится ли ошибка в выражении
        /// </summary>
        bool hasError() const
        {
            return mTokens.back().type() == TokenType::INVALID;
        }
    };
}