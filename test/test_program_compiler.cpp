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
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, piu, baa: double;\n"
        "begin\n"
        "a := pop;"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_boolean_vars_consts)
{
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "trueConst: boolean = True;\n"
        "falseConst: boolean = False;\n"
        "var\n"
        "val: boolean;\n"
        "begin\n"
        "val := trueConst or falseConst or False and True;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_string_vars_consts)
{
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "hello: string = 'Hello';\n"
        "world: string = 'world!';\n"
        "var\n"
        "val: string;\n"
        "begin\n"
        "val := hello + ' ' + world;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_write) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, piu, baa: double;\n"
        "begin\n"
        "a := 10; foo := 1.0; piu := 2.0; baa := 3.0;\n"
        "Write(a);\n"
        "WriteLn(pi);\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, write_supports_many_parameters) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, piu, baa: double;\n"
        "begin\n"
        "a := 10; foo := 1.0; piu := 2.0; baa := 3.0;\n"
        "Write(a, foo, piu, pi);\n"
        "WriteLn(a, foo, piu, pi);\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, cant_compile_write_with_no_parameters) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "foo, piu, baa: double;\n"
        "begin\n"
        "a := 10; foo := 1.0; piu := 2.0; baa := 3.0;\n"
        "Write();\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_read) {
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

TEST(ProgramCompilerTest, cant_compile_read_with_no_params) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "Read();\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, cant_compile_read_with_more_than_one_parameter) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "Read(a, b);\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_assign) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10 + 15 * (a + a);\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, cant_compile_assign_without_right_part) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := ;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_compile_if) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10;\n"
        "if (a < 20) then a:=20;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, can_compile_if_else) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10;\n"
        "if (a < 20) then a:=20;\n"
        "else a:=10;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}

TEST(ProgramCompilerTest, cant_compile_if_without_condition) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10;\n"
        "if () then a:=20;\n"
        "else a:=10;\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, cant_compile_if_without_code) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10;\n"
        "if (1<>10) then\n"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
}

TEST(ProgramCompilerTest, can_either_use_begin_end_blocks_and_one_line_operators_in_if) {
    Lexer::Lexer lexer(
        "program aaa;\n"
        "const\n"
        "pi: double = 3.14;\n"
        "var\n"
        "a: integer;\n"
        "begin\n"
        "a := 10;\n"
        "if (1<>10) then a:=10;\n"
        "if (1<>10) then begin a:=10; end\n"
        "if (1<>10) then a:=10; else begin a := 20; end\n"
        "if (1<=10) then begin a:=round(20*pi); a:=a; end else a:=0;"
        "end.\n");
    Compiler::ProgramCompiler comp(lexer.getAllTokens());

    auto result = comp.compileProgram();
    EXPECT_TRUE(std::holds_alternative<ExecutionContext>(result));
}