#include <gtest/gtest.h>
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"

TEST(DataTypeTest, data_type_correct) {
    DataType type1("int", 4);
    DataType type2("float", 4);
    DataType type3("int", 8);
    DataType type4("int", 8);

    EXPECT_EQ(type1.name(), "int");
    EXPECT_EQ(type1.size(), 4);
    EXPECT_EQ(type2.name(), "float");
    EXPECT_EQ(type2.size(), 4);
    
    EXPECT_TRUE(type3 == type4);
    EXPECT_FALSE(type1 == type3);
    EXPECT_FALSE(type1 == type2);
}

// Тесты для TypeStorage
TEST(TypeStorageTest, can_perform_type_registration) {
    TypeStorage storage;
    
    DataType type1("int", 4);
    DataType type2("float", 4);
    
    auto id1 = storage.registerType(type1);
    auto id2 = storage.registerType(type2);
    
    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    
    EXPECT_TRUE(storage.isTypeName("int"));
    EXPECT_TRUE(storage.isTypeName("float"));
    EXPECT_FALSE(storage.isTypeName("double"));
    
    EXPECT_EQ(storage.getTypeId("int").value(), id1);
    EXPECT_EQ(storage.getTypeId("float").value(), id2);
    EXPECT_FALSE(storage.getTypeId("double").has_value());
    
    EXPECT_EQ(storage.getTypeInfo(id1).value().name(), "int");
    EXPECT_EQ(storage.getTypeInfo(id1).value().size(), 4);
    EXPECT_EQ(storage.getTypeInfo(id2).value().name(), "float");
    EXPECT_FALSE(storage.getTypeInfo(999).has_value());
    
    EXPECT_THROW(storage.registerType(type1), std::runtime_error);
}

TEST(VariableInfoTest, variable_info_correct) {
    DataTypeId typeId = 1;
    VariableInfo var1("x", typeId, false);
    VariableInfo var2("y", typeId, true);
    
    EXPECT_EQ(var1.name(), "x");
    EXPECT_EQ(var1.type(), typeId);
    EXPECT_FALSE(var1.isConstant());
    
    EXPECT_EQ(var2.name(), "y");
    EXPECT_EQ(var2.type(), typeId);
    EXPECT_TRUE(var2.isConstant());
}

class VariablesStorageTest : public ::testing::Test {
protected:
    void SetUp() override {
        DataType type1("int", 4);
        DataType type2("float", 4);
        typeStorage.registerType(type1);
        typeStorage.registerType(type2);
    }
    
    TypeStorage typeStorage;
};

TEST_F(VariablesStorageTest, can_register_variables) {
    VariablesStorage storage(typeStorage);
    
    VariableInfo var1("x", 0, false);
    VariableInfo var2("y", 1, true);
    
    auto id1 = storage.registerVariable(var1, 42);
    auto id2 = storage.registerVariable(var2, 3.14f);
    
    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 1);
    
    EXPECT_TRUE(storage.isVariableName("x"));
    EXPECT_TRUE(storage.isVariableName("y"));
    EXPECT_FALSE(storage.isVariableName("z"));
    
    EXPECT_EQ(storage.getId("x").value(), id1);
    EXPECT_EQ(storage.getId("y").value(), id2);
    EXPECT_FALSE(storage.getId("z").has_value());
    
    EXPECT_EQ(storage.getInfo(id1).name(), "x");
    EXPECT_EQ(storage.getInfo(id1).type(), 0);
    EXPECT_FALSE(storage.getInfo(id1).isConstant());
    
    EXPECT_EQ(storage.getInfo(id2).name(), "y");
    EXPECT_EQ(storage.getInfo(id2).type(), 1);
    EXPECT_TRUE(storage.getInfo(id2).isConstant());
    
    EXPECT_THROW(storage.registerVariable(var1, 10), std::runtime_error);
}

TEST_F(VariablesStorageTest, can_perform_value_operations) {
    VariablesStorage storage(typeStorage);
    
    VariableInfo var1("x", 0, false);
    VariableInfo var2("y", 1, true);
    
    auto id1 = storage.registerVariable(var1, 42);
    auto id2 = storage.registerVariable(var2, 3.14f);
    
    EXPECT_EQ(storage.getValue<int>(id1), 42);
    EXPECT_FLOAT_EQ(storage.getValue<float>(id2), 3.14f);
    
    storage.setValue(id1, 100);
    EXPECT_EQ(storage.getValue<int>(id1), 100);
    
    EXPECT_THROW(storage.setValue(id2, 2.71f), std::runtime_error);
    
    auto literalId = storage.getIdByLiteral<float>(1, 1.23f);
    EXPECT_TRUE(storage.getInfo(literalId).isConstant());
    EXPECT_FLOAT_EQ(storage.getValue<float>(literalId), 1.23f);
}

TEST(FunctionInfoTest, function_info_correct) {
    std::vector<DataTypeId> argTypes = {0, 1};
    FunctionInfo func("add", 0, argTypes, 1, 10);
    
    EXPECT_EQ(func.name(), "add");
    EXPECT_EQ(func.returnType(), 0);
    EXPECT_EQ(func.argumentsCount(), 2);
    EXPECT_EQ(func.getArgument(0), 0);
    EXPECT_EQ(func.getArgument(1), 1);
    EXPECT_EQ(func.cost(), 10);
    EXPECT_EQ(func.interpreterId(), 1);
}

class FunctionStorageTest : public ::testing::Test {
protected:
    void SetUp() override {
        DataType type1("int", 4);
        DataType type2("float", 4);
        typeStorage.registerType(type1);
        typeStorage.registerType(type2);
    }
    
    TypeStorage typeStorage;
};

TEST_F(FunctionStorageTest, can_register_functions) {
    FunctionStorage storage(typeStorage);
    
    std::vector<DataTypeId> argTypes1 = {0, 0}; // int, int
    std::vector<DataTypeId> argTypes2 = {0, 1}; // int, float
    
    FunctionInfo func1("add", 0, argTypes1, 1);
    FunctionInfo func2("add", 1, argTypes2, 2);
    FunctionInfo func3("sub", 0, argTypes1, 3);
    
    storage.registerFunction(func1);
    storage.registerFunction(func2);
    storage.registerFunction(func3);
    
    EXPECT_TRUE(storage.isFunctionName("add"));
    EXPECT_TRUE(storage.isFunctionName("sub"));
    EXPECT_FALSE(storage.isFunctionName("mul"));
    
    auto found1 = storage.findFunction("add", argTypes1);
    EXPECT_TRUE(found1.has_value());
    EXPECT_EQ(found1->interpreterId(), 1);
    
    auto found2 = storage.findFunction("add", argTypes2);
    EXPECT_TRUE(found2.has_value());
    EXPECT_EQ(found2->interpreterId(), 2);
    
    auto found3 = storage.findFunction("sub", argTypes1);
    EXPECT_TRUE(found3.has_value());
    EXPECT_EQ(found3->interpreterId(), 3);
    
    std::vector<DataTypeId> wrongArgs = {1, 1};
    auto notFound = storage.findFunction("add", wrongArgs);
    EXPECT_FALSE(notFound.has_value());
    
    auto notFound2 = storage.findFunction("mul", argTypes1);
    EXPECT_FALSE(notFound2.has_value());
}