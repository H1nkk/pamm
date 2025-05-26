#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include "lexer/lexer.h"
#include "expression_compiler/program_compiler.h"
#include "expression_interpreter/program_interpreter.h"
#include "Windows.h"

int main()
{
    std::cin >> std::noskipws;

    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string code(it, end);

    Lexer::Lexer lexer(code);

    if (lexer.hasError())
    { 
        std::cerr << "SYNTAX_ERROR " << lexer.getAllTokens().back().startPos() << " " <<
            "Invalid token" << std::endl;
        return -1;
    }

    auto compRes = Compiler::ProgramCompiler(lexer.getAllTokens()).compileProgram();

    if (std::holds_alternative<SyntaxError>(compRes))
    {
        SyntaxError err = std::get<SyntaxError>(compRes);
        
        std::cerr << "SYNTAX_ERROR " << err.pos << " " <<
            err.message << std::endl;
        return -1;
    }

    ExecutionContext ctx = std::get<ExecutionContext>(compRes);

    FreeConsole();
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    std::cin >> std::skipws;
    std::cout.clear();
    std::cin.clear();
    std::ios_base::sync_with_stdio();

    try
    {
        Intr::ProgramInterpreter::execute(ctx);
    }
    catch (std::string e)
    {
        std::cerr << "RUNTIME_ERROR " << e << std::endl;
        return -1;
    }

    system("pause");
    return 0;
}