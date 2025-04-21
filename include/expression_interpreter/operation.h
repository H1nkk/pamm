#pragma once
#include <variant>
#include <string>
#include "polynomial.h"

namespace Intr {
    enum class Opcode
    {
        ASSIGN,
        ADD, SUBTRACT, MULT, POWER, UMINUS,
        CALC, DERX, DERY, DERZ, DERW,
        INTX, INTY, INTZ, INTW
    };

    // Код операции/полином/идентификатор/число
    using Op = std::variant<Opcode, Polynomial, std::string, double, unsigned long>;

    std::string toString(Opcode opcode);
}