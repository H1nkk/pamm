#pragma once
#include <string>
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"
#include "execution_node.h"

class ExecutionContext final {
private:
    TypeStorage mTypeStorage;
    VariablesStorage mVariableStorage;
    FunctionStorage mFunctionStorage;

    BlockNode* mRoot;
public:
    ExecutionContext() : mVariableStorage(mTypeStorage), mFunctionStorage(mTypeStorage), mRoot(nullptr) {}

    TypeStorage& typeStorage() { return mTypeStorage; }
    VariablesStorage& variableStorage() { return mVariableStorage; }
    FunctionStorage& functionStorage() { return mFunctionStorage; }
};