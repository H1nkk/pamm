#include "expression_compiler/postfix_compiler.h"

std::variant<Intr::Program, SyntaxError> Compiler::PostfixCompiler::compileExpression(const std::vector<Lexer::Token>& tokens)
{
    return std::variant<Intr::Program, SyntaxError>();
}
