#include <gtest/gtest.h>
#include "expression_compiler/postfix_compiler.h"
#include "lexer/lexer_token.h"
#include "lexer/lexer.h"
#include "expression_interpreter/execution_context.h"
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"

using Lexer::TokenType, Lexer::Token;
using namespace Compiler;
using namespace Intr;

class PostfixCompilerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        execContext.typeStorage().registerType(DataType("integer", variant_index<DataValue, long long>()));
        execContext.typeStorage().registerType(DataType("double", variant_index<DataValue, double>()));
        execContext.typeStorage().registerType(DataType("boolean", variant_index<DataValue, bool>()));

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
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 1);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
}

TEST_F(PostfixCompilerTest, can_compile_single_double)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("42.0").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 1);
    ASSERT_EQ(program.size(), 1);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
}

TEST_F(PostfixCompilerTest, can_compile_addition)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2+3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_subtraction)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2-3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_multiplication)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 * 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_division)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2/3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_int_division)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 div 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_modulo)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 mod 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_unary_minus)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("-3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 2);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 = 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_not_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 <> 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_less)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 < 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_greater)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 > 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_lessequal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 <= 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_greater_equal)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 >= 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_and)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("(3=3)and(3=3)").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
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

    std::vector<Token> tokens = Lexer::Lexer("(3=3)or(3=3)").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 2);
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

    std::vector<Token> tokens = Lexer::Lexer("not (3=3)").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 4);
    EXPECT_EQ(program[0].first, Opcode::LOAD);
    EXPECT_EQ(program[1].first, Opcode::LOAD);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_operator_precedence)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 + 3 * 4").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
    ASSERT_EQ(program.size(), 5);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program[3].second, 3);
    EXPECT_EQ(program[4].first, Opcode::CALL);
    EXPECT_EQ(program[4].second, 1);
}

TEST_F(PostfixCompilerTest, can_handle_parentheses)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("(2 + 3) * 4").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
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

    std::vector<Token> tokens = Lexer::Lexer("x").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
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

    std::vector<Token> tokens = Lexer::Lexer("add(1, 2)").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 0);
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

    std::vector<Token> tokens = Lexer::Lexer("2 + 3.5").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 1);
    ASSERT_EQ(program.size(), 3);
    EXPECT_EQ(program.back().first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_implicit_type_casting)
{
    registerOperator("$operator+", { "double", "double" }, "double", 100001);
    registerOperator("$cast", { "integer" }, "double", 100002);

    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 + 3.5").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
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

    std::vector<Token> tokens = Lexer::Lexer("2 + (3 * 4) - 5").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
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

    std::vector<Token> tokens = Lexer::Lexer("flag1 and (not flag2) or flag1").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_GE(program.size(), 5);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[3].first, Opcode::CALL);
    EXPECT_EQ(program.back().first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_compile_comparison_operators)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("5 < 10 and 10 >= 5").getAllTokens();
    
    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    ASSERT_EQ(program.size(), 7);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[6].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_int_overflow)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens1 = Lexer::Lexer("9223372036854775807").getAllTokens();

    auto result1 = compiler.compileExpression(tokens1);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result1));

    std::vector<Token> tokens2 = Lexer::Lexer("9223372036854775808").getAllTokens();

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result2));
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
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result1));

    std::vector<Token> tokens2 = {
        {TokenType::FLOAT, minDouble, 0, minDouble.size()},
        {TokenType::ENDOFFILE, "", minDouble.size() + 1, minDouble.size() + 1}
    };

    auto result2 = compiler.compileExpression(tokens2);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result2));

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

    std::vector<Token> tokens = Lexer::Lexer("(x > 5) and ((x - 5) <= 20)").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 2);
    ASSERT_EQ(program.size(), 9);
    EXPECT_EQ(program[2].first, Opcode::CALL);
    EXPECT_EQ(program[5].first, Opcode::CALL);
    EXPECT_EQ(program[7].first, Opcode::CALL);
    EXPECT_EQ(program[8].first, Opcode::CALL);
}

TEST_F(PostfixCompilerTest, can_handle_long_expression_with_multiple_operators)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("1 + 5 * 2 / 4 - 3 mod 3 div 5 = 1 and 1 >= 0 or 0 <= 20 and (not (30 = 20)) and (4 < 5) or (5 > 10) or (4 <> 1)").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<PostfixCompilationResult>(result));

    auto program = std::get<PostfixCompilationResult>(result).program;
    EXPECT_EQ(std::get<PostfixCompilationResult>(result).resultType, 2);
    EXPECT_EQ(program.size(), 40);
}

TEST_F(PostfixCompilerTest, error_on_binary_operator_without_left_argument)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("+2").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_binary_operator_without_right_argument)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2-").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_two_unary_operators_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("--2").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_two_binary_operators_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2++2").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_invalid_types)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 and 2").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_undeclared_variable)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("y = x").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}


TEST_F(PostfixCompilerTest, error_on_unclosed_parenthesis)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("(2").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_too_many_parenthesis)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("(2))").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST_F(PostfixCompilerTest, error_on_two_values_in_a_row)
{
    PostfixCompiler compiler(execContext);

    std::vector<Token> tokens = Lexer::Lexer("2 3").getAllTokens();

    auto result = compiler.compileExpression(tokens);
    ASSERT_TRUE(std::holds_alternative<SyntaxError>(result));
}