#pragma once
#include "polynomial.h"
#include "syntax_error.h"
#include <string>
#include "table.h"
#include "lexer/lexer.h"
#include "expression_compiler/expression_compiler.h"
#include "expression_interpreter/expression_interpreter.h"

class PolynomialCalculator final
{
public:
    /// @brief Вычисляет выражение с полиномами
    /// @param expression Выражение
    /// @param pAggregator Агрегатор таблиц
    /// @return polynomial - ответ в виде полинома, SyntaxError - синтакcическая ошибка или std::string - ошибка исполнения
    static std::variant<Polynomial, SyntaxError, std::string> calculate(const std::string& expression, Aggregator* pAggregator)
    {
        Lexer::Lexer lexer(expression);
        auto tokens = lexer.getAllTokens();

        if (lexer.hasError())
        {
            return SyntaxError{ tokens.back().startPos(), "Unexpected symbol" };
        }

        Compiler::ExpressionCompiler comp;
        auto compRes = comp.compileExpression(tokens);

        if (std::holds_alternative<SyntaxError>(compRes))
        {
            return std::get<SyntaxError>(compRes);
        }

        Intr::ExpressionInterpreter intr(pAggregator);
        auto execRes = intr.execute(std::get<Intr::Program>(compRes));

        if (std::holds_alternative<std::string>(execRes))
        {
            return std::get<std::string>(execRes);
        }

        return std::get<Polynomial>(execRes);
    }
};