#include "expression_interpreter/program_interpreter.h"
#include "execution_node.h"

void Intr::ProgramInterpreter::execute(ExecutionContext& context)
{
    context.root()->execute(context);
}
