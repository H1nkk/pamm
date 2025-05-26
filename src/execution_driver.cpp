#include "execution_driver.h"
#include "expression_compiler/program_compiler.h"
#include "expression_interpreter/program_interpreter.h"

std::variant<Table<std::string, DataValue>, SyntaxError, std::string> ExecutionDriver::execute(const std::string& program)
{
    Lexer::Lexer lexer(program);

    if (lexer.hasError())
    {
        return SyntaxError{ lexer.getAllTokens().back().startPos(), "Invalid token" };
    }

    auto compRes = Compiler::ProgramCompiler(lexer.getAllTokens()).compileProgram();

    if (std::holds_alternative<SyntaxError>(compRes))
    {
        return std::get<SyntaxError>(compRes);
    }

    ExecutionContext ctx = std::get<ExecutionContext>(compRes);

    Intr::ProgramInterpreter::execute(ctx);

    return Table<std::string, DataValue>();
}
