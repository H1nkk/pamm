#include <gtest/gtest.h>
#include "lexer/lexer.h"
#include "expression_compiler/program_compiler.h"

TEST(ProgramCompilerTest, cant_compile_without_program_name) {
    Lexer::Lexer lexer("begin end.");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, cant_compile_without_begin_end) {
    Lexer::Lexer lexer("program aaa;");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_without_variables) {
    Lexer::Lexer lexer("program aaa; begin end.");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_with_one_variable) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_with_multiple_variables) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, _bar, __megabar: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, cant_compile_if_variables_have_same_name) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, _bar, foo: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_without_consts) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "var\n"
        "a: integer;\n"
        "foo, _bar, ba: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_with_multiple_consts) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "sorokdva: integer = 42;\n"
        "var\n"
        "a: integer;\n"
        "foo, _bar, ba: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, cant_compile_if_const_have_same_name) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "pi: integer = 42;\n"
        "var\n"
        "a: integer;\n"
        "foo, _bar, foo: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, cant_compile_if_have_vars_and_consts_with_same_name) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, pi, foo: double;\n"
        "begin\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, cant_compile_then_using_unknown_var_or_const) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_compile_write) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, write_supports_many_parameters) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_write_with_no_parameters) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_compile_read) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_read_with_no_params) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_read_with_more_than_one_parameter) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_compile_assign) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_assign_without_right_part) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_compile_if) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_compile_if_else) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_if_without_condition) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, cant_compile_if_with_empty_block) {
    ADD_FAILURE();
}

TEST(ProgramCompilerTest, can_either_use_begin_end_blocks_and_one_line_operators_in_if) {
    ADD_FAILURE();
}