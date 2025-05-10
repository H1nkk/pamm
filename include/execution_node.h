#pragma once
#include <vector>
#include "lexer/lexer_token.h"
#include "expression_interpreter/operation.h"

struct ExecutionContext;

class ExecutionNode {
private:
    ExecutionNode* mpNext;
    ExecutionNode* mpChild;
public:
    ExecutionNode(ExecutionNode* pNext, ExecutionNode* pChild) : mpNext(pNext), mpChild(pChild) {}

    ExecutionNode* getNext() const {
        return mpNext;
    }

    ExecutionNode* getChildrenRoot() const {
        return mpChild;
    }

    virtual void execute(const ExecutionContext& context) = 0;
    virtual ~ExecutionNode() = 0 {}
};

class BlockNode : public ExecutionNode {
public:
    BlockNode(ExecutionNode* pNext, ExecutionNode* pChild)
        : ExecutionNode(pNext, pChild) {}

    virtual void execute(const ExecutionContext& context) override;
    virtual ~BlockNode() {}
};

class WriteNode : public ExecutionNode {
private:
    std::vector<Lexer::Token> mArgs;
public:
    WriteNode(ExecutionNode* pNext, ExecutionNode* pChild, const std::vector<Lexer::Token>& args)
        : ExecutionNode(pNext, pChild), mArgs(args) {}

    virtual void execute(const ExecutionContext& context) override;
    virtual ~WriteNode() {}
};

class ReadNode : public ExecutionNode {
private:
    Lexer::Token mArg;
public:
    ReadNode(ExecutionNode* pNext, ExecutionNode* pChild, const Lexer::Token& arg) : ExecutionNode(pNext, pChild), mArg(arg) {}

    virtual void execute(const ExecutionContext& context) override;
    virtual ~ReadNode() {}
};

class AssignNode : public ExecutionNode {
private:
    Lexer::Token mName;
    Intr::Program mProg;
public:
    AssignNode(ExecutionNode* pNext, ExecutionNode* pChild, const Lexer::Token& name, const Intr::Program& prog) 
        : ExecutionNode(pNext, pChild), mName(name), mProg(prog) {}

    virtual void execute(const ExecutionContext& context) override;
    virtual ~AssignNode() {}
};

class IfNode : public ExecutionNode {
private:
    Intr::Program mExpr;
public:
    IfNode(ExecutionNode* pNext, ExecutionNode* pChild, const Intr::Program& expr) 
        : ExecutionNode(pNext, pChild), mExpr(expr) {}

    virtual void execute(const ExecutionContext& context) override;
    virtual ~IfNode() {}
};

