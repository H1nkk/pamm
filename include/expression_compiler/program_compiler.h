#pragma once
#include <variant>
#include <vector>
#include "lexer/lexer_token.h"
#include "expression_interpreter/execution_context.h"
#include "expression_interpreter/operation.h"
#include "syntax_error.h"

namespace Compiler {
    class PostfixCompiler final
    {
    public:
        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        static std::variant<ExecutionContext, SyntaxError> compileProgram(const std::vector<Lexer::Token>& tokens);
    };
}