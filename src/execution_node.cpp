#include "execution_node.h"
#include "expression_interpreter/postfix_interpreter.h"

void BlockNode::execute(ExecutionContext& context)
{
    std::shared_ptr<ExecutionNode> cur = getChildrenRoot();

    while (cur != nullptr)
    {
        cur->execute(context);
        cur = cur->getNext();
    }
}

void WriteNode::execute(ExecutionContext& context)
{
    for (const auto& prog : mArgs)
    {
        Intr::PostfixInterpreter intr;
        auto res = intr.execute(prog, context);

        if (std::holds_alternative<std::string>(res))
            throw res;

        DataValue val = std::get<DataValue>(res);
        if (std::holds_alternative<double>(val))
            std::cout << std::get<double>(val);
        else if (std::holds_alternative<long long>(val))
            std::cout << std::get<long long>(val);
        else if (std::holds_alternative<bool>(val))
            std::cout << std::get<bool>(val);
        else if (std::holds_alternative<std::string>(val))
            std::cout << std::get<std::string>(val);
        else
            throw std::runtime_error(__FUNCTION__ ": data type is not supported.");
    }

    if (mAddNewLine) std::cout << std::endl;
}

void ReadNode::execute(ExecutionContext& context)
{
    DataTypeId typeId = context.variableStorage().getInfo(mId).type();
    std::string typeName = context.typeStorage().getTypeInfo(typeId).value().name();

    if (typeName == "integer")
    {
        long long val;
        std::cin >> val;
        context.variableStorage().setValue(mId, val);
    } else if (typeName == "double")
    {
        double val;
        std::cin >> val;
        context.variableStorage().setValue(mId, val);
    } else if (typeName == "bool")
    {
        bool val;
        std::cin >> val;
        context.variableStorage().setValue(mId, val);
    } else if (typeName == "string")
    {
        std::string val;
        std::cin >> val;
        context.variableStorage().setValue(mId, val);
    } else
        throw std::runtime_error(__FUNCTION__ ": unknown type.");
}

void IfNode::execute(ExecutionContext& context)
{
    Intr::PostfixInterpreter intr;
    auto res = intr.execute(mLogicalExpr, context);

    if (std::holds_alternative<std::string>(res))
        throw res;

    bool val = std::get<bool>(std::get<DataValue>(res));
    
    std::shared_ptr<ExecutionNode> first = getChildrenRoot();
    std::shared_ptr<ExecutionNode> second = first->getNext();

    if (val) first->execute(context);
    else if (second != nullptr) second->execute(context);
}

void AssignNode::execute(ExecutionContext& context)
{
    Intr::PostfixInterpreter intr;
    auto res = intr.execute(mProg, context);

    if (std::holds_alternative<std::string>(res))
        throw res;

    DataValue val = std::get<DataValue>(res);
    if (std::holds_alternative<double>(val))
        context.variableStorage().setValue(mId, std::get<double>(val));
    else if (std::holds_alternative<long long>(val))
        context.variableStorage().setValue(mId, std::get<long long>(val));
    else if (std::holds_alternative<bool>(val))
        context.variableStorage().setValue(mId, std::get<bool>(val));
    else if (std::holds_alternative<std::string>(val))
        context.variableStorage().setValue(mId, std::get<std::string>(val));
    else
        throw std::runtime_error(__FUNCTION__ ": data type is not supported.");
}
