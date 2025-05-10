#include "expression_interpreter/postfix_interpreter.h"

std::variant<DataValue, std::string> Intr::PostfixInterpreter::execute(const Program& program, const ExecutionContext& context)
{
    return std::variant<DataValue, std::string>();
}
