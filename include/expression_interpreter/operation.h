#pragma once
#include <variant>
#include "data_values.h"
#include <string>
#include <vector>

namespace Intr {
    enum class Opcode
    {
        LOAD, STORE, CALL
    };

    using Op = std::pair<Opcode, unsigned long long>;
    using Program = std::vector<Op>;

    std::string toString(Opcode opcode);
}