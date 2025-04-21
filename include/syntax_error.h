#pragma once
#include <string>

struct SyntaxError
{
    size_t pos;
    std::string message;
};