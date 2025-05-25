#pragma once
#include <variant>
#include <string>
#include <vector>
#include "lexer/lexer_token.h"

using DataValue = std::variant<long long, double, bool, char*>;