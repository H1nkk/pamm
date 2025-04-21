#include "expression_interpreter/operation.h"

namespace Intr {
    std::string toString(Opcode opc)
    {
        switch (opc)
        {
        case Opcode::ADD: return "Opcode::ADD";
        case Opcode::ASSIGN: return "Opcode::ASSIGN";
        case Opcode::CALC: return "Opcode::CALC";
        case Opcode::DERW: return "Opcode::DERW";
        case Opcode::DERX: return "Opcode::DERX";
        case Opcode::DERY: return "Opcode::DERY";
        case Opcode::DERZ: return "Opcode::DERZ";
        case Opcode::INTW: return "Opcode::INTW";
        case Opcode::INTX: return "Opcode::INTX";
        case Opcode::INTY: return "Opcode::INTY";
        case Opcode::INTZ: return "Opcode::INTZ";
        case Opcode::MULT: return "Opcode::MULT";
        case Opcode::POWER: return "Opcode::POWER";
        case Opcode::SUBTRACT: return "Opcode::SUBTRACT";
        case Opcode::UMINUS: return "Opcode::UMINUS";
        default: return "UNKNOWN";
        }
    }
}