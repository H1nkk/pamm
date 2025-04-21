#pragma once
#include <variant>
#include <vector>
#include "operation.h"
#include "execution_context.h"
#include "stack.h"
#include "table.h"

namespace Intr {
    class PostfixInterpreter final
    {
        /// @brief Выполнить программу, сгенерированную парсером
        /// @param program программа для выполнения
        /// @param context контекст исполнения
        /// @return полином-результат или ошибка-строка
        static std::variant<DataValue, std::string> execute(const Program& program, const ExecutionContext& context);
    };
}