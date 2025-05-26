#pragma once
#include <vector>
#include <memory>
#include "lexer/lexer_token.h"
#include "expression_interpreter/operation.h"
#include "variables_storage.h"

struct ExecutionContext;

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

class BlockNode : public ExecutionNode {
public:
    BlockNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild)
        : ExecutionNode(pNext, pChild) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~BlockNode() {}
};

class PostfixNode : public ExecutionNode
{
private:
    Intr::Program mProgram;
public:
    PostfixNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild,
        const Intr::Program& program)
        : ExecutionNode(pNext, pChild), mProgram(program) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~PostfixNode() {}
};

class WriteNode : public ExecutionNode {
private:
    std::vector<Lexer::Token> mArgs;
public:
    WriteNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, const std::vector<Lexer::Token>& args)
        : ExecutionNode(pNext, pChild), mArgs(args) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~WriteNode() {}
};

class ReadNode : public ExecutionNode {
private:
    VariableId mId;
public:
    ReadNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, VariableId variableId) : ExecutionNode(pNext, pChild), mId(variableId) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~ReadNode() {}
};

class AssignNode : public ExecutionNode {
private:
    Lexer::Token mName;
    Intr::Program mProg;
public:
    AssignNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild, const Lexer::Token& name, const Intr::Program& prog) 
        : ExecutionNode(pNext, pChild), mName(name), mProg(prog) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~AssignNode() {}
};

class IfNode : public ExecutionNode {
private:
    std::shared_ptr<PostfixNode> mLogicalExpr;
public:
    IfNode(const std::shared_ptr<ExecutionNode>& pNext, const std::shared_ptr<ExecutionNode>& pChild,
        const std::shared_ptr<PostfixNode>& logicalExpr) 
        : ExecutionNode(pNext, pChild), mLogicalExpr(logicalExpr) {}

    virtual void execute(ExecutionContext& context) override;
    virtual ~IfNode() {}
};

