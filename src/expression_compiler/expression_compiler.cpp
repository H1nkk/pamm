#include "expression_compiler/expression_compiler.h"
#include "expression_compiler/postfix_member.h"
#include "lexer/lexer_token.h"
#include "stack.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>

namespace Compiler {

    using Lexer::TokenType;
    using Lexer::Token;

    enum class PostfixType : int
    {
        INT = 1,
        FLOAT = 2,
        ID = 4,
        POLYNOMIAL = 8,
        ANY = 15
    };

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
        { Lexer::TokenType::NONE, Lexer::TokenType::X },
        { Lexer::TokenType::NONE, Lexer::TokenType::Y },
        { Lexer::TokenType::NONE, Lexer::TokenType::Z },
        { Lexer::TokenType::NONE, Lexer::TokenType::W },
        { Lexer::TokenType::NONE, Lexer::TokenType::LPAR },
        { Lexer::TokenType::NONE, Lexer::TokenType::MINUS },
        { Lexer::TokenType::NONE, Lexer::TokenType::CALC },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERX },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERY },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERZ },
        { Lexer::TokenType::NONE, Lexer::TokenType::DERW },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTX },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTY },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTZ },
        { Lexer::TokenType::NONE, Lexer::TokenType::INTW },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::X },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::Y },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::Z },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::W },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::RPAR },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::PLUS },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::MINUS },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::MULT },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::COMMA },
        { Lexer::TokenType::FLOAT, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::INT, Lexer::TokenType::X },
        { Lexer::TokenType::INT, Lexer::TokenType::Y },
        { Lexer::TokenType::INT, Lexer::TokenType::Z },
        { Lexer::TokenType::INT, Lexer::TokenType::W },
        { Lexer::TokenType::INT, Lexer::TokenType::RPAR },
        { Lexer::TokenType::INT, Lexer::TokenType::PLUS },
        { Lexer::TokenType::INT, Lexer::TokenType::MINUS },
        { Lexer::TokenType::INT, Lexer::TokenType::MULT },
        { Lexer::TokenType::INT, Lexer::TokenType::COMMA },
        { Lexer::TokenType::INT, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::ID, Lexer::TokenType::RPAR },
        { Lexer::TokenType::ID, Lexer::TokenType::PLUS },
        { Lexer::TokenType::ID, Lexer::TokenType::MINUS },
        { Lexer::TokenType::ID, Lexer::TokenType::MULT },
        { Lexer::TokenType::ID, Lexer::TokenType::ASSIGN },
        { Lexer::TokenType::ID, Lexer::TokenType::COMMA },
        { Lexer::TokenType::ID, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::X, Lexer::TokenType::INT },
        { Lexer::TokenType::X, Lexer::TokenType::X },
        { Lexer::TokenType::X, Lexer::TokenType::Y },
        { Lexer::TokenType::X, Lexer::TokenType::Z },
        { Lexer::TokenType::X, Lexer::TokenType::W },
        { Lexer::TokenType::X, Lexer::TokenType::RPAR },
        { Lexer::TokenType::X, Lexer::TokenType::PLUS },
        { Lexer::TokenType::X, Lexer::TokenType::MINUS },
        { Lexer::TokenType::X, Lexer::TokenType::MULT },
        { Lexer::TokenType::X, Lexer::TokenType::CARET },
        { Lexer::TokenType::X, Lexer::TokenType::COMMA },
        { Lexer::TokenType::X, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::Y, Lexer::TokenType::INT },
        { Lexer::TokenType::Y, Lexer::TokenType::X },
        { Lexer::TokenType::Y, Lexer::TokenType::Y },
        { Lexer::TokenType::Y, Lexer::TokenType::Z },
        { Lexer::TokenType::Y, Lexer::TokenType::W },
        { Lexer::TokenType::Y, Lexer::TokenType::RPAR },
        { Lexer::TokenType::Y, Lexer::TokenType::PLUS },
        { Lexer::TokenType::Y, Lexer::TokenType::MINUS },
        { Lexer::TokenType::Y, Lexer::TokenType::MULT },
        { Lexer::TokenType::Y, Lexer::TokenType::CARET },
        { Lexer::TokenType::Y, Lexer::TokenType::COMMA },
        { Lexer::TokenType::Y, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::Z, Lexer::TokenType::INT },
        { Lexer::TokenType::Z, Lexer::TokenType::X },
        { Lexer::TokenType::Z, Lexer::TokenType::Y },
        { Lexer::TokenType::Z, Lexer::TokenType::Z },
        { Lexer::TokenType::Z, Lexer::TokenType::W },
        { Lexer::TokenType::Z, Lexer::TokenType::RPAR },
        { Lexer::TokenType::Z, Lexer::TokenType::PLUS },
        { Lexer::TokenType::Z, Lexer::TokenType::MINUS },
        { Lexer::TokenType::Z, Lexer::TokenType::MULT },
        { Lexer::TokenType::Z, Lexer::TokenType::CARET },
        { Lexer::TokenType::Z, Lexer::TokenType::COMMA },
        { Lexer::TokenType::Z, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::W, Lexer::TokenType::INT },
        { Lexer::TokenType::W, Lexer::TokenType::X },
        { Lexer::TokenType::W, Lexer::TokenType::Y },
        { Lexer::TokenType::W, Lexer::TokenType::Z },
        { Lexer::TokenType::W, Lexer::TokenType::W },
        { Lexer::TokenType::W, Lexer::TokenType::RPAR },
        { Lexer::TokenType::W, Lexer::TokenType::PLUS },
        { Lexer::TokenType::W, Lexer::TokenType::MINUS },
        { Lexer::TokenType::W, Lexer::TokenType::MULT },
        { Lexer::TokenType::W, Lexer::TokenType::CARET },
        { Lexer::TokenType::W, Lexer::TokenType::COMMA },
        { Lexer::TokenType::W, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::LPAR, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INT },
        { Lexer::TokenType::LPAR, Lexer::TokenType::ID },
        { Lexer::TokenType::LPAR, Lexer::TokenType::X },
        { Lexer::TokenType::LPAR, Lexer::TokenType::Y },
        { Lexer::TokenType::LPAR, Lexer::TokenType::Z },
        { Lexer::TokenType::LPAR, Lexer::TokenType::W },
        { Lexer::TokenType::LPAR, Lexer::TokenType::LPAR },
        { Lexer::TokenType::LPAR, Lexer::TokenType::MINUS },
        { Lexer::TokenType::LPAR, Lexer::TokenType::CALC },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERX },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERY },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERZ },
        { Lexer::TokenType::LPAR, Lexer::TokenType::DERW },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTX },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTY },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTZ },
        { Lexer::TokenType::LPAR, Lexer::TokenType::INTW },
        { Lexer::TokenType::RPAR, Lexer::TokenType::RPAR },
        { Lexer::TokenType::RPAR, Lexer::TokenType::PLUS },
        { Lexer::TokenType::RPAR, Lexer::TokenType::MINUS },
        { Lexer::TokenType::RPAR, Lexer::TokenType::MULT },
        { Lexer::TokenType::RPAR, Lexer::TokenType::COMMA },
        { Lexer::TokenType::RPAR, Lexer::TokenType::ENDOFFILE },
        { Lexer::TokenType::PLUS, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INT },
        { Lexer::TokenType::PLUS, Lexer::TokenType::ID },
        { Lexer::TokenType::PLUS, Lexer::TokenType::X },
        { Lexer::TokenType::PLUS, Lexer::TokenType::Y },
        { Lexer::TokenType::PLUS, Lexer::TokenType::Z },
        { Lexer::TokenType::PLUS, Lexer::TokenType::W },
        { Lexer::TokenType::PLUS, Lexer::TokenType::LPAR },
        { Lexer::TokenType::PLUS, Lexer::TokenType::CALC },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERX },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERY },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERZ },
        { Lexer::TokenType::PLUS, Lexer::TokenType::DERW },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTX },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTY },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTZ },
        { Lexer::TokenType::PLUS, Lexer::TokenType::INTW },
        { Lexer::TokenType::MINUS, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INT },
        { Lexer::TokenType::MINUS, Lexer::TokenType::ID },
        { Lexer::TokenType::MINUS, Lexer::TokenType::X },
        { Lexer::TokenType::MINUS, Lexer::TokenType::Y },
        { Lexer::TokenType::MINUS, Lexer::TokenType::Z },
        { Lexer::TokenType::MINUS, Lexer::TokenType::W },
        { Lexer::TokenType::MINUS, Lexer::TokenType::LPAR },
        { Lexer::TokenType::MINUS, Lexer::TokenType::CALC },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERX },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERY },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERZ },
        { Lexer::TokenType::MINUS, Lexer::TokenType::DERW },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTX },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTY },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTZ },
        { Lexer::TokenType::MINUS, Lexer::TokenType::INTW },
        { Lexer::TokenType::MULT, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::MULT, Lexer::TokenType::INT },
        { Lexer::TokenType::MULT, Lexer::TokenType::ID },
        { Lexer::TokenType::MULT, Lexer::TokenType::X },
        { Lexer::TokenType::MULT, Lexer::TokenType::Y },
        { Lexer::TokenType::MULT, Lexer::TokenType::Z },
        { Lexer::TokenType::MULT, Lexer::TokenType::W },
        { Lexer::TokenType::MULT, Lexer::TokenType::LPAR },
        { Lexer::TokenType::MULT, Lexer::TokenType::CALC },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERX },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERY },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERZ },
        { Lexer::TokenType::MULT, Lexer::TokenType::DERW },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTX },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTY },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTZ },
        { Lexer::TokenType::MULT, Lexer::TokenType::INTW },
        { Lexer::TokenType::CARET, Lexer::TokenType::INT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INT },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::ID },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::X },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::Y },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::Z },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::W },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::LPAR },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::MINUS },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::CALC },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERX },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERY },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERZ },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::DERW },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTX },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTY },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTZ },
        { Lexer::TokenType::ASSIGN, Lexer::TokenType::INTW },
        { Lexer::TokenType::COMMA, Lexer::TokenType::FLOAT },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INT },
        { Lexer::TokenType::COMMA, Lexer::TokenType::ID },
        { Lexer::TokenType::COMMA, Lexer::TokenType::X },
        { Lexer::TokenType::COMMA, Lexer::TokenType::Y },
        { Lexer::TokenType::COMMA, Lexer::TokenType::Z },
        { Lexer::TokenType::COMMA, Lexer::TokenType::W },
        { Lexer::TokenType::COMMA, Lexer::TokenType::LPAR },
        { Lexer::TokenType::COMMA, Lexer::TokenType::MINUS },
        { Lexer::TokenType::COMMA, Lexer::TokenType::CALC },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERX },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERY },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERZ },
        { Lexer::TokenType::COMMA, Lexer::TokenType::DERW },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTX },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTY },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTZ },
        { Lexer::TokenType::COMMA, Lexer::TokenType::INTW },
        { Lexer::TokenType::CALC, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERX, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERY, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERZ, Lexer::TokenType::LPAR },
        { Lexer::TokenType::DERW, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTX, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTY, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTZ, Lexer::TokenType::LPAR },
        { Lexer::TokenType::INTW, Lexer::TokenType::LPAR }
    };

    bool validateToken(TokenType prev, TokenType cur)
    {
        return validTokenSequences.count({ prev, cur }) > 0;
    }

    bool isBinaryOperator(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::CARET:
        case TokenType::ASSIGN:
            return true;
        default:
            return false;
        }
    }

    bool isPrefixOperator(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::MINUS:
        case TokenType::CALC:
        case TokenType::DERX:
        case TokenType::DERY:
        case TokenType::DERZ:
        case TokenType::DERW:
        case TokenType::INTX:
        case TokenType::INTY:
        case TokenType::INTZ:
        case TokenType::INTW:
            return true;
        default:
            return false;
        }
    }

    bool isFunction(const Token& token)
    {
        switch (token.type())
        {
        case TokenType::CALC:
        case TokenType::DERX:
        case TokenType::DERY:
        case TokenType::DERZ:
        case TokenType::DERW:
        case TokenType::INTX:
        case TokenType::INTY:
        case TokenType::INTZ:
        case TokenType::INTW:
            return true;
        default:
            return false;
        }
    }

    int getFuncCommaCount(TokenType type)
    {
        switch (type)
        {
        case TokenType::CALC: return 4;
        case TokenType::DERX: return 0;
        case TokenType::DERY: return 0;
        case TokenType::DERZ: return 0;
        case TokenType::DERW: return 0;
        case TokenType::INTX: return 0;
        case TokenType::INTY: return 0;
        case TokenType::INTZ: return 0;
        case TokenType::INTW: return 0;
        default:
            throw std::invalid_argument(__FUNCTION__ ": Unknown operation provided");
        }
    }

    PostfixMember toMember(Token token, bool unary = false)
    {
        switch (token.type())
        {
        case TokenType::PLUS: return PostfixMember(token, 2);
        case TokenType::MINUS: return PostfixMember(token, 2, unary);
        case TokenType::MULT: return PostfixMember(token, 3);
        case TokenType::CARET: return PostfixMember(token, 4);
        case TokenType::ASSIGN: return PostfixMember(token, 0, false, PostfixMember::RIGHTTOLEFT);
        case TokenType::CALC:
        case TokenType::DERX:
        case TokenType::DERY:
        case TokenType::DERZ:
        case TokenType::DERW:
        case TokenType::INTX:
        case TokenType::INTY:
        case TokenType::INTZ:
        case TokenType::INTW: return PostfixMember(token, 0, true);
        case TokenType::LPAR: return PostfixMember(token);
        case TokenType::ID: return PostfixMember(token);
        default: throw std::invalid_argument(__FUNCTION__ ": Unknown operation provided");
        }
    }

    struct CompilationBlock
    {
        TokenType startTokenType;
        // «десь хран€тс€ количества аргументов функций - 1 (по количеству зап€тых)
        int commaCount;
    };

    struct OpInfo
    {
        Intr::Opcode opcode;
        PostfixType resType;
        std::vector<int> argFlags;

        OpInfo(Intr::Opcode opc, PostfixType res, const std::vector<int>& args) : opcode(opc), resType(res), argFlags(args) {}
    };

    const static std::vector<std::pair<TokenType, OpInfo>> ops{
        { TokenType::ASSIGN, OpInfo(Intr::Opcode::ASSIGN, PostfixType::POLYNOMIAL, { (int)PostfixType::ID, (int)PostfixType::ANY })},
        { TokenType::PLUS, OpInfo(Intr::Opcode::ADD, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY, (int)PostfixType::ANY })},
        { TokenType::MINUS, OpInfo(Intr::Opcode::SUBTRACT, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY, (int)PostfixType::ANY })},
        { TokenType::CARET, OpInfo(Intr::Opcode::POWER, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY, (int)PostfixType::INT })},
        { TokenType::MULT, OpInfo(Intr::Opcode::MULT, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY, (int)PostfixType::ANY })},
    };

    const static std::vector<std::pair<TokenType, OpInfo>> prefixOps{
        { TokenType::CALC, OpInfo(Intr::Opcode::CALC, PostfixType::FLOAT, { (int)PostfixType::ANY, (int)PostfixType::FLOAT | (int)PostfixType::INT, (int)PostfixType::FLOAT | (int)PostfixType::INT, (int)PostfixType::FLOAT | (int)PostfixType::INT, (int)PostfixType::FLOAT | (int)PostfixType::INT})},
        { TokenType::DERX, OpInfo(Intr::Opcode::DERX, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::DERY, OpInfo(Intr::Opcode::DERY, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::DERZ, OpInfo(Intr::Opcode::DERZ, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::DERW, OpInfo(Intr::Opcode::DERW, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::INTX, OpInfo(Intr::Opcode::INTX, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::INTY, OpInfo(Intr::Opcode::INTY, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::INTZ, OpInfo(Intr::Opcode::INTZ, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::INTW, OpInfo(Intr::Opcode::INTW, PostfixType::POLYNOMIAL, { (int)PostfixType::ANY })},
        { TokenType::MINUS, OpInfo(Intr::Opcode::UMINUS, PostfixType::FLOAT, { (int)PostfixType::FLOAT, })},
        { TokenType::MINUS, OpInfo(Intr::Opcode::UMINUS, PostfixType::FLOAT, { (int)PostfixType::INT, })},
        { TokenType::MINUS, OpInfo(Intr::Opcode::UMINUS, PostfixType::POLYNOMIAL, { (int)PostfixType::POLYNOMIAL, })},
    };

    class CompilationContext
    {
    private:
        std::vector<Token> tokens;
        Intr::Program prog;
        Stack<PostfixMember> stack;
        Stack<CompilationBlock> blocks;
        Stack<PostfixType> types;

        size_t tokIndex;
        Token prev;
        Token tok;

        Intr::Op CompileOperation(const PostfixMember& member)
        {
            if (member.type() == TokenType::ID)
            {
                types.push(PostfixType::ID);
                return member.token().value();
            }

            const std::vector<std::pair<TokenType, OpInfo>>& funcs = member.isPrefixOp() ? prefixOps : ops;

            for (const auto& row : funcs)
            {
                if (row.first != member.type()) continue;

                const auto& op = row.second;

                if (types.size() < op.argFlags.size()) continue;

                bool valid = true;

                Stack<PostfixType> tmp;
                for (int i = op.argFlags.size() - 1; i >= 0; --i)
                {
                    PostfixType type = types.top();
                    if (((int)type & op.argFlags[i]) == 0)
                        valid = false;

                    tmp.push(type);
                    types.pop();
                }

                if (!valid)
                {
                    while (tmp.size())
                    {
                        types.push(tmp.top());
                        tmp.pop();
                    }
                    continue;
                }


                types.push(op.resType);
                return op.opcode;
            }

            throw std::runtime_error(__FUNCTION__ ": compilation error occurred.");
        }

        void compileToTok(const std::vector<TokenType>& types, bool popFound)
        {
            while (stack.size() && std::find(types.begin(), types.end(), stack.top().type()) == types.end())
            {
                prog.push_back(CompileOperation(stack.top()));
                stack.pop();
            }

            if (stack.size() > 0 && popFound) stack.pop();
        }

        void compilePrefixOps()
        {
            while (stack.size() && stack.top().isPrefixOp())
            {
                prog.push_back(CompileOperation(stack.top()));
                stack.pop();
            }
        }
    public:
        CompilationContext(const std::vector<Token>& toks)
        {
            tokens = toks;
            tokIndex = 0;
            prev = tok = Token(TokenType::NONE);
        }

        TokenType curType() const { return tok.type(); }
        TokenType prevType() const { return prev.type(); }

        const Intr::Program& getProgram() const { return prog; }

        void nextTok()
        {
            prev = tok;
            tok = tokens[tokIndex++];

            if (!validateToken(prevType(), curType()))
                throw SyntaxError{ tok.startPos(), "Unexpected token" };
        }

        bool isTok(TokenType type)
        {
            return curType() == type;
        }

        bool isPrefixOp()
        {
            return isPrefixOperator(tok) && (prevType() == TokenType::NONE || prevType() == TokenType::LPAR || prevType() == TokenType::COMMA || isBinaryOperator(prev));
        }

        bool isBinaryOp()
        {
            return isBinaryOperator(tok);
        }

        void openNewBlock()
        {
            if (isTok(TokenType::LPAR))
            {
                int commaCount = 0;
                if (stack.size() && isFunction(stack.top().type()))
                    commaCount = getFuncCommaCount(stack.top().type());

                blocks.push({ TokenType::LPAR, commaCount });
                stack.push(toMember(TokenType::LPAR));
            } else if (isTok(TokenType::NONE))
            {
                blocks.push({ TokenType::NONE, 0 });
            }
        }

        void closeBlock()
        {
            if (blocks.top().commaCount > 0)
                throw SyntaxError{ tok.startPos(), "Not enough arguments" };

            if (isTok(TokenType::RPAR))
            {
                if (blocks.top().startTokenType != TokenType::LPAR)
                    throw SyntaxError{ tok.startPos(), "Unexpected right parenthesis" };

                blocks.pop();

                compileToTok({ TokenType::LPAR, }, true);
                compilePrefixOps();
            } else if (isTok(TokenType::ENDOFFILE))
            {
                if (blocks.size() != 1 || blocks.top().startTokenType != TokenType::NONE)
                    throw SyntaxError{ tok.startPos(), "Unclosed parenthesis" };

                blocks.pop();

                compileToTok({}, false);
            }
        }

        void applyComma()
        {
            CompilationBlock val = blocks.top();

            if (val.commaCount == 0)
                throw SyntaxError{ tok.startPos(), "Too many arguments" };

            compileToTok({ TokenType::LPAR, }, false);

            blocks.pop();
            val.commaCount--;
            blocks.push(val);
        }

        void compilePrefixOp()
        {
            stack.push(toMember(tok, true));
        }

        void compileBinaryOp()
        {
            PostfixMember member = toMember(tok);
            // ѕрошлые с таким же приоритетом выполн€ютс€ при левой ассоциативности, но не выполн€ютс€ при правой
            if (member.associativity() == PostfixMember::LEFTTORIGHT)
            {
                while (stack.size() && member.precedence() <= stack.top().precedence())
                {
                    prog.push_back(CompileOperation(stack.top()));
                    stack.pop();
                }
            } else
            {
                while (stack.size() && member.precedence() < stack.top().precedence())
                {
                    prog.push_back(CompileOperation(stack.top()));
                    stack.pop();
                }
            }

            stack.push(member);
        }

        void compileId()
        {
            prog.push_back(CompileOperation(toMember(tok)));
        }

        void compileMonomial()
        {
            const std::unordered_set<TokenType> monomialTokens = {
                TokenType::INT, TokenType::FLOAT,
                TokenType::X, TokenType::Y, TokenType::Z, TokenType::W,
                TokenType::CARET
            };

            if (!monomialTokens.count(tok.type()))
                throw SyntaxError{ tok.startPos(), "Expected polynomial" };

            size_t startIndex = tok.startPos();
            size_t count = 0;
            std::stringstream monomialStr;

            size_t tmpIndex = tokIndex;
            Token tmpPrev = prev;
            Token tmpTok = tok;

            for (; monomialTokens.count(tmpTok.type()); tmpPrev = tmpTok, tmpTok = tokens[tmpIndex++])
            {
                monomialStr << tmpTok.value();

                prev = tmpPrev;
                tok = tmpTok;
                tokIndex = tmpIndex;
                ++count;
            }

            if (count == 1 && (isTok(TokenType::INT) || isTok(TokenType::FLOAT)))
            {
                if (isTok(TokenType::INT))
                {
                    try
                    {
                        unsigned long val = std::stoul(tok.value());
                        types.push(PostfixType::INT);
                        prog.push_back(val);
                    }
                    catch (std::out_of_range)
                    {
                        throw SyntaxError{ tok.startPos(), "Too big integer" };
                    }
                } else
                {
                    try
                    {
                        double val = std::stod(tok.value());
                        types.push(PostfixType::INT);
                        prog.push_back(val);
                    }
                    catch (std::out_of_range)
                    {
                        throw SyntaxError{ tok.startPos(), "Too big float" };
                    }
                }
            } else
            {
                std::variant<Polynomial, SyntaxError> res = Polynomial::fromString(monomialStr.str());
                if (std::holds_alternative<SyntaxError>(res))
                {
                    SyntaxError err = std::get<SyntaxError>(res);
                    err.pos = startIndex + err.pos;
                    throw err;
                }

                types.push(PostfixType::POLYNOMIAL);
                prog.push_back(std::get<Polynomial>(res));
            }
        }
    };

    std::variant<Intr::Program, SyntaxError> ExpressionCompiler::compileExpression(
        const std::vector<Lexer::Token>& tokens)
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

                if (ctx.isTok(TokenType::LPAR))
                {
                    ctx.openNewBlock();

                } else if (ctx.isTok(TokenType::COMMA))
                {
                    ctx.applyComma();

                } else if (ctx.isTok(TokenType::RPAR) || ctx.isTok(TokenType::ENDOFFILE))
                {
                    ctx.closeBlock();

                } else if (ctx.isPrefixOp())
                {
                    ctx.compilePrefixOp();

                } else if (ctx.isBinaryOp())
                {
                    ctx.compileBinaryOp();

                } else if (ctx.isTok(TokenType::ID))
                {
                    ctx.compileId();

                } else
                {
                    ctx.compileMonomial();
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