#pragma once
#include <variant>
#include "data_values.h"
#include <string>
#include <vector>

namespace Intr {
    enum class Opcode
    {
        ASSIGN,
        ADD, SUBTRACT, MULT, POWER, UMINUS,
        CALC, DERX, DERY, DERZ, DERW,
        INTX, INTY, INTZ, INTW // TODO: fix
    };

    using Op = std::variant<Opcode, DataValue>;
    using Program = std::vector<Op>;

    std::string toString(Opcode opcode);
}