#include <gtest/gtest.h>
#include "expression_compiler/postfix_compiler.h"
#include "lexer/lexer_token.h"
#include "expression_interpreter/execution_context.h"
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"

using namespace Lexer;
using namespace Compiler;
using namespace Intr;

class PostfixCompilerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        execContext.typeStorage().registerType(DataType("integer", sizeof(long long)));
        execContext.typeStorage().registerType(DataType("double", sizeof(double)));
        execContext.typeStorage().registerType(DataType("boolean", sizeof(bool)));

        registerOperator("$operator+", { "integer", "integer" }, "integer", 1);
        registerOperator("$operator-", { "integer", "integer" }, "integer", 2);
        registerOperator("$operator*", { "integer", "integer" }, "integer", 3);
        registerOperator("$operator/", { "integer", "integer" }, "integer", 4);
        registerOperator("$operatormod", { "integer", "integer" }, "integer", 26);
        registerOperator("$operatordiv", { "integer", "integer" }, "integer", 27);
        registerOperator("$operator-", { "integer" }, "integer", 5);

        registerOperator("$operatorand", { "boolean", "boolean" }, "boolean", 10);
        registerOperator("$operatoror", { "boolean", "boolean" }, "boolean", 11);
        registerOperator("$operatornot", { "boolean" }, "boolean", 12);

        registerOperator("$operator=", { "integer", "integer" }, "boolean", 20);
        registerOperator("$operator<>", { "integer", "integer" }, "boolean", 21);
        registerOperator("$operator<", { "integer", "integer" }, "boolean", 22);
        registerOperator("$operator<=", { "integer", "integer" }, "boolean", 23);
        registerOperator("$operator>", { "integer", "integer" }, "boolean", 24);
        registerOperator("$operator>=", { "integer", "integer" }, "boolean", 25);
    }

    void registerOperator(const std::string& name,
        const std::vector<std::string>& argTypes,
        const std::string& retType,
        FunctionId interpreterId)
    {
        std::vector<DataTypeId> argTypeIds;
        for (const auto& t : argTypes)
        {
            argTypeIds.push_back(execContext.typeStorage().getTypeId(t).value());
        }
        DataTypeId retTypeId = execContext.typeStorage().getTypeId(retType).value();

        FunctionInfo info(name, retTypeId, argTypeIds, interpreterId);
        execContext.functionStorage().registerFunction(info);
    }

    ExecutionContext execContext;
};

TEST_F(PostfixCompilerTest, can_compile_single_integer)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "42", 0, 2},
        {TokenType::ENDOFFILE, "", 2, 2}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 1);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
}

TEST_F(PostfixCompilerTest, can_compile_single_double)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "42.0", 0, 2},
        {TokenType::ENDOFFILE, "", 2, 2}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 1);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
}

TEST_F(PostfixCompilerTest, can_compile_addition)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_subtraction)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::MINUS, "-", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_multiplication)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::MULT, "*", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_division)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::DIV, "/", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_int_division)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::DIVINT, "div", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_modulo)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::MOD, "mod", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_unary_minus)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::MINUS, "-", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 2);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_not_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::NOTEQUAL, "<>", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_less)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::LESS, "<", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_greater)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::GREATER, ">", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_lessequal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::LESSEQUAL, "<=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_greater_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::GREATEREQUAL, ">=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_and)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "3", 4, 5},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::RPAREN, ")", 4, 5},
        {TokenType::AND, "and", 2, 3},
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "3", 4, 5},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::RPAREN, ")", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::LOAD);
    EXPECT_EQ(program[4].first, Opcode::LOAD);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_or)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "3", 4, 5},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::RPAREN, ")", 4, 5},
        {TokenType::OR, "or", 2, 3},
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "3", 4, 5},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::RPAREN, ")", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::LOAD);
    EXPECT_EQ(program[4].first, Opcode::LOAD);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_not)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::NOT, "not", 0, 1},
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "3", 4, 5},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::RPAREN, ")", 4, 5},
        {TokenType::ENDOFFILE, "", 6, 6}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 4);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_operator_precedence)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::INT, "3", 4, 5},
        {TokenType::MULT, "*", 6, 7},
        {TokenType::INT, "4", 8, 9},
        {TokenType::ENDOFFILE, "", 10, 10}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 5);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program[3].second, 3);
    EXPECT_EQ(program[4].first, Opcode::CALL);
    EXPECT_EQ(program[4].second, 1);
}

TEST_F(PostfixCompilerTest, can_handle_parentheses)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "2", 2, 3},
        {TokenType::PLUS, "+", 4, 5},
        {TokenType::INT, "3", 6, 7},
        {TokenType::RPAREN, ")", 8, 9},
        {TokenType::MULT, "*", 10, 11},
        {TokenType::INT, "4", 12, 13},
        {TokenType::ENDOFFILE, "", 14, 14}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 5);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[0].second, 0);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[1].second, 1);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[2].second, 1);
    EXPECT_EQ(program[3].first, Opcode::LOAD);
    EXPECT_EQ(program[3].second, 2);
    EXPECT_EQ(program[4].first, Opcode::CALL);
    EXPECT_EQ(program[4].second, 3);
}

TEST_F(PostfixCompilerTest, can_compile_single_variable)
{
    VariableInfo varInfo("x", execContext.typeStorage().getTypeId("integer").value(), false);
    execContext.variableStorage().registerVariable<long long>(varInfo, 10);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::ID, "x", 0, 1},
        {TokenType::ENDOFFILE, "", 2, 2}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 1);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[0].second, 0);
}

TEST_F(PostfixCompilerTest, can_handle_function_calls)
{
    std::vector<DataTypeId> argTypes = {
        execContext.typeStorage().getTypeId("integer").value(),
        execContext.typeStorage().getTypeId("integer").value()
    };
    DataTypeId retType = execContext.typeStorage().getTypeId("integer").value();
    FunctionInfo funcInfo("add", retType, argTypes, 10);
    execContext.functionStorage().registerFunction(funcInfo);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::ID, "add", 0, 3},
        {TokenType::LPAREN, "(", 4, 5},
        {TokenType::INT, "1", 6, 7},
        {TokenType::COMMA, ",", 8, 9},
        {TokenType::INT, "2", 10, 11},
        {TokenType::RPAREN, ")", 12, 13},
        {TokenType::ENDOFFILE, "", 14, 14}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[0].second, 0);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[1].second, 1);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[2].second, 10);
}

TEST_F(PostfixCompilerTest, can_handle_mixed_type_operators)
{
    registerOperator("$operator+", { "integer", "double" }, "double", 6);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::FLOAT, "3.5", 4, 7},
        {TokenType::ENDOFFILE, "", 8, 8}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program.back().first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_implicit_type_casting)
{
    registerOperator("$operator+", { "double", "double" }, "double", 100001);
    registerOperator("$cast", { "integer" }, "double", 100002);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::FLOAT, "3.5", 4, 7},
        {TokenType::ENDOFFILE, "", 8, 8}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 4);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[0].second, 0);
    EXPECT_EQ(program[1].first, Opcode::CALL);
    EXPECT_EQ(program[1].second, 100002);
    EXPECT_EQ(program[2].first, Opcode::LOAD);
    EXPECT_EQ(program[2].second, 1);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program[3].second, 100001);
}

TEST_F(PostfixCompilerTest, can_compile_complex_arithmetic_expressions)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::LPAREN, "(", 4, 5},
        {TokenType::INT, "3", 6, 7},
        {TokenType::MULT, "*", 8, 9},
        {TokenType::INT, "4", 10, 11},
        {TokenType::RPAREN, ")", 12, 13},
        {TokenType::MINUS, "-", 14, 15},
        {TokenType::INT, "5", 16, 17},
        {TokenType::ENDOFFILE, "", 18, 18}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program[4].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_logical_operators)
{
    VariableInfo var1("flag1", execContext.typeStorage().getTypeId("boolean").value(), false);
    VariableInfo var2("flag2", execContext.typeStorage().getTypeId("boolean").value(), false);
    execContext.variableStorage().registerVariable(var1, true);
    execContext.variableStorage().registerVariable(var2, false);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::ID, "flag1", 0, 5},
        {TokenType::AND, "and", 6, 9},
        {TokenType::LPAREN, "(", 10, 11},
        {TokenType::NOT, "not", 12, 15},
        {TokenType::ID, "flag2", 16, 21},
        {TokenType::RPAREN, ")", 22, 23},
        {TokenType::OR, "or", 24, 26},
        {TokenType::ID, "flag1", 27, 32},
        {TokenType::ENDOFFILE, "", 33, 33}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_GE(program.size(), 5);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program.back().first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_comparison_operators)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::INT, "5", 0, 1},
        {TokenType::LESS, "<", 2, 3},
        {TokenType::INT, "10", 4, 6},
        {TokenType::AND, "and", 7, 10},
        {TokenType::INT, "10", 11, 13},
        {TokenType::GREATEREQUAL, ">=", 14, 16},
        {TokenType::INT, "5", 17, 18},
        {TokenType::ENDOFFILE, "", 19, 19}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_min_max_int_values)
{
    PostfixCompiler compiler(execContext);

    std::string maxInt64 = "9223372036854775807";
    std::string minInt64 = "-9223372036854775808";

    std::vector<Token> tokens1 = {
        {TokenType::INT, maxInt64, 0, maxInt64.size()},
        {TokenType::ENDOFFILE, "", maxInt64.size() + 1, maxInt64.size() + 1}
    };

    auto result1 = compiler.compileExpression(tokens1);
    ASSERT_TRUE(std::holds_alternative<Program>(result1));

    std::vector<Token> tokens2 = {
        {TokenType::INT, minInt64, 0, minInt64.size()},
        {TokenType::ENDOFFILE, "", minInt64.size() + 1, minInt64.size() + 1}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<Program>(result2));

    std::string overflowInt = "9223372036854775808";
    std::vector<Token> tokens3 = {
        {TokenType::INT, overflowInt, 0, overflowInt.size()},
        {TokenType::ENDOFFILE, "", overflowInt.size() + 1, overflowInt.size() + 1}
    };

    auto result3 = compiler.compileExpression(tokens3);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result3));
}

TEST_F(PostfixCompilerTest, can_hanle_double_min_max_values)
{
    PostfixCompiler compiler(execContext);

    std::string maxDouble = "1.7976931348623157E+308";
    std::string minDouble = "2.2250738585072014E-308";
    std::string overflowDouble = "1.8E+308";

    std::vector<Token> tokens1 = {
        {TokenType::FLOAT, maxDouble, 0, maxDouble.size()},
        {TokenType::ENDOFFILE, "", maxDouble.size() + 1, maxDouble.size() + 1}
    };

    auto result1 = compiler.compileExpression(tokens1);
    ASSERT_TRUE(std::holds_alternative<Program>(result1));

    std::vector<Token> tokens2 = {
        {TokenType::FLOAT, minDouble, 0, minDouble.size()},
        {TokenType::ENDOFFILE, "", minDouble.size() + 1, minDouble.size() + 1}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<Program>(result2));

    std::vector<Token> tokens3 = {
        {TokenType::FLOAT, overflowDouble, 0, overflowDouble.size()},
        {TokenType::ENDOFFILE, "", overflowDouble.size() + 1, overflowDouble.size() + 1}
    };

    auto result3 = compiler.compileExpression(tokens3);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result3));
}

TEST_F(PostfixCompilerTest, can_compile_arithmetic_and_logic_in_one_expr)
{
    VariableInfo var("x", execContext.typeStorage().getTypeId("integer").value(), false);
    execContext.variableStorage().registerVariable<long long>(var, 10);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::ID, "x", 2, 3},
        {TokenType::GREATER, ">", 4, 5},
        {TokenType::INT, "5", 6, 7},
        {TokenType::RPAREN, ")", 8, 9},
        {TokenType::AND, "and", 10, 13},
        {TokenType::LPAREN, "(", 14, 15},
        {TokenType::ID, "x", 16, 17},
        {TokenType::LESSEQUAL, "<=", 18, 20},
        {TokenType::INT, "20", 21, 23},
        {TokenType::RPAREN, ")", 24, 25},
        {TokenType::ENDOFFILE, "", 26, 26}
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_long_expression_with_multiple_operators)
{
    PostfixCompiler compiler(execContext);

    // 1 + 5 * 2 / 4 - 3 mod 3 div 5 = 1 and 1 >= 0 or 0 <= 20 and (not (30 = 20)) and (4 < 5) or (5 > 10) or (4 <> 1)
    std::vector<Token> tokens = {
        {TokenType::INT, "1", 0, 2},
        {TokenType::PLUS, "+", 3, 4},
        {TokenType::INT, "5", 5, 6},
        {TokenType::MULT, "*", 7, 8},
        {TokenType::INT, "2", 9, 10},
        {TokenType::DIV, "/", 11, 12},
        {TokenType::INT, "4", 13, 14},
        {TokenType::MOD, "mod", 15, 18},
        {TokenType::INT, "3", 19, 20},
        {TokenType::DIVINT, "div", 19, 20},
        {TokenType::INT, "5", 19, 20},
        {TokenType::EQUAL, "=", 21, 22},
        {TokenType::INT, "1", 23, 24},
        {TokenType::AND, "and", 25, 28},
        {TokenType::INT, "1", 29, 30},
        {TokenType::GREATEREQUAL, ">=", 29, 30},
        {TokenType::INT, "0", 29, 30},
        {TokenType::OR, "or", 29, 30},
        {TokenType::INT, "0", 29, 30},
        {TokenType::LESSEQUAL, "<=", 29, 30},
        {TokenType::INT, "20", 29, 30},
        {TokenType::AND, "and", 29, 30},
        {TokenType::LPAREN, "(", 29, 30},
        {TokenType::NOT, "not", 29, 30},
        {TokenType::LPAREN, "(", 29, 30},
        {TokenType::INT, "30", 29, 30},
        {TokenType::EQUAL, "=", 29, 30},
        {TokenType::INT, "20", 29, 30},
        {TokenType::RPAREN, ")", 29, 30},
        {TokenType::RPAREN, ")", 29, 30},
        {TokenType::AND, "and", 29, 30},
        {TokenType::LPAREN, "(", 29, 30},
        {TokenType::INT, "4", 29, 30},
        {TokenType::LESS, "<", 29, 30},
        {TokenType::INT, "5", 29, 30},
        {TokenType::RPAREN, ")", 29, 30},
        {TokenType::OR, "or", 29, 30},
        {TokenType::LPAREN, "(", 29, 30},
        {TokenType::INT, "5", 29, 30},
        {TokenType::GREATER, ">", 29, 30},
        {TokenType::INT, "10", 29, 30},
        {TokenType::RPAREN, ")", 29, 30},
        {TokenType::OR, "or", 29, 30},
        {TokenType::LPAREN, "(", 29, 30},
        {TokenType::INT, "4", 29, 30},
        {TokenType::NOTEQUAL, "<>", 29, 30},
        {TokenType::INT, "1", 29, 30},
        {TokenType::RPAREN, ")", 29, 30},
        {TokenType::ENDOFFILE, "", 29, 30},
    };

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<Program>(result));

    auto program = std::get<Program>(result);
    EXPECT_EQ(program.size(), 38);
}

TEST_F(PostfixCompilerTest, error_on_binary_operator_without_left_argument)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::INT, "2", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_binary_operator_without_right_argument)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::MINUS, "-", 2, 3},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_two_unary_operators_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::MINUS, "-", 0, 1},
        {TokenType::MINUS, "-", 2, 3},
        {TokenType::INT, "2", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_two_binary_operators_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::PLUS, "+", 2, 3},
        {TokenType::INT, "2", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_invalid_types)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::AND, "and", 2, 3},
        {TokenType::INT, "2", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_undeclared_variable)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::ID, "y", 0, 1},
        {TokenType::EQUAL, "=", 2, 3},
        {TokenType::ID, "x", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}


TEST_F(PostfixCompilerTest, error_on_unclosed_parenthesis)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "2", 2, 3},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_too_many_parenthesis)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens2 = {
        {TokenType::LPAREN, "(", 0, 1},
        {TokenType::INT, "2", 2, 3},
        {TokenType::RPAREN, ")", 0, 1},
        {TokenType::RPAREN, ")", 0, 1},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
}

TEST_F(PostfixCompilerTest, error_on_two_values_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens1 = {
        {TokenType::INT, "2", 0, 1},
        {TokenType::INT, "3", 2, 3},
        {TokenType::ENDOFFILE, "", 4, 4}
    };

    auto result1 = compiler.compileExpression(tokens1);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result1));
}