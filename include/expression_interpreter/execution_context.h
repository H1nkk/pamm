#pragma once
#include <string>
#include <memory>
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"
#include "execution_node.h"

class ExecutionContext final {
private:
    TypeStorage mTypeStorage;
    VariablesStorage mVariableStorage;
    FunctionStorage mFunctionStorage;

    std::shared_ptr<BlockNode> mRoot;
public:
    ExecutionContext() : mVariableStorage(mTypeStorage), mFunctionStorage(mTypeStorage), mRoot(nullptr) {}

    void setRoot(const std::shared_ptr<BlockNode>& root)
    {
        mRoot = root;
    }

    TypeStorage& typeStorage() { return mTypeStorage; }
    VariablesStorage& variableStorage() { return mVariableStorage; }
    FunctionStorage& functionStorage() { return mFunctionStorage; }
    std::shared_ptr<BlockNode> root() { return mRoot; }
};