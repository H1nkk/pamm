#pragma once
#include <variant>
#include <vector>
#include "lexer/lexer_token.h"
#include "expression_interpreter/execution_context.h"
#include "expression_interpreter/operation.h"
#include "syntax_error.h"

namespace Compiler {
    struct PostfixCompilationResult
    {
        DataTypeId resultType;
        Intr::Program program;
    };

    class PostfixCompiler final
    {
    private:
        ExecutionContext& mExecContext;

    public:
        PostfixCompiler(ExecutionContext& ctx) : mExecContext(ctx) {}

        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        std::variant<PostfixCompilationResult, SyntaxError> compileExpression(const std::vector<Lexer::Token>& tokens);
    };
}