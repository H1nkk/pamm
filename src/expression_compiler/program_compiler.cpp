#include "expression_compiler/program_compiler.h"
#include "expression_interpreter/postfix_interpreter.h"

using Lexer::Token;
using Lexer::TokenType;

std::variant<ExecutionContext, SyntaxError> Compiler::ProgramCompiler::compileProgram()
{

    try
    {
        // matching program
        matchToken(TokenType::PROGRAM);
        matchToken(TokenType::ID);
        matchToken(TokenType::SEMICOLON);

        // matching vars and consts

        while (isToken(TokenType::CONST) || isToken(TokenType::VAR))
        {
            if (isToken(TokenType::CONST))
            {
                parseConstants();
            } else
            {
                parseVariables();
            }
        }

        if (!isToken(TokenType::BEGIN))
        {
            throw SyntaxError{ curToken().startPos(), "Expected 'begin'" };
        }

        mContext.setRoot(parseCodeBlock());

        matchToken(TokenType::DOT);

        return mContext;
    }
    catch (SyntaxError e)
    {
        return e;
    }
}

std::shared_ptr<BlockNode> Compiler::ProgramCompiler::parseCodeBlock()
{
    matchToken(TokenType::BEGIN);

    std::shared_ptr<ExecutionNode> start = nullptr;
    std::shared_ptr<ExecutionNode> prev = nullptr;

    while (!isToken(TokenType::END))
    {
        std::shared_ptr<ExecutionNode> node = parseStatement();

        if (start == nullptr)
        {
            start = node;
            prev = node;
        } else
        {
            prev->setNext(node);
            prev = node;
        }
    }
    // match some expressions here

    matchToken(TokenType::END);

    return std::make_shared<BlockNode>(nullptr, start);
}

std::shared_ptr<ExecutionNode> Compiler::ProgramCompiler::parseStatement()
{
    if (isToken(TokenType::BEGIN))
    {
        return parseCodeBlock();
    }

    if (isToken(TokenType::READ))
    {
        matchToken(TokenType::READ);
        matchToken(TokenType::LPAREN);

        if (!isToken(TokenType::ID))
            throw SyntaxError{ curToken().startPos(), "Expected variable name" };

        std::string name = curToken().value();
        if (!isVariableName(name))
            throw SyntaxError{ curToken().startPos(), "Undeclared variable" };

        VariableId id = mContext.variableStorage().getId(name).value();

        matchToken(TokenType::ID);

        matchToken(TokenType::RPAREN);
        matchToken(TokenType::SEMICOLON);

        return std::make_shared<ReadNode>(nullptr, nullptr, id);
    }

    if (isToken(TokenType::WRITE) || isToken(TokenType::WRITELN))
    {
        bool addNewLine = isToken(TokenType::WRITELN);

        if (isToken(TokenType::WRITE)) matchToken(TokenType::WRITE);
        else matchToken(TokenType::WRITELN);

        matchToken(TokenType::LPAREN);

        if (isToken(TokenType::RPAREN))
            throw SyntaxError{ curToken().startPos(), "Expected at least one argument" };

        std::vector<Intr::Program> argumentPrograms;

        argumentPrograms.push_back(compileExpression().program);
        while (isToken(TokenType::COMMA))
        {
            matchToken(TokenType::COMMA);
            argumentPrograms.push_back(compileExpression().program);
        }

        matchToken(TokenType::RPAREN);
        matchToken(TokenType::SEMICOLON);

        return std::make_shared<WriteNode>(nullptr, nullptr, argumentPrograms, addNewLine);
    }

    if (isToken(TokenType::ID))
    {
        std::string name = curToken().value();
        if (!isVariableName(name))
            throw SyntaxError{ curToken().startPos(), "Expected variable name" };

        matchToken(TokenType::ID);
        matchToken(TokenType::ASSIGN);

        auto compRes = compileExpression();
        DataTypeId retType = compRes.resultType;
        VariableId varId = mContext.variableStorage().getId(name).value();
        DataTypeId targetType = mContext.variableStorage().getInfo(varId).type();

        if (retType != targetType)
        {
            throw SyntaxError{ curToken().startPos(), "Expected expression of type " +
                mContext.typeStorage().getTypeInfo(targetType).value().name() + " but got " +
                mContext.typeStorage().getTypeInfo(retType).value().name() };
        }

        matchToken(TokenType::SEMICOLON);

        return std::make_shared<AssignNode>(nullptr, nullptr, varId, compRes.program);
    }

    if (isToken(TokenType::IF))
    {
        matchToken(TokenType::IF);
        matchToken(TokenType::LPAREN);

        auto compRes = compileExpression();
        DataTypeId retType = compRes.resultType;
        std::string retTypeName = mContext.typeStorage().getTypeInfo(retType).value().name();
        if (retTypeName != "boolean")
            throw SyntaxError{ curToken().startPos(), "Expected expression return type is boolean, but it is " + retTypeName };

        matchToken(TokenType::RPAREN);
        matchToken(TokenType::THEN);

        std::shared_ptr<ExecutionNode> ifClause = parseStatement();
        std::shared_ptr<ExecutionNode> elseClause = nullptr;

        if (isToken(TokenType::ELSE))
        {
            matchToken(TokenType::ELSE);
            elseClause = parseStatement();
        }

        ifClause->setNext(elseClause);

        return std::make_shared<IfNode>(nullptr, ifClause, compRes.program);
    }

    throw SyntaxError{ curToken().startPos(), "Expected statement" };
}

void Compiler::ProgramCompiler::initializeContext()
{
    DataType intType("integer", sizeof(long long));
    DataTypeId intId = mContext.typeStorage().registerType(intType);
    DataType doubleType("double", sizeof(double));
    DataTypeId doubleId = mContext.typeStorage().registerType(doubleType);
    DataType boolType("boolean", sizeof(bool));
    DataTypeId boolId = mContext.typeStorage().registerType(boolType);

    mContext.functionStorage().registerFunction({
        "$operator+", intId, { intId, intId }, Intr::PostfixInterpreter::ADD_INT_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator+", doubleId, { doubleId, doubleId }, Intr::PostfixInterpreter::ADD_DOUBLE_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator-", intId, { intId, intId }, Intr::PostfixInterpreter::SUB_INT_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator-", doubleId, { doubleId, doubleId }, Intr::PostfixInterpreter::SUB_DOUBLE_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator*", intId, { intId, intId }, Intr::PostfixInterpreter::MULT_INT_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator*", doubleId, { doubleId, doubleId }, Intr::PostfixInterpreter::MULT_DOUBLE_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operatordiv", intId, { intId, intId }, Intr::PostfixInterpreter::DIV_INT_INT
        });
    mContext.functionStorage().registerFunction({
        "$operatormod", intId, { intId, intId }, Intr::PostfixInterpreter::MOD_INT_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator/", doubleId, { doubleId, doubleId }, Intr::PostfixInterpreter::DIV_DOUBLE_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator-", intId, { intId }, Intr::PostfixInterpreter::NEGATE_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator-", doubleId, { doubleId }, Intr::PostfixInterpreter::NEGATE_DOUBLE
        });

    mContext.functionStorage().registerFunction({
        "$operator<", boolId, { intId, intId }, Intr::PostfixInterpreter::LESS_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator<", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::LESS_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator<=", boolId, { intId, intId }, Intr::PostfixInterpreter::LESSEQUAL_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator<=", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::LESSEQUAL_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator>", boolId, { intId, intId }, Intr::PostfixInterpreter::GREATER_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator>", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::GREATER_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator>=", boolId, { intId, intId }, Intr::PostfixInterpreter::GREATEREQUAL_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator>=", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::GREATEREQUAL_DOUBLE
        });

    mContext.functionStorage().registerFunction({
        "$operator=", boolId, { intId, intId }, Intr::PostfixInterpreter::EQUAL_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator=", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::EQUAL_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator=", boolId, { boolId, boolId }, Intr::PostfixInterpreter::EQUAL_BOOL
        });
    mContext.functionStorage().registerFunction({
        "$operator<>", boolId, { intId, intId }, Intr::PostfixInterpreter::NOTEQUAL_INT
        });
    mContext.functionStorage().registerFunction({
        "$operator<>", boolId, { doubleId, doubleId }, Intr::PostfixInterpreter::NOTEQUAL_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "$operator<>", boolId, { boolId, boolId }, Intr::PostfixInterpreter::NOTEQUAL_BOOL
        });

    mContext.functionStorage().registerFunction({
        "$operatorand", boolId, { boolId, boolId }, Intr::PostfixInterpreter::AND_BOOL_BOOL
        });
    mContext.functionStorage().registerFunction({
        "$operatoror", boolId, { boolId, boolId }, Intr::PostfixInterpreter::OR_BOOL_BOOL
        });
    mContext.functionStorage().registerFunction({
        "$operatornot", boolId, { boolId }, Intr::PostfixInterpreter::NOT_BOOL
        });

    mContext.functionStorage().registerFunction({
        "$cast", doubleId, { intId }, Intr::PostfixInterpreter::CAST_INT_DOUBLE
        });

    mContext.functionStorage().registerFunction({
        "abs", intId, { intId }, Intr::PostfixInterpreter::ABS_INT
        });
    mContext.functionStorage().registerFunction({
        "abs", doubleId, { doubleId }, Intr::PostfixInterpreter::ABS_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "exp", doubleId, { doubleId }, Intr::PostfixInterpreter::EXP_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "sin", doubleId, { doubleId }, Intr::PostfixInterpreter::SIN_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "cos", doubleId, { doubleId }, Intr::PostfixInterpreter::COS_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "floor", intId, { doubleId }, Intr::PostfixInterpreter::FLOOR_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "ceil", intId, { doubleId }, Intr::PostfixInterpreter::CEIL_DOUBLE
        });
    mContext.functionStorage().registerFunction({
        "round", intId, { doubleId }, Intr::PostfixInterpreter::ROUND_DOUBLE
        });
}

void Compiler::ProgramCompiler::parseVariables()
{
    matchToken(TokenType::VAR);

    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected variable name" };

    while (isToken(TokenType::ID))
        parseVariablesRow();
}

void Compiler::ProgramCompiler::parseVariablesRow()
{
    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected variable name" };

    std::vector<std::pair<std::string, size_t>> names;
    names.push_back({ curToken().value(), curToken().startPos() });

    matchToken(TokenType::ID);

    while (isToken(TokenType::COMMA))
    {
        matchToken(TokenType::COMMA);

        if (!isToken(TokenType::ID))
            throw SyntaxError{ curToken().startPos(), "Expected variable name" };

        names.push_back({ curToken().value(), curToken().startPos() });
        matchToken(TokenType::ID);
    }

    matchToken(TokenType::COLON);

    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected type" };

    std::string typeName = curToken().value();

    if (!isTypeName(typeName))
        throw SyntaxError{ curToken().startPos(), "Invalid type" };

    DataTypeId type = mContext.typeStorage().getTypeId(typeName).value();

    matchToken(TokenType::ID);
    matchToken(TokenType::SEMICOLON);

    for (const auto& entry : names)
    {
        if (!isFreeName(entry.first))
            throw SyntaxError{ entry.second, "This name is already taken" };

        VariableInfo info(entry.first, type, false);

        if (typeName == "integer")
            mContext.variableStorage().registerVariable<long long>(info, 0LL);
        else if (typeName == "double")
            mContext.variableStorage().registerVariable<double>(info, 0.0);
        else
            throw SyntaxError{ curToken().startPos(), "Expected integer or double type" };
    }
}

void Compiler::ProgramCompiler::parseConstants()
{
    matchToken(TokenType::CONST);

    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected constant name" };

    while (isToken(TokenType::ID))
        parseConstantsRow();
}

void Compiler::ProgramCompiler::parseConstantsRow()
{
    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected constant name" };

    std::string name = curToken().value();

    if (!isFreeName(name))
        throw SyntaxError{ curToken().startPos(), "This name is already taken" };

    matchToken(TokenType::ID);
    matchToken(TokenType::COLON);

    if (!isToken(TokenType::ID))
        throw SyntaxError{ curToken().startPos(), "Expected type" };

    std::string typeName = curToken().value();

    if (!isTypeName(typeName))
        throw SyntaxError{ curToken().startPos(), "Invalid type" };

    matchToken(TokenType::ID);
    matchToken(TokenType::EQUAL);

    if (isToken(TokenType::INT))
    {
        if (typeName != "integer")
            throw SyntaxError{ curToken().startPos(), "Expected integer literal" };

        try
        {
            long long val = std::stoll(curToken().value());

            DataTypeId type = mContext.typeStorage().getTypeId("integer").value();
            VariableInfo info(name, type, true);
            mContext.variableStorage().registerVariable(info, val);
            matchToken(TokenType::INT);
        }
        catch (std::out_of_range)
        {
            throw SyntaxError{ curToken().startPos(), "Too big integer" };
        }
    } else if (isToken(TokenType::FLOAT))
    {
        if (typeName != "double")
            throw SyntaxError{ curToken().startPos(), "Expected double literal" };

        try
        {
            double val = std::stod(curToken().value());
            DataTypeId type = mContext.typeStorage().getTypeId("double").value();
            VariableInfo info(name, type, true);
            mContext.variableStorage().registerVariable(info, val);
            matchToken(TokenType::FLOAT);
        }
        catch (std::out_of_range)
        {
            throw SyntaxError{ curToken().startPos(), "Too big double" };
        }
    } else
    {
        throw SyntaxError{ curToken().startPos(), "Expected integer or double literal" };
    }

    matchToken(TokenType::SEMICOLON);
}

Compiler::PostfixCompilationResult Compiler::ProgramCompiler::compileExpression()
{
    size_t closingPos = findClosingTokenPosition();
    std::vector<Token> expr(mTokens.begin() + mCurrentPosition, mTokens.begin() + closingPos);
    expr.push_back(Token(TokenType::ENDOFFILE, "", closingPos, closingPos));
    auto compRes = mPostfixCompiler.compileExpression(expr);
    if (std::holds_alternative<SyntaxError>(compRes))
        throw std::get<SyntaxError>(compRes);

    mCurrentPosition = closingPos;
    return std::get<PostfixCompilationResult>(compRes);
}

size_t Compiler::ProgramCompiler::findClosingTokenPosition() const
{
    size_t pos = mCurrentPosition;
    int parenCount = 0;
    int blockCount = 0;

    for (; pos < mTokens.size(); pos++)
    {
        TokenType curType = mTokens[pos].type();
        if (curType == TokenType::ENDOFFILE ||
            curType == TokenType::INVALID)
            return pos;

        else if (parenCount == 0 && blockCount == 0
            && (curType == TokenType::COMMA
                || curType == TokenType::SEMICOLON
                || curType == TokenType::END
                || curType == TokenType::RPAREN))
            return pos;
        else if (curType == TokenType::LPAREN)
            parenCount++;
        else if (curType == TokenType::BEGIN)
            blockCount++;
        else if (curType == TokenType::RPAREN)
            parenCount--;
        else if (curType == TokenType::END)
            blockCount--;

    }

    return pos;
}