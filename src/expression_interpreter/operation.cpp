#include "expression_interpreter/operation.h"

namespace Intr {
    std::string toString(Opcode opc)
    {
        switch (opc)
        {
        case Opcode::LOAD: return "LOAD";
        case Opcode::STORE: return "STORE";
        case Opcode::CALL: return "CALL";
        default: return "UNKNOWN";
        }
    }
}