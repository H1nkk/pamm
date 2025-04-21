#include "expression_interpreter/expression_interpreter.h"
#include "table.h"

namespace Intr {

    class VirtualMachine final
    {
    private:
        Aggregator* pAggregator;
        Stack<Op> mOperands;

        double getDoubleOp()
        {
            Op op = mOperands.top();
            mOperands.pop();

            if (std::holds_alternative<unsigned long>(op))
            {
                return std::get<unsigned long>(op);
            } else if (std::holds_alternative<double>(op))
            {
                double res = std::get<double>(op);

                if (!isfinite(res))
                    throw "Overflow error occurred";

                return res;
            } else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        Polynomial getPolynomialOp()
        {
            Op op = mOperands.top();
            mOperands.pop();

            if (std::holds_alternative<Polynomial>(op))
            {
                return std::get<Polynomial>(op);
            } else if (std::holds_alternative<unsigned long>(op))
            {
                return Polynomial(std::get<unsigned long>(op));
            } else if (std::holds_alternative<double>(op))
            {
                double res = std::get<double>(op);

                if (!isfinite(res))
                    throw "Overflow error occurred";

                return Polynomial(res);
            } else if (std::holds_alternative<std::string>(op))
            {
                auto p = pAggregator->findPolynomial(std::get<std::string>(op));
                if (!p.has_value())
                {
                    throw "Polynomial with name \'" + std::get<std::string>(op) + "\' does not exist";
                }

                return p.value();
            } else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        void add()
        {
            Polynomial p1 = getPolynomialOp();
            Polynomial p2 = getPolynomialOp();
            mOperands.push(p2 + p1);
        }

        void subtract()
        {
            Polynomial p1 = getPolynomialOp();
            Polynomial p2 = getPolynomialOp();
            mOperands.push(p2 - p1);
        }

        void multiply()
        {
            Polynomial p1 = getPolynomialOp();
            Polynomial p2 = getPolynomialOp();
            mOperands.push(p2 * p1);
        }

        void assign()
        {
            Polynomial p = getPolynomialOp();

            Op name = mOperands.top();
            mOperands.pop();

            if (!std::holds_alternative<std::string>(name))
            {
                throw std::runtime_error(__FUNCTION__ ": expected identifier.");
            }

            mOperands.push(p);
            pAggregator->addPolynomial(std::get<std::string>(name), p);
        }

        void negate()
        {
            Op op = mOperands.top();
            mOperands.pop();

            if (std::holds_alternative<Polynomial>(op))
            {
                mOperands.push(-std::get<Polynomial>(op));
            } else if (std::holds_alternative<unsigned long>(op))
            {
                mOperands.push(-(double)std::get<unsigned long>(op));
            } else if (std::holds_alternative<double>(op))
            {
                mOperands.push(-std::get<double>(op));
            } else if (std::holds_alternative<std::string>(op))
            {
                auto p = pAggregator->findPolynomial(std::get<std::string>(op));
                if (!p.has_value())
                {
                    throw "Polynomial with name \'" + std::get<std::string>(op) + "\' does not exist";
                }

                mOperands.push(-p.value());
            } else
            {
                throw std::runtime_error(__FUNCTION__ ": unknown operand type.");
            }
        }

        void calc()
        {
            double z = getDoubleOp();
            double y = getDoubleOp();
            double x = getDoubleOp();
            double w = getDoubleOp();
            Polynomial p = getPolynomialOp();
            mOperands.push(p.evaluate(w, x, y, z));
        }

        void derx()
        {
            mOperands.push(getPolynomialOp().derivativeX());
        }

        void dery()
        {
            mOperands.push(getPolynomialOp().derivativeY());
        }

        void derz()
        {
            mOperands.push(getPolynomialOp().derivativeZ());
        }

        void derw()
        {
            mOperands.push(getPolynomialOp().derivativeW());
        }

        void intx()
        {
            mOperands.push(getPolynomialOp().integralX());
        }

        void inty()
        {
            mOperands.push(getPolynomialOp().integralY());
        }

        void intz()
        {
            mOperands.push(getPolynomialOp().integralZ());
        }

        void intw()
        {
            mOperands.push(getPolynomialOp().integralW());
        }

    public:
        VirtualMachine(Aggregator* aggregator) : pAggregator(aggregator) {}

        void execOp(const Op& op)
        {
            if (!std::holds_alternative<Opcode>(op))
            {
                mOperands.push(op);
                return;
            }

            Opcode opcode = std::get<Opcode>(op);
            switch (opcode)
            {
            case Opcode::ADD: add(); break;
            case Opcode::SUBTRACT: subtract(); break;
            case Opcode::MULT: multiply(); break;
            case Opcode::UMINUS: negate(); break;
            case Opcode::ASSIGN: assign(); break;
            case Opcode::CALC: calc(); break;
            case Opcode::DERX: derx(); break;
            case Opcode::DERY: dery(); break;
            case Opcode::DERZ: derz(); break;
            case Opcode::DERW: derw(); break;
            case Opcode::INTX: intx(); break;
            case Opcode::INTY: inty(); break;
            case Opcode::INTZ: intz(); break;
            case Opcode::INTW: intw(); break;
            default: throw std::runtime_error(__FUNCTION__ ": operation is not supported.");
            }
        }

        Polynomial getResult()
        {
            return getPolynomialOp();
        }
    };

    std::variant<Polynomial, std::string> ExpressionInterpreter::execute(const Program& program)
    {
        VirtualMachine vm(mpAggregator);

        for (size_t i = 0; i < program.size(); ++i)
        {
            vm.execOp(program[i]);
        }

        return vm.getResult();
    }
}