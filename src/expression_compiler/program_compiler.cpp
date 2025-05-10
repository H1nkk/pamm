#include "expression_compiler/program_compiler.h"

std::variant<ExecutionContext, SyntaxError> Compiler::PostfixCompiler::compileProgram(const std::vector<Lexer::Token>& tokens)
{
    return std::variant<ExecutionContext, SyntaxError>();
}
