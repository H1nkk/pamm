#pragma once
#include <vector>
#include <memory>
#include "lexer/lexer_token.h"
#include "expression_interpreter/execution_context.h"
#include "expression_interpreter/operation.h"
#include "variables_storage.h"

class ExecutionNode {
private:
    std::shared_ptr<ExecutionNode> mNext;
    std::shared_ptr<ExecutionNode> mChild;
public:
    ExecutionNode(
        const std::shared_ptr<ExecutionNode>& next, 
        const std::shared_ptr<ExecutionNode>& child
    ) : mNext(next), mChild(child) {}

    void setNext(const std::shared_ptr<ExecutionNode>& val)
    {
        mNext = val;
    }

    std::shared_ptr<ExecutionNode> getNext() const {
        return mNext;
    }

    std::shared_ptr<ExecutionNode> getChildrenRoot() const {
        return mChild;
    }

    virtual void execute(ExecutionContext& context) = 0;
    virtual ~ExecutionNode() = 0 {}
};

class BlockNode final : public ExecutionNode {
public:
    BlockNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild)
        : ExecutionNode(pNext, pChild) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~BlockNode() {}
};

class WriteNode final : public ExecutionNode {
private:
    std::vector<Intr::Program> mArgs;
    bool mAddNewLine;
public:
    WriteNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, const std::vector<Intr::Program>& args, bool addNewLine)
        : ExecutionNode(pNext, pChild), mArgs(args), mAddNewLine(addNewLine) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~WriteNode() {}
};

class ReadNode final : public ExecutionNode {
private:
    VariableId mId;
public:
    ReadNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, VariableId variableId) : ExecutionNode(pNext, pChild), mId(variableId) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~ReadNode() {}
};

class AssignNode final : public ExecutionNode {
private:
    VariableId mId;
    Intr::Program mProg;
public:
    AssignNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, VariableId id, const Intr::Program& prog) 
        : ExecutionNode(pNext, pChild), mId(id), mProg(prog) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~AssignNode() {}
};

class IfNode final : public ExecutionNode {
private:
    Intr::Program mLogicalExpr;
public:
    IfNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild,
        const Intr::Program& logicalExpr) 
        : ExecutionNode(pNext, pChild), mLogicalExpr(logicalExpr) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~IfNode() {}
};

class WhileNode final : public ExecutionNode
{
private:
    Intr::Program mLogicalExpr;
public:
    WhileNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild,
        const Intr::Program& logicalExpr)
        : ExecutionNode(pNext, pChild), mLogicalExpr(logicalExpr)
    {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~WhileNode() {}
};