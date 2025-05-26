#pragma once
#include "execution_context.h"

namespace Intr
{
    class ProgramInterpreter final
    {
    public:
        static void execute(ExecutionContext& context);
    };
}
