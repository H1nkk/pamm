#pragma once
#include <variant>
#include <vector>
#include <memory>
#include "lexer/lexer_token.h"
#include "expression_compiler/postfix_compiler.h"
#include "expression_interpreter/execution_context.h"
#include "expression_interpreter/operation.h"
#include "syntax_error.h"

namespace Compiler {
    class ProgramCompiler final
    {
    private:
        ExecutionContext mContext;
        PostfixCompiler mPostfixCompiler;
        std::vector<Lexer::Token> mTokens;
        size_t mCurrentPosition;

        /// <summary>
        /// Returns the position of the closing token.
        /// The closing token is of type COMMA, RPAREN, END, SEMICOLON
        /// It can also be ENDOFFILE OR INVALID if the end of mTokens is reached.
        /// Function supports parentheses and block counting.
        /// </summary>
        /// <returns>Token position</returns>
        size_t findClosingTokenPosition() const;

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

        bool isVariableName(const std::string& name)
        {
            return mContext.variableStorage().isVariableName(name)
                && !mContext.variableStorage().getInfo(
                    mContext.variableStorage().getId(name).value()
                ).isConstant();
        }

        void initializeContext();

        PostfixCompilationResult compileExpression();

        void parseVariables();
        void parseVariablesRow();
        void parseConstants();
        void parseConstantsRow();

        std::shared_ptr<BlockNode> parseCodeBlock();
        std::shared_ptr<ExecutionNode> parseStatement();
    public:
        ProgramCompiler(const std::vector<Lexer::Token>& tokens) : mTokens(tokens), mCurrentPosition(0), mPostfixCompiler(mContext)
        {
            initializeContext();
        }

        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        std::variant<ExecutionContext, SyntaxError> compileProgram();
    };
}