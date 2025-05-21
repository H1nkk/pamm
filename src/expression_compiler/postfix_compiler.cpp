#include "expression_compiler/postfix_compiler.h"
#include "expression_compiler/postfix_member.h"
#include "lexer/lexer_token.h"
#include "stack.h"
#include "data_values.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>

namespace Compiler {

    using Lexer::Token;
    using Lexer::TokenType;

    struct TokenTypePairHasher
    {
        size_t operator()(std::pair<TokenType, TokenType> t) const
        {
            return static_cast<size_t>(t.first) * 1000 + static_cast<size_t>(t.second);
        }
    };

    const static std::unordered_set<std::pair<TokenType, TokenType>, TokenTypePairHasher> validTokenSequences{
        { Lexer::TokenType::NONE, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::NONE, Lexer::TokenType::INT },
    { Lexer::TokenType::NONE, Lexer::TokenType::ID },
    { Lexer::TokenType::NONE, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::NONE, Lexer::TokenType::MINUS },
    { Lexer::TokenType::NONE, Lexer::TokenType::NOT },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::COMMA },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::RPAREN },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::PLUS },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::MINUS },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::MULT },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::DIV },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::DIVINT },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::MOD },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::EQUAL },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::LESS },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::GREATER },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::LESSEQUAL },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::GREATEREQUAL },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::NOTEQUAL },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::AND },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::OR },
    { Lexer::TokenType::FLOAT, Lexer::TokenType::ENDOFFILE },
    { Lexer::TokenType::INT, Lexer::TokenType::COMMA },
    { Lexer::TokenType::INT, Lexer::TokenType::RPAREN },
    { Lexer::TokenType::INT, Lexer::TokenType::PLUS },
    { Lexer::TokenType::INT, Lexer::TokenType::MINUS },
    { Lexer::TokenType::INT, Lexer::TokenType::MULT },
    { Lexer::TokenType::INT, Lexer::TokenType::DIV },
    { Lexer::TokenType::INT, Lexer::TokenType::DIVINT },
    { Lexer::TokenType::INT, Lexer::TokenType::MOD },
    { Lexer::TokenType::INT, Lexer::TokenType::EQUAL },
    { Lexer::TokenType::INT, Lexer::TokenType::LESS },
    { Lexer::TokenType::INT, Lexer::TokenType::GREATER },
    { Lexer::TokenType::INT, Lexer::TokenType::LESSEQUAL },
    { Lexer::TokenType::INT, Lexer::TokenType::GREATEREQUAL },
    { Lexer::TokenType::INT, Lexer::TokenType::NOTEQUAL },
    { Lexer::TokenType::INT, Lexer::TokenType::AND },
    { Lexer::TokenType::INT, Lexer::TokenType::OR },
    { Lexer::TokenType::INT, Lexer::TokenType::ENDOFFILE },
    { Lexer::TokenType::ID, Lexer::TokenType::COMMA },
    { Lexer::TokenType::ID, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::ID, Lexer::TokenType::RPAREN },
    { Lexer::TokenType::ID, Lexer::TokenType::PLUS },
    { Lexer::TokenType::ID, Lexer::TokenType::MINUS },
    { Lexer::TokenType::ID, Lexer::TokenType::MULT },
    { Lexer::TokenType::ID, Lexer::TokenType::DIV },
    { Lexer::TokenType::ID, Lexer::TokenType::DIVINT },
    { Lexer::TokenType::ID, Lexer::TokenType::MOD },
    { Lexer::TokenType::ID, Lexer::TokenType::EQUAL },
    { Lexer::TokenType::ID, Lexer::TokenType::LESS },
    { Lexer::TokenType::ID, Lexer::TokenType::GREATER },
    { Lexer::TokenType::ID, Lexer::TokenType::LESSEQUAL },
    { Lexer::TokenType::ID, Lexer::TokenType::GREATEREQUAL },
    { Lexer::TokenType::ID, Lexer::TokenType::NOTEQUAL },
    { Lexer::TokenType::ID, Lexer::TokenType::AND },
    { Lexer::TokenType::ID, Lexer::TokenType::OR },
    { Lexer::TokenType::ID, Lexer::TokenType::ENDOFFILE },
    { Lexer::TokenType::COMMA, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::COMMA, Lexer::TokenType::INT },
    { Lexer::TokenType::COMMA, Lexer::TokenType::ID },
    { Lexer::TokenType::COMMA, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::COMMA, Lexer::TokenType::MINUS },
    { Lexer::TokenType::COMMA, Lexer::TokenType::NOT },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::INT },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::ID },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::MINUS },
    { Lexer::TokenType::LPAREN, Lexer::TokenType::NOT },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::COMMA },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::RPAREN },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::PLUS },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::MINUS },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::MULT },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::DIV },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::DIVINT },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::MOD },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::EQUAL },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::LESS },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::GREATER },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::LESSEQUAL },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::GREATEREQUAL },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::NOTEQUAL },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::AND },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::OR },
    { Lexer::TokenType::RPAREN, Lexer::TokenType::ENDOFFILE },
    { Lexer::TokenType::PLUS, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::PLUS, Lexer::TokenType::INT },
    { Lexer::TokenType::PLUS, Lexer::TokenType::ID },
    { Lexer::TokenType::PLUS, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::MINUS, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::MINUS, Lexer::TokenType::INT },
    { Lexer::TokenType::MINUS, Lexer::TokenType::ID },
    { Lexer::TokenType::MINUS, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::MULT, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::MULT, Lexer::TokenType::INT },
    { Lexer::TokenType::MULT, Lexer::TokenType::ID },
    { Lexer::TokenType::MULT, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::DIV, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::DIV, Lexer::TokenType::INT },
    { Lexer::TokenType::DIV, Lexer::TokenType::ID },
    { Lexer::TokenType::DIV, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::DIVINT, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::DIVINT, Lexer::TokenType::INT },
    { Lexer::TokenType::DIVINT, Lexer::TokenType::ID },
    { Lexer::TokenType::DIVINT, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::MOD, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::MOD, Lexer::TokenType::INT },
    { Lexer::TokenType::MOD, Lexer::TokenType::ID },
    { Lexer::TokenType::MOD, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::EQUAL, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::EQUAL, Lexer::TokenType::INT },
    { Lexer::TokenType::EQUAL, Lexer::TokenType::ID },
    { Lexer::TokenType::EQUAL, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::LESS, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::LESS, Lexer::TokenType::INT },
    { Lexer::TokenType::LESS, Lexer::TokenType::ID },
    { Lexer::TokenType::LESS, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::GREATER, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::GREATER, Lexer::TokenType::INT },
    { Lexer::TokenType::GREATER, Lexer::TokenType::ID },
    { Lexer::TokenType::GREATER, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::LESSEQUAL, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::LESSEQUAL, Lexer::TokenType::INT },
    { Lexer::TokenType::LESSEQUAL, Lexer::TokenType::ID },
    { Lexer::TokenType::LESSEQUAL, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::GREATEREQUAL, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::GREATEREQUAL, Lexer::TokenType::INT },
    { Lexer::TokenType::GREATEREQUAL, Lexer::TokenType::ID },
    { Lexer::TokenType::GREATEREQUAL, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::NOTEQUAL, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::NOTEQUAL, Lexer::TokenType::INT },
    { Lexer::TokenType::NOTEQUAL, Lexer::TokenType::ID },
    { Lexer::TokenType::NOTEQUAL, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::AND, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::AND, Lexer::TokenType::INT },
    { Lexer::TokenType::AND, Lexer::TokenType::ID },
    { Lexer::TokenType::AND, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::OR, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::OR, Lexer::TokenType::INT },
    { Lexer::TokenType::OR, Lexer::TokenType::ID },
    { Lexer::TokenType::OR, Lexer::TokenType::LPAREN },
    { Lexer::TokenType::NOT, Lexer::TokenType::FLOAT },
    { Lexer::TokenType::NOT, Lexer::TokenType::INT },
    { Lexer::TokenType::NOT, Lexer::TokenType::ID },
    { Lexer::TokenType::NOT, Lexer::TokenType::LPAREN },
    };

    const static std::vector<BinaryOperatorInfo> binaryOperatorInfos = {};
    const static std::vector<UnaryOperatorInfo> unaryOperatorInfos = {};
    const static std::vector<FunctionInfo> functionInfos = {};
    const static std::vector<TypeCastingInfo> typeCastInfos = {};

    bool validateToken(TokenType prev, TokenType cur)
    {
        return validTokenSequences.count({ prev, cur }) > 0;
    }

    bool isBinaryOperator(const Token& token)
    {
        for (const auto& row : binaryOperatorInfos)
        {
            if (row.opType() == token.type())
                return true;
        }

        return false;
    }

    bool isPrefixOperator(const Token& token)
    {
        if (token.type() == TokenType::ID)
        {
            for (const auto& row : functionInfos)
            {
                if (row.name() == token.value())
                    return true;
            }
        }
        else
        {
            for (const auto& row : unaryOperatorInfos)
            {
                if (row.opType() == token.type())
                    return true;
            }
        }

        return false;
    }

    bool isFunction(const Token& token)
    {
        if (token.type() != TokenType::ID)
            return false;

        for (const auto& row : functionInfos)
        {
            if (row.name() == token.value())
                return true;
        }

        return false;
    }

    PostfixMember toMember(Token token, bool unary = false)
    {
        if (token.type() == TokenType::LPAREN) return PostfixMember(token);
        else if (token.type() == TokenType::ID)
        {
            if (unary)
            {
                // function name
                return PostfixMember(token, 0, true);
            }
            else
            {
                // variable
                return PostfixMember(token);
            }
        }
        
        if (unary)
        {
            for (const auto& row : unaryOperatorInfos)
            {
                if (row.opType() == token.type()) return PostfixMember(token, 0, true);
            }
        }
        else
        {
            for (const auto& row : binaryOperatorInfos)
            {
                if (row.opType() == token.type()) return PostfixMember(token, row.precedence(), false, row.associativity());
            }
        }
        

        throw std::invalid_argument(__FUNCTION__ ": Unknown operation provided");
    }

    struct CompilationBlock
    {
        TokenType startTokenType;
        size_t commaCount;
    };


    class CompilationContext final
    {
    private:
        std::vector<Token> mTokens;
        Intr::Program mProg;
        Stack<PostfixMember> mStack;
        Stack<CompilationBlock> mBlocks;
        Stack<DataType> mTypes;

        size_t mTokIndex;
        Token mPrev;
        Token mTok;

        Intr::Op compileOperation(const PostfixMember& member)
        {
            if (member.type() == TokenType::ID)
            {
                if (member.isPrefixOp())
                {
                    // as function

                    for (const auto& row : functionInfos)
                    {
                        if (row.name() != member.token().value()) continue;
                        if (mTypes.size() < row.argumentsCount()) continue;

                        bool valid = true;

                        Stack<DataType> tmp;
                        for (int i = row.argumentsCount() - 1; i >= 0; --i)
                        {
                            DataType type = mTypes.top();
                            if (row.getArgument(i) != type)
                                valid = false;

                            tmp.push(type);
                            mTypes.pop();
                        }

                        if (!valid)
                        {
                            while (tmp.size())
                            {
                                mTypes.push(tmp.top());
                                tmp.pop();
                            }
                            continue;
                        }

                        mTypes.push(row.returnType());

                        return row.opCode;
                    }

                    throw SyntaxError{ member.token().startPos(), "No suitable function found for name " + member.token().value() + " and provided arguments."};
                }
                else
                {
                    // as variable name
                    mTypes.push() // хахах теперь надо уметь получать тип данных через таблицу
                    // вернуть айди для чтения
                }
            }
            
            if (member.isPrefixOp())
            {
                if (mTypes.size() < 1)
                {
                    throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
                }

                DataType opType = mTypes.top();
                mTypes.pop();

                for (const auto& row : unaryOperatorInfos)
                {
                    if (row.opType() != member.type()) continue;
                    if (row.inputType() != opType) continue;

                    mTypes.push(row.outputType());
                    return row.opCode;
                }

                mTypes.push(opType);

                throw SyntaxError{ member.token().startPos(), "No suitable operator found for type " + mTypes.top().name() };
            }
            else
            {
                if (mTypes.size() < 2)
                {
                    throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
                }

                DataType rightType = mTypes.top();
                mTypes.pop();
                DataType leftType = mTypes.top();
                mTypes.pop();

                for (const auto& row : binaryOperatorInfos)
                {
                    if (row.opType() != member.type()) continue;
                    if (leftType != row.leftType() || rightType != row.rightType()) continue;

                    mTypes.push(row.returnType());
                    return row.opCode;
                }

                mTypes.push(leftType);
                mTypes.push(rightType);

                throw SyntaxError{ member.token().startPos(), "No suitable operator found for types: " + leftType.name() + ", " + rightType.name() };
            }

            throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
        }

        void compileToTok(const std::vector<TokenType>& types, bool popFound)
        {
            while (mStack.size() && std::find(types.begin(), types.end(), mStack.top().type()) == types.end())
            {
                mProg.push_back(compileOperation(mStack.top()));
                mStack.pop();
            }

            if (mStack.size() > 0 && popFound) mStack.pop();
        }

        void compilePrefixOps()
        {
            while (mStack.size() && mStack.top().isPrefixOp())
            {
                mProg.push_back(compileOperation(mStack.top()));
                mStack.pop();
            }
        }

    public:
        CompilationContext(const std::vector<Token>& toks)
        {
            mTokens = toks;
            mTokIndex = 0;
            mPrev = mTok = Token(TokenType::NONE);
        }

        TokenType curType() const { return mTok.type(); }
        TokenType prevType() const { return mPrev.type(); }

        const Intr::Program& getProgram() const { return mProg; }

        void nextTok()
        {
            mPrev = mTok;
            mTok = mTokens[mTokIndex++];

            if (!validateToken(prevType(), curType()))
                throw SyntaxError{ mTok.startPos(), "Unexpected token" };
        }

        bool isTok(TokenType type)
        {
            return curType() == type;
        }

        bool isPrefixOp()
        {
            return isPrefixOperator(mTok) 
                && (prevType() == TokenType::NONE 
                    || prevType() == TokenType::LPAREN
                    || prevType() == TokenType::COMMA
                    || isBinaryOperator(mPrev));
        }

        bool isBinaryOp()
        {
            return isBinaryOperator(mTok);
        }

        void openNewBlock()
        {
            if (isTok(TokenType::LPAREN))
            {
                mBlocks.push({ TokenType::LPAREN, 0 });
                mStack.push(toMember(TokenType::LPAREN));
            }
            else if (isTok(TokenType::NONE))
            {
                mBlocks.push({ TokenType::NONE, 0 });
            }
        }

        void closeBlock()
        {
            // check argument counts

            if (isTok(TokenType::RPAREN))
            {
                if (mBlocks.top().startTokenType != TokenType::LPAREN)
                    throw SyntaxError{ mTok.startPos(), "Unexpected right parenthesis" };

                mBlocks.pop();

                compileToTok({ TokenType::LPAREN, }, true);
                compilePrefixOps();
            }
            else if (isTok(TokenType::ENDOFFILE))
            {
                if (mBlocks.size() != 1 || mBlocks.top().startTokenType != TokenType::NONE)
                    throw SyntaxError{ mTok.startPos(), "Unclosed parenthesis" };

                mBlocks.pop();

                compileToTok({}, false);
            }
        }

        void applyComma()
        {
            CompilationBlock val = mBlocks.top();

            compileToTok({ TokenType::LPAREN }, false);
            mBlocks.pop();
            val.commaCount++;
            mBlocks.push(val);
        }

        void compilePrefixOp()
        {
            mStack.push(toMember(mTok, true));
        }

        void compileBinaryOp()
        {
            PostfixMember member = toMember(mTok);

            if (member.associativity() == OpAssociativity::LEFTTORIGHT)
            {
                while (mStack.size() && member.precedence() <= mStack.top().precedence())
                {
                    mProg.push_back(compileOperation(mStack.top()));
                    mStack.pop();
                }
            }
            else
            {
                while (mStack.size() && member.precedence() < mStack.top().precedence())
                {
                    mProg.push_back(compileOperation(mStack.top()));
                    mStack.pop();
                }
            }

            mStack.push(member);
        }
    };


    std::variant<Intr::Program, SyntaxError> PostfixCompiler::compileExpression(const std::vector<Lexer::Token>& tokens)
    {
        if (tokens.size() == 0 || tokens.back().type() != TokenType::ENDOFFILE)
            throw std::invalid_argument(__FUNCTION__ ": invalid token sequence");

        CompilationContext ctx(tokens);

        try
        {
            ctx.openNewBlock();

            do
            {
                ctx.nextTok();

                if (ctx.isTok(TokenType::LPAREN))
                {
                    ctx.openNewBlock();
                }
                else if (ctx.isTok(TokenType::COMMA))
                {
                    ctx.applyComma();
                }
                else if (ctx.isTok(TokenType::RPAREN) || ctx.isTok(TokenType::ENDOFFILE))
                {
                    ctx.closeBlock();
                }
                else if (ctx.isPrefixOp())
                {
                    ctx.compilePrefixOp();
                }
                else if (ctx.isBinaryOp())
                {
                    ctx.compileBinaryOp();
                }
            } while (!ctx.isTok(TokenType::ENDOFFILE));
        }
        catch (SyntaxError e)
        {
            return e;
        }

        return ctx.getProgram();
    }

}