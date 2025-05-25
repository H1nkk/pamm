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
    private:
        Stack<DataValue> mStack;

        template<typename T>
        T popValue()
        {
            T val = std::get<T>(mStack.top());
            mStack.pop();
            return val;
        }

        void pushValue(const DataValue& val)
        {
            mStack.push(val);
        }
    public:
        enum Opcodes
        {
            ADD_INT_INT = 0,
            ADD_DOUBLE_DOUBLE,
            SUB_INT_INT,
            SUB_DOUBLE_DOUBLE,
            MULT_INT_INT,
            MULT_DOUBLE_DOUBLE,
            DIV_INT_INT,
            MOD_INT_INT,
            DIV_DOUBLE_DOUBLE,
            NEGATE_INT,
            NEGATE_DOUBLE,
            
            LESS_INT,
            LESS_DOUBLE,
            GREATER_INT,
            GREATER_DOUBLE,
            LESSEQUAL_INT,
            LESSEQUAL_DOUBLE,
            GREATEREQUAL_INT,
            GREATEREQUAL_DOUBLE,
            EQUAL_INT,
            EQUAL_DOUBLE,
            EQUAL_BOOL,
            NOTEQUAL_INT,
            NOTEQUAL_DOUBLE,
            NOT_EQUAL_BOOL,
            
            AND_BOOL_BOOL,
            OR_BOOL_BOOL,
            NOT_BOOL
        };

        /// @brief Выполнить программу, сгенерированную парсером
        /// @param program программа для выполнения
        /// @param context контекст исполнения
        /// @return результат или ошибка-строка
        std::variant<DataValue, std::string> execute(const Program& program, ExecutionContext& context);
    };
}