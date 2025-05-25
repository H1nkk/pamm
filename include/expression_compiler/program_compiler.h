#pragma once
#include <variant>
#include <vector>
#include <memory>
#include "lexer/lexer_token.h"
#include "expression_interpreter/execution_context.h"
#include "expression_interpreter/operation.h"
#include "syntax_error.h"

namespace Compiler {
    class ProgramCompiler final
    {
    private:
        ExecutionContext mContext;
        std::vector<Lexer::Token> mTokens;
        size_t mCurrentPosition;

        bool isToken(Lexer::TokenType type) const
        {
            return mTokens[mCurrentPosition].type() == type;
        }

        void matchToken(Lexer::TokenType type)
        {
            if (isToken(type))
            {
                mCurrentPosition++;
            }
            else
            {
                throw SyntaxError{ mTokens[mCurrentPosition].startPos(), "Expected '" + toString(type) + "' token" };
            }
        }

        Lexer::Token& curToken() { return mTokens[mCurrentPosition]; }

        bool isTypeName(const std::string& name)
        {
            return mContext.typeStorage().isTypeName(name);
        }

        bool isFreeName(const std::string& name)
        {
            return !mContext.typeStorage().isTypeName(name)
                && !mContext.functionStorage().isFunctionName(name)
                && !mContext.variableStorage().isVariableName(name);
        }

        void initializeContext();

        void parseVariables();
        void parseVariablesRow();
        void parseConstants();
        void parseConstantsRow();

        std::shared_ptr<BlockNode> parseCodeBlock();
    public:
        ProgramCompiler(const std::vector<Lexer::Token>& tokens) : mTokens(tokens), mCurrentPosition(0)
        {
            initializeContext();
        }

        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        std::variant<ExecutionContext, SyntaxError> compileProgram();
    };
}