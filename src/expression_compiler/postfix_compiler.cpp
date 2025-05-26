#include "expression_compiler/postfix_compiler.h"
#include "expression_compiler/postfix_member.h"
#include "lexer/lexer_token.h"
#include "stack.h"
#include "data_values.h"
#include "type_storage.h"
#include "variables_storage.h"
#include "function_storage.h"
#include "expression_interpreter/execution_context.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <sstream>
#include <list>

namespace Compiler {

    using Lexer::Token;
    using Lexer::TokenType;

    struct MemberTypePairHasher
    {
        size_t operator()(std::pair<PostfixMember::Type, PostfixMember::Type> t) const
        {
            return static_cast<size_t>(t.first) * 1000 + static_cast<size_t>(t.second);
        }
    };

    enum class OpAssociativity
    {
        LEFTTORIGHT,
        RIGHTTOLEFT
    };

    struct OperatorMapping
    {
        std::string funcName;
    };

    struct BinaryOperatorMapping : public OperatorMapping
    {
        int precedence;
        OpAssociativity associativity;
    };

    const static std::unordered_map<TokenType, OperatorMapping> unaryOperatorMappings = {
        { TokenType::MINUS, { "$operator-" }},
        { TokenType::NOT, { "$operatornot"} },
    };

    const static std::unordered_map<TokenType, BinaryOperatorMapping> binaryOperatorMappings = {
        // or
        { TokenType::OR, { "$operatoror", 100, OpAssociativity::LEFTTORIGHT }},
        // and
        { TokenType::AND, { "$operatorand", 101, OpAssociativity::LEFTTORIGHT }},
        // equality
        { TokenType::EQUAL, { "$operator=", 102, OpAssociativity::LEFTTORIGHT }},
        { TokenType::NOTEQUAL, { "$operator<>", 102, OpAssociativity::LEFTTORIGHT }},
        // comparison
        { TokenType::LESS, { "$operator<", 103, OpAssociativity::LEFTTORIGHT }},
        { TokenType::LESSEQUAL, { "$operator<=", 103, OpAssociativity::LEFTTORIGHT }},
        { TokenType::GREATER, { "$operator>", 103, OpAssociativity::LEFTTORIGHT }},
        { TokenType::GREATEREQUAL, { "$operator>=", 103, OpAssociativity::LEFTTORIGHT }},
        // additive
        { TokenType::PLUS, { "$operator+", 104, OpAssociativity::LEFTTORIGHT }},
        { TokenType::MINUS, { "$operator-", 104, OpAssociativity::LEFTTORIGHT }},
        // multiplicative
        { TokenType::MULT, { "$operator*", 105, OpAssociativity::LEFTTORIGHT }},
        { TokenType::DIV, { "$operator/", 105, OpAssociativity::LEFTTORIGHT }},
        { TokenType::DIVINT, { "$operatordiv", 105, OpAssociativity::LEFTTORIGHT }},
        { TokenType::MOD, { "$operatormod", 105, OpAssociativity::LEFTTORIGHT }}
    };

    const static std::unordered_set<std::pair<PostfixMember::Type, PostfixMember::Type>, MemberTypePairHasher> validMemberSequences{
        { PostfixMember::NONE, PostfixMember::VALUE },
        { PostfixMember::NONE, PostfixMember::FUNCCALL },
        { PostfixMember::NONE, PostfixMember::LPAREN },
        { PostfixMember::NONE, PostfixMember::UNARYOP },
        { PostfixMember::VALUE, PostfixMember::END },
        { PostfixMember::VALUE, PostfixMember::RPAREN },
        { PostfixMember::VALUE, PostfixMember::COMMA },
        { PostfixMember::VALUE, PostfixMember::BINARYOP },
        { PostfixMember::FUNCCALL, PostfixMember::LPAREN },
        { PostfixMember::LPAREN, PostfixMember::VALUE },
        { PostfixMember::LPAREN, PostfixMember::FUNCCALL },
        { PostfixMember::LPAREN, PostfixMember::LPAREN },
        { PostfixMember::LPAREN, PostfixMember::UNARYOP },
        { PostfixMember::RPAREN, PostfixMember::END },
        { PostfixMember::RPAREN, PostfixMember::VALUE },
        { PostfixMember::RPAREN, PostfixMember::RPAREN },
        { PostfixMember::RPAREN, PostfixMember::COMMA },
        { PostfixMember::RPAREN, PostfixMember::BINARYOP },
        { PostfixMember::COMMA, PostfixMember::VALUE },
        { PostfixMember::COMMA, PostfixMember::FUNCCALL },
        { PostfixMember::COMMA, PostfixMember::LPAREN },
        { PostfixMember::COMMA, PostfixMember::UNARYOP },
        { PostfixMember::BINARYOP, PostfixMember::VALUE },
        { PostfixMember::BINARYOP, PostfixMember::FUNCCALL },
        { PostfixMember::BINARYOP, PostfixMember::LPAREN },
        { PostfixMember::BINARYOP, PostfixMember::UNARYOP },
        { PostfixMember::UNARYOP, PostfixMember::VALUE },
        { PostfixMember::UNARYOP, PostfixMember::FUNCCALL },
        { PostfixMember::UNARYOP, PostfixMember::LPAREN },
    };

    bool validateSeq(PostfixMember::Type prev, PostfixMember::Type cur)
    {
        return validMemberSequences.count({ prev, cur }) > 0;
    }

    bool isBinaryOperator(const Token& token)
    {
        return binaryOperatorMappings.contains(token.type());
    }

    OpAssociativity getBinaryOperatorAssociativity(const Token& token)
    {
        return binaryOperatorMappings.at(token.type()).associativity;
    }

    int getBinaryOperatorPrecedence(const Token& token)
    {
        return binaryOperatorMappings.at(token.type()).precedence;
    }

    bool isUnaryOperator(const Token& token)
    {
        return unaryOperatorMappings.contains(token.type());
    }

    bool isFunction(const Token& token, const FunctionStorage& funcStorage)
    {
        if (token.type() != TokenType::ID)
            return false;

        return funcStorage.isFunctionName(token.value());
    }

    bool isValue(const Token& token, const TypeStorage& typeStorage)
    {
        TokenType type = token.type();
        if (type != TokenType::ID
            && type != TokenType::INT
            && type != TokenType::FLOAT
            && type != TokenType::STRING)
            return false;

        if (type == TokenType::ID
            && typeStorage.isTypeName(token.value()))
            return false;

        return true;
    }

    struct CompilationBlock
    {
        PostfixMember::Type startMemberType;
        size_t commaCount;
    };

    class CompilationContext final
    {
    private:
        ExecutionContext& mExecContext;

        std::vector<Token> mTokens;
        std::vector<PostfixMember> mMembers;

        std::list<Intr::Op> mOpList;

        Stack<PostfixMember> mStack;
        Stack<CompilationBlock> mBlocks;
        Stack<std::pair<DataTypeId, std::list<Intr::Op>::iterator>> mTypes;

        size_t mMemberIndex;
        PostfixMember mPrev;
        PostfixMember mCur;

        std::vector<PostfixMember> preprocess(const std::vector<Token>& tokens) const
        {
            std::vector<PostfixMember> res;
            res.reserve(tokens.size() + 2);

            PostfixMember::Type prev = PostfixMember::NONE;
            for (size_t i = 0; i < tokens.size(); i++)
            {
                const auto& tok = tokens[i];

                if (tok.type() == TokenType::ENDOFFILE)
                    res.push_back({ PostfixMember::END, i });
                else if (tok.type() == TokenType::LPAREN)
                    res.push_back({ PostfixMember::LPAREN, i });
                else if (tok.type() == TokenType::RPAREN)
                    res.push_back({ PostfixMember::RPAREN, i });
                else if (tok.type() == TokenType::COMMA)
                    res.push_back({ PostfixMember::COMMA, i });
                else if (isFunction(tok, mExecContext.functionStorage()))
                    res.push_back({ PostfixMember::FUNCCALL, i });
                else if (isUnaryOperator(tok) && validateSeq(prev, PostfixMember::UNARYOP))
                    res.push_back({ PostfixMember::UNARYOP, i });
                else if (isBinaryOperator(tok))
                    res.push_back({ PostfixMember::BINARYOP, i });
                else if (isValue(tok, mExecContext.typeStorage()))
                    res.push_back({ PostfixMember::VALUE, i });
                else
                    throw SyntaxError{ tok.startPos(), "Unexpected token in expression" };

                prev = res.back().type();
            }

            return res;
        }

        bool compileFuncCallWithCasts(const std::string& name,
            const std::vector<typename decltype(mTypes)::value_type>& arguments)
        {
            std::vector<DataTypeId> argTypes(arguments.size());
            for (size_t i = 0; i < arguments.size(); i++)
                argTypes[i] = arguments[i].first;

            auto suitableFunc = mExecContext.functionStorage().findFunction(
                name, argTypes
            );

            if (!suitableFunc.has_value())
                return false;

            const auto& func = suitableFunc.value().info;
            const auto& typeCasts = suitableFunc.value().casts;

            for (size_t i = 0; i < arguments.size(); i++)
            {
                auto insertIter = arguments[i].second;
                ++insertIter;

                for (size_t j = 0; j < typeCasts[i].size(); j++)
                {
                    mOpList.insert(insertIter, { Intr::Opcode::CALL, typeCasts[i][j] });
                    ++insertIter;
                }
            }

            mOpList.push_back({ Intr::Opcode::CALL, func.interpreterId() });
            mTypes.push({ func.returnType(), --mOpList.end() });
            return true;
        }

        void compileValue(const PostfixMember& member)
        {
            Token token = mTokens[member.tokenIndex()];

            if (token.type() == TokenType::ID)
            {
                if (!mExecContext.variableStorage().isVariableName(token.value()))
                {
                    throw SyntaxError{ token.startPos(), "Unknown variable '" + token.value() + "' used" };
                }

                VariableId id = mExecContext.variableStorage().getId(token.value()).value();
                DataTypeId type = mExecContext.variableStorage().getInfo(id).type();

                mOpList.push_back({ Intr::Opcode::LOAD, id });
                mTypes.push({ type, --mOpList.end() });
            } else if (token.type() == TokenType::INT)
            {
                try
                {
                    long long val = std::stoll(token.value());

                    DataTypeId type = mExecContext.typeStorage().getTypeId("integer").value();
                    VariableId id = mExecContext.variableStorage().getIdByLiteral(type, val);

                    mOpList.push_back({ Intr::Opcode::LOAD, id });
                    mTypes.push({ type, --mOpList.end() });
                }
                catch (std::out_of_range)
                {
                    throw SyntaxError{ token.startPos(), "Too big integer" };
                }
            } else if (token.type() == TokenType::FLOAT)
            {
                try
                {
                    double val = std::stod(token.value());

                    DataTypeId type = mExecContext.typeStorage().getTypeId("double").value();
                    VariableId id = mExecContext.variableStorage().getIdByLiteral(type, val);

                    mOpList.push_back({ Intr::Opcode::LOAD, id });
                    mTypes.push({ type, --mOpList.end() });
                }
                catch (std::out_of_range)
                {
                    throw SyntaxError{ token.startPos(), "Too big double" };
                }
            } else if (token.type() == TokenType::STRING)
            {
                std::string val = token.value();
                val = val.substr(1, val.size() - 2);

                char* cstr = new char[val.size() + 1];
                strcpy(cstr, val.c_str());

                DataTypeId type = mExecContext.typeStorage().getTypeId("string").value();
                VariableId id = mExecContext.variableStorage().getIdByLiteral<char*>(type, cstr);

                mOpList.push_back({ Intr::Opcode::LOAD, id });
                mTypes.push({ type, --mOpList.end() });
            }
            else 
            {
                throw std::runtime_error(__FUNCTION__ ": invalid value type.");
            }

            compilePrefixOps(1);
        }

        void compileUnaryOp(const PostfixMember& member)
        {
            if (mTypes.size() < 1)
            {
                throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
            }

            auto argVal = mTypes.top();
            mTypes.pop();

            TokenType opType = mTokens[member.tokenIndex()].type();

            if (compileFuncCallWithCasts(
                unaryOperatorMappings.at(opType).funcName, { argVal }
            )) return;

            mTypes.push(argVal);

            std::string typeName = mExecContext.typeStorage().getTypeInfo(argVal.first).value().name();
            throw SyntaxError{ mTokens[member.tokenIndex()].startPos(),
                "No suitable operator '" + mTokens[member.tokenIndex()].value() + "' found for type " + typeName };
        }

        void compileBinaryOp(const PostfixMember& member)
        {
            if (mTypes.size() < 2)
            {
                throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
            }

            auto argRight = mTypes.top();
            mTypes.pop();
            auto argLeft = mTypes.top();
            mTypes.pop();

            TokenType opType = mTokens[member.tokenIndex()].type();

            if (compileFuncCallWithCasts(
                binaryOperatorMappings.at(opType).funcName, { argLeft, argRight }
            )) return;

            mTypes.push(argLeft);
            mTypes.push(argRight);

            std::string typeNameL = mExecContext.typeStorage().getTypeInfo(argLeft.first).value().name();
            std::string typeNameR = mExecContext.typeStorage().getTypeInfo(argRight.first).value().name();
            throw SyntaxError{ mTokens[member.tokenIndex()].startPos(),
                "No suitable operator '" + mTokens[member.tokenIndex()].value() + "' found for types: " + typeNameL + ", " + typeNameR };
        }

        void compileFunction(const PostfixMember& member, size_t argCount)
        {
            if (mTypes.size() < argCount)
                throw std::runtime_error(__FUNCTION__ ": type checking stack error.");

            Token token = mTokens[member.tokenIndex()];
            std::string funcName = token.value();

            std::vector<typename decltype(mTypes)::value_type> args;

            args.reserve(argCount);

            for (size_t i = 0; i < argCount; i++)
            {
                args.push_back(mTypes.top());
                mTypes.pop();
            }

            std::reverse(args.begin(), args.end());

            if (compileFuncCallWithCasts(
                funcName, args
            )) return;

            for (size_t i = 0; i < args.size(); i++)
            {
                mTypes.push(args[i]);
            }

            std::stringstream error;
            error << "No suitable function found for ";
            error << funcName;
            error << '(';
            for (size_t i = 0; i < args.size(); i++)
            {
                error << mExecContext.typeStorage().getTypeInfo(args[i].first).value().name();
                error << (i + 1 == args.size() ? ')' : ',');
            }

            throw SyntaxError{ token.startPos(), error.str() };
        }

        void compileToTok(const std::vector<PostfixMember::Type>& types, bool popFound)
        {
            while (mStack.size() && std::find(types.begin(), types.end(), mStack.top().type()) == types.end())
            {
                if (mStack.top().type() != PostfixMember::BINARYOP)
                    throw std::runtime_error(__FUNCTION__ ": expected only binary operators.");

                compileBinaryOp(mStack.top());
                mStack.pop();
            }

            if (mStack.size() > 0 && popFound) mStack.pop();
        }

        void compilePrefixOps(size_t argCount)
        {
            while (mStack.size() && (mStack.top().type() == PostfixMember::UNARYOP || mStack.top().type() == PostfixMember::FUNCCALL))
            {
                PostfixMember::Type type = mStack.top().type();

                if (type == PostfixMember::FUNCCALL)
                {
                    // function
                    compileFunction(mStack.top(), argCount);
                    argCount = 1;
                } else
                {
                    // unary
                    if (argCount != 1)
                        throw SyntaxError{ mTokens[mCur.tokenIndex()].startPos(), "Too many arguments" };

                    compileUnaryOp(mStack.top());
                }

                mStack.pop();
            }

            if (argCount != 1)
                throw SyntaxError{ mTokens[mCur.tokenIndex()].startPos(), "Too many arguments" };
        }

    public:
        CompilationContext(const std::vector<Token>& toks, ExecutionContext& execCtx) :
            mTokens(toks), mMemberIndex(0), mPrev({ PostfixMember::NONE, 0 }),
            mCur({ PostfixMember::NONE, 0 }), mMembers(preprocess(toks)),
            mExecContext(execCtx)
        {}

        PostfixMember::Type curType() const { return mCur.type(); }
        PostfixMember::Type prevType() const { return mPrev.type(); }

        DataTypeId resultType() const { return mTypes.top().first; }
        Intr::Program program() const { return Intr::Program(mOpList.begin(), mOpList.end()); }

        void nextTok()
        {
            mPrev = mCur;
            mCur = mMembers[mMemberIndex++];

            if (!validateSeq(prevType(), curType()))
                throw SyntaxError{ mTokens[mCur.tokenIndex()].startPos(),
                    "Unexpected token '" + mTokens[mCur.tokenIndex()].value() + "'" };
        }

        bool isMember(PostfixMember::Type type)
        {
            return curType() == type;
        }

        void openNewBlock()
        {
            if (isMember(PostfixMember::LPAREN))
            {
                mBlocks.push({ PostfixMember::LPAREN, 0 });
                mStack.push(mCur);
            } else if (isMember(PostfixMember::NONE))
            {
                mBlocks.push({ PostfixMember::NONE, 0 });
            }
        }

        void closeBlock()
        {
            if (isMember(PostfixMember::RPAREN))
            {
                if (mBlocks.top().startMemberType != PostfixMember::LPAREN)
                    throw SyntaxError{ mTokens[mCur.tokenIndex()].startPos(), "Unexpected right parenthesis" };

                size_t argCount = mBlocks.top().commaCount + 1;

                mBlocks.pop();

                compileToTok({ PostfixMember::LPAREN, }, true);
                compilePrefixOps(argCount);
            } else if (isMember(PostfixMember::END))
            {
                if (mBlocks.size() != 1 || mBlocks.top().startMemberType != PostfixMember::NONE)
                    throw SyntaxError{ mTokens[mCur.tokenIndex()].startPos(), "Unclosed parenthesis" };

                mBlocks.pop();

                compileToTok({}, false);
            }
        }

        void applyComma()
        {
            CompilationBlock val = mBlocks.top();

            compileToTok({ PostfixMember::LPAREN }, false);
            mBlocks.pop();
            val.commaCount++;
            mBlocks.push(val);
        }

        void addValue()
        {
            compileValue(mCur);
        }

        void addUnaryOp()
        {
            mStack.push(mCur);
        }

        void addBinaryOp()
        {
            int precedence = getBinaryOperatorPrecedence(mTokens[mCur.tokenIndex()]);
            OpAssociativity associativity = getBinaryOperatorAssociativity(mTokens[mCur.tokenIndex()]);

            if (associativity == OpAssociativity::LEFTTORIGHT)
            {
                while (mStack.size()
                    && mStack.top().type() == PostfixMember::BINARYOP
                    && precedence <= getBinaryOperatorPrecedence(mTokens[mStack.top().tokenIndex()]))
                {

                    compileBinaryOp(mStack.top());
                    mStack.pop();
                }
            } else
            {
                while (mStack.size()
                    && mStack.top().type() == PostfixMember::BINARYOP
                    && precedence < getBinaryOperatorPrecedence(mTokens[mStack.top().tokenIndex()]))
                {
                    compileBinaryOp(mStack.top());
                    mStack.pop();
                }
            }

            mStack.push(mCur);
        }
    };


    std::variant<PostfixCompilationResult, SyntaxError> PostfixCompiler::compileExpression(const std::vector<Lexer::Token>& tokens)
    {
        if (tokens.size() == 0 || tokens.back().type() != TokenType::ENDOFFILE)
            throw std::invalid_argument(__FUNCTION__ ": invalid token sequence");

        try
        {
            CompilationContext ctx(tokens, mExecContext);

            ctx.openNewBlock();

            do
            {
                ctx.nextTok();

                if (ctx.isMember(PostfixMember::LPAREN))
                {
                    ctx.openNewBlock();
                } else if (ctx.isMember(PostfixMember::COMMA))
                {
                    ctx.applyComma();
                } else if (ctx.isMember(PostfixMember::RPAREN) || ctx.isMember(PostfixMember::END))
                {
                    ctx.closeBlock();
                } else if (ctx.isMember(PostfixMember::UNARYOP) || ctx.isMember(PostfixMember::FUNCCALL))
                {
                    ctx.addUnaryOp();
                } else if (ctx.isMember(PostfixMember::BINARYOP))
                {
                    ctx.addBinaryOp();
                } else if (ctx.isMember(PostfixMember::VALUE))
                {
                    ctx.addValue();
                } else
                {
                    throw std::runtime_error(__FUNCTION__ ": unexpected postfix member.");
                }
            } while (!ctx.isMember(PostfixMember::END));

            return PostfixCompilationResult{
                ctx.resultType(),
                ctx.program()
            };
        }
        catch (SyntaxError e)
        {
            return e;
        }
    }

}