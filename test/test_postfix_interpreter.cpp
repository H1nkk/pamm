#include <gtest/gtest.h>
#include "expression_interpreter/postfix_interpreter.h"
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"
#include "variant_index.h"

class PostfixInterpreterTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        execContext.typeStorage().registerType(DataType("integer", variant_index<DataValue, long long>()));
        execContext.typeStorage().registerType(DataType("double", variant_index<DataValue, double>()));
        execContext.typeStorage().registerType(DataType("boolean", variant_index<DataValue, bool>()));
        execContext.typeStorage().registerType(DataType("string", variant_index<DataValue, std::string>()));
    }

    void registerVariable(const std::string& name, const std::string& type, auto value)
    {
        DataTypeId typeId = execContext.typeStorage().getTypeId(type).value();
        VariableInfo info(name, typeId, false);
        execContext.variableStorage().registerVariable(info, value);
    }

    ExecutionContext execContext;
    Intr::PostfixInterpreter interpreter;
};

TEST_F(PostfixInterpreterTest, can_execute_single_integer)
{
    registerVariable("a", "integer", 10LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 10LL);
}

TEST_F(PostfixInterpreterTest, can_execute_single_float)
{
    registerVariable("a", "double", 10.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0);
}

TEST_F(PostfixInterpreterTest, can_execute_sum_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ADD_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 13LL);
}

TEST_F(PostfixInterpreterTest, can_execute_sum_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ADD_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0 + 3.0);
}

TEST_F(PostfixInterpreterTest, can_execute_sum_string)
{
    registerVariable("a", "string", std::string("Hello"));
    registerVariable("b", "string", std::string("world"));

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ADD_STRING_STRING}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<std::string>(val));
    EXPECT_EQ(std::get<std::string>(val), "Helloworld");
}

TEST_F(PostfixInterpreterTest, can_execute_sub_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::SUB_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 7LL);
}

TEST_F(PostfixInterpreterTest, can_execute_sub_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::SUB_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0 - 3.0);
}

TEST_F(PostfixInterpreterTest, can_execute_mult_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::MULT_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 30LL);
}

TEST_F(PostfixInterpreterTest, can_execute_mult_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::MULT_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0 * 3.0);
}

TEST_F(PostfixInterpreterTest, can_execute_div_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 3LL);
}

TEST_F(PostfixInterpreterTest, can_execute_div_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0 / 3.0);
}

TEST_F(PostfixInterpreterTest, can_execute_mod_int_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::MOD_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 1LL);
}

TEST_F(PostfixInterpreterTest, can_execute_negate_int)
{
    registerVariable("a", "integer", 10LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NEGATE_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), -10LL);
}

TEST_F(PostfixInterpreterTest, can_execute_negate_double)
{
    registerVariable("a", "double", 10.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NEGATE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), -10.0);
}

TEST_F(PostfixInterpreterTest, can_execute_less_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::LESS_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_less_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::LESS_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_greater_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::GREATER_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_greater_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::GREATER_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_lessequal_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::LESSEQUAL_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_lessequal_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::LESSEQUAL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_greaterequal_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::GREATEREQUAL_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_greaterequal_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::GREATEREQUAL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_equal_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::EQUAL_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_equal_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::EQUAL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_equal_bool)
{
    registerVariable("a", "boolean", true);
    registerVariable("b", "boolean", true);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::EQUAL_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_notequal_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NOTEQUAL_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_notequal_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 3.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NOTEQUAL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_notequal_bool)
{
    registerVariable("a", "boolean", true);
    registerVariable("b", "boolean", true);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NOTEQUAL_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_and_bool)
{
    registerVariable("a", "boolean", true);
    registerVariable("b", "boolean", false);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::AND_BOOL_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_execute_or_bool)
{
    registerVariable("a", "boolean", true);
    registerVariable("b", "boolean", false);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::OR_BOOL_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), true);
}

TEST_F(PostfixInterpreterTest, can_execute_not_bool)
{
    registerVariable("a", "boolean", true);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::NOT_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_EQ(std::get<bool>(val), false);
}

TEST_F(PostfixInterpreterTest, can_convert_int_to_double)
{
    registerVariable("a", "integer", 10LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::CAST_INT_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0);
}

TEST_F(PostfixInterpreterTest, can_execute_abs_int)
{
    registerVariable("a", "integer", -10LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ABS_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 10LL);
}

TEST_F(PostfixInterpreterTest, can_execute_abs_double)
{
    registerVariable("a", "double", -10.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ABS_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), 10.0);
}

TEST_F(PostfixInterpreterTest, can_execute_sin_double)
{
    registerVariable("a", "double", 123.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::SIN_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), sin(123.0));
}

TEST_F(PostfixInterpreterTest, can_execute_cos_double)
{
    registerVariable("a", "double", 123.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::COS_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), cos(123.0));
}

TEST_F(PostfixInterpreterTest, can_execute_exp_double)
{
    registerVariable("a", "double", 123.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::EXP_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_EQ(std::get<double>(val), exp(123.0));
}

TEST_F(PostfixInterpreterTest, can_execute_floor_double)
{
    registerVariable("a", "double", 123.5);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::FLOOR_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), static_cast<long long>(floor(123.5)));
}

TEST_F(PostfixInterpreterTest, can_execute_ceil_double)
{
    registerVariable("a", "double", 123.5);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::CEIL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), static_cast<long long>(ceil(123.5)));
}

TEST_F(PostfixInterpreterTest, can_execute_round_double)
{
    registerVariable("a", "double", 123.5);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ROUND_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), static_cast<long long>(round(123.5)));
}

TEST_F(PostfixInterpreterTest, error_on_zero_division_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 0LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<std::string>(result));
    EXPECT_EQ(std::get<std::string>(result), "Zero division occurred! Error.");
}

TEST_F(PostfixInterpreterTest, error_on_zero_division_mod_int)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 0LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::MOD_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<std::string>(result));
    EXPECT_EQ(std::get<std::string>(result), "Zero division occurred! Error.");
}

TEST_F(PostfixInterpreterTest, error_on_zero_division_double)
{
    registerVariable("a", "double", 10.0);
    registerVariable("b", "double", 0.0);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<std::string>(result));
    EXPECT_EQ(std::get<std::string>(result), "Zero division occurred! Error.");
}

TEST_F(PostfixInterpreterTest, error_on_type_mismatch)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "double", 3.14);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ADD_INT_INT}
    };

    EXPECT_THROW(interpreter.execute(program, execContext), std::bad_variant_access);
}

TEST_F(PostfixInterpreterTest, can_work_with_int_boundary_values)
{
    registerVariable("max", "integer", LLONG_MAX);
    registerVariable("min", "integer", LLONG_MIN);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::ADD_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), -1);
}

TEST_F(PostfixInterpreterTest, can_load_strings)
{
    std::string str = "test";
    registerVariable("s", "string", str);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<std::string>(val));
    EXPECT_EQ(std::get<std::string>(val), "test");
}

TEST_F(PostfixInterpreterTest, can_convert_int_to_string)
{
    long long v = 10ll;
    registerVariable("s", "integer", v);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::TO_STRING_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<std::string>(val));
    EXPECT_EQ(std::get<std::string>(val), std::to_string(v));
}

TEST_F(PostfixInterpreterTest, can_convert_double_to_string)
{
    double v = 10.0;
    registerVariable("s", "double", v);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::TO_STRING_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<std::string>(val));
    EXPECT_EQ(std::get<std::string>(val), std::to_string(v));
}

TEST_F(PostfixInterpreterTest, can_convert_bool_to_string)
{
    bool v = true;
    registerVariable("s", "boolean", v);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::TO_STRING_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<std::string>(val));
    EXPECT_EQ(std::get<std::string>(val), std::to_string(v));
}