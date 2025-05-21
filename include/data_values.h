#pragma once
#include <variant>
#include <string>
#include <vector>
#include "lexer/lexer_token.h"

using DataValue = std::variant<long long, double>;

enum class OpAssociativity {
    LEFTTORIGHT,
    RIGHTTOLEFT
};

class DataType final {
private:
    std::string mName;
public:
    DataType(const std::string& name) : mName(name) {}

    std::string name() const { return mName; }

    bool operator==(const DataType& other) const { return mName == other.mName; }
};

class BinaryOperatorInfo final {
private:
    Lexer::TokenType mOperator;
    DataType mLeftType;
    DataType mRightType;
    DataType mReturnType;
    int mPrecedence;
    OpAssociativity mAssociativity;

public:
    BinaryOperatorInfo(Lexer::TokenType op, const DataType& leftType, const DataType& rightType,
        const DataType& retType, int precedence, OpAssociativity associativity) :
        mOperator(op), mLeftType(leftType), mRightType(rightType), mReturnType(retType),
        mPrecedence(precedence), mAssociativity(associativity) {}

    Lexer::TokenType opType() const { return mOperator; }
    DataType leftType() const { return mLeftType; }
    DataType rightType() const { return mRightType; }
    DataType returnType() const { return mReturnType; }
    int precedence() const { return mPrecedence; }
    OpAssociativity associativity() const { return mAssociativity; }
};

class UnaryOperatorInfo final {
private:
    Lexer::TokenType mOperator;
    DataType mInputType;
    DataType mOutputType;

public:
    UnaryOperatorInfo(Lexer::TokenType op, const DataType& inputType, const DataType& outputType) :
        mOperator(op), mInputType(inputType), mOutputType(outputType) {}

    Lexer::TokenType opType() const { return mOperator; }
    DataType inputType() const { return mInputType; }
    DataType outputType() const { return mOutputType; }
};

class TypeCastingInfo final {
private:
    DataType mInputType;
    DataType mOutputType;

public:
    TypeCastingInfo(const DataType& inputType, const DataType& outputType) :
        mInputType(inputType), mOutputType(outputType) {}

    DataType inputType() const { return mInputType; }
    DataType outputType() const { return mOutputType; }
};

class FunctionInfo final {
private:
    std::string mName;
    DataType mReturnType;
    std::vector<DataType> mArgumentTypes;

public:
    FunctionInfo(const std::string& name, const DataType& retType,
        const std::vector<DataType> argTypes) :
        mName(name), mReturnType(retType), mArgumentTypes(argTypes) {}

    std::string name() const { return mName; }
    DataType returnType() const { return mReturnType; }
    
    size_t argumentsCount() const { return mArgumentTypes.size(); }
    DataType getArgument(size_t index) const { return mArgumentTypes[index]; }
};