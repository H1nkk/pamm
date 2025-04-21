#pragma once
#include "execution_context.h"

namespace Intr
{
    class ProgramInterpreter final
    {
        static void execute(const ExecutionContext& context);
    };
}
