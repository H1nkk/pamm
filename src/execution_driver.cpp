#include "execution_driver.h"

std::variant<Table<std::string, DataValue>, SyntaxError, std::string> ExecutionDriver::execute(const std::string& program)
{
    return std::variant<Table<std::string, DataValue>, SyntaxError, std::string>();
}
