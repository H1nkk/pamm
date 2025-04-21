#pragma once
#include <string>
#include "table.h"
#include "data_values.h"
#include "execution_node.h"

struct ExecutionContext {
    Table<std::string, DataValue> constsTable;
    Table<std::string, DataValue> variablesTable;

    BlockNode* root;
};