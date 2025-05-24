#include "googletest/include/gtest/gtest.h"
#include "lexer/lexer.h"
int main(int argc, char** argv)
{
    Lexer::Lexer l("+-* . , ; : / := ");
    ////////////////01234567890123456789
    //std::cout << l.getAllTokens()[8]->value;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}