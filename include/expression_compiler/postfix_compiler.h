#pragma once
#include <variant>
#include <vector>
#include "lexer/lexer_token.h"
#include "expression_interpreter/operation.h"
#include "syntax_error.h"

namespace Compiler {
    class PostfixCompiler final
    {
    public:
        /// @brief Преобразовать последовательность токенов в программу для интерпретации
        /// @param tokens Последовательность токенов, которую нужно скомпилировать
        /// @return Программа или синтаксическая ошибка
        std::variant<Intr::Program, SyntaxError> compileExpression(const std::vector<Lexer::Token>& tokens);
    };
}