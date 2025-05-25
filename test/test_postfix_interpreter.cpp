//#include <gtest/gtest.h>
//#include "expression_interpreter/postfix_interpreter.h"
//
//TEST(PostfixInterpreterTest, can_execute_single_integer)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_single_float)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_sum)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_subtraction)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_multiplication)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterText, can_execute_remainder)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_float_division)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_integer_division)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_less)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_greater)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_equal)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_not_equal)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_lessequal)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_greaterequal)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_and)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_or)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, can_execute_not)
//{
//    ADD_FAILURE();
//}
//
//TEST(PostfixInterpreterTest, throw_on_zero_division)
//{
//    ADD_FAILURE();
//}

#include <gtest/gtest.h>
#include "expression_interpreter/postfix_interpreter.h"
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"

class PostfixInterpreterTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        execContext.typeStorage().registerType(DataType("integer", sizeof(long long)));
        execContext.typeStorage().registerType(DataType("double", sizeof(double)));
        execContext.typeStorage().registerType(DataType("boolean", sizeof(bool)));
        execContext.typeStorage().registerType(DataType("string", sizeof(char*)));
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

// Тесты для арифметических операций
TEST_F(PostfixInterpreterTest, IntegerArithmetic)
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

TEST_F(PostfixInterpreterTest, DoubleArithmetic)
{
    registerVariable("x", "double", 5.5);
    registerVariable("y", "double", 2.5);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}, // x
        {Intr::Opcode::LOAD, 1}, // y
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::MULT_DOUBLE_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<double>(val));
    EXPECT_DOUBLE_EQ(std::get<double>(val), 5.5 * 2.5);
}

TEST_F(PostfixInterpreterTest, IntegerDivision)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 3LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}, // a
        {Intr::Opcode::LOAD, 1}, // b
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<long long>(val));
    EXPECT_EQ(std::get<long long>(val), 3LL);
}

// Тесты для операций сравнения
TEST_F(PostfixInterpreterTest, IntegerComparison)
{
    registerVariable("a", "integer", 5LL);
    registerVariable("b", "integer", 10LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}, // a
        {Intr::Opcode::LOAD, 1}, // b
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::LESS_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_TRUE(std::get<bool>(val));
}

TEST_F(PostfixInterpreterTest, DoubleEquality)
{
    registerVariable("x", "double", 3.14);
    registerVariable("y", "double", 3.14);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}, // x
        {Intr::Opcode::LOAD, 1}, // y
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::EQUAL_DOUBLE}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_TRUE(std::get<bool>(val));
}

// Тесты для логических операций
TEST_F(PostfixInterpreterTest, LogicalOperations)
{
    registerVariable("t", "boolean", true);
    registerVariable("f", "boolean", false);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::LOAD, 1},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::AND_BOOL_BOOL},
        {Intr::Opcode::LOAD, 0},
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::OR_BOOL_BOOL}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<bool>(val));
    EXPECT_TRUE(std::get<bool>(val));
}

// Тесты для обработки ошибок
TEST_F(PostfixInterpreterTest, DivisionByZero)
{
    registerVariable("a", "integer", 10LL);
    registerVariable("b", "integer", 0LL);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}, // a
        {Intr::Opcode::LOAD, 1}, // b
        {Intr::Opcode::CALL, Intr::PostfixInterpreter::DIV_INT_INT}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<std::string>(result));
    EXPECT_EQ(std::get<std::string>(result), "Zero division occurred! Error.");
}

TEST_F(PostfixInterpreterTest, TypeMismatch)
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

// Тесты для граничных значений
TEST_F(PostfixInterpreterTest, IntegerBoundaryValues)
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

TEST_F(PostfixInterpreterTest, StringOperations)
{
    char str[] = "test";
    registerVariable("s", "string", str);

    Intr::Program program = {
        {Intr::Opcode::LOAD, 0}
    };

    auto result = interpreter.execute(program, execContext);
    ASSERT_TRUE(std::holds_alternative<DataValue>(result));
    DataValue val = std::get<DataValue>(result);
    ASSERT_TRUE(std::holds_alternative<char*>(val));
    EXPECT_STREQ(std::get<char*>(val), "test");
    delete[] std::get<char*>(val);
}