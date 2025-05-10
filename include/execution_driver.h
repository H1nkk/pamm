#pragma once
#include "syntax_error.h"
#include "data_values.h"
#include <variant>
#include <string>
#include "table.h"
#include "lexer/lexer.h"
#include "table.h"

class ExecutionDriver final
{
public:
    static std::variant<Table<std::string, DataValue>, SyntaxError, std::string> execute(const std::string& program);
};