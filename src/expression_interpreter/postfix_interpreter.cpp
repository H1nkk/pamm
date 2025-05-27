#include "expression_interpreter/postfix_interpreter.h"

#include <cmath>

std::variant<DataValue, std::string> Intr::PostfixInterpreter::execute(const Program& program, ExecutionContext& context)
{
    while (mStack.size()) mStack.pop();

    for (auto& op : program)
    {
        if (op.first == Opcode::LOAD)
        {
            VariableId varId = op.second;

            const auto& info = context.variableStorage().getInfo(varId);
            std::string typeName = context.typeStorage().getTypeInfo(info.type()).value().name();

            if (typeName == "integer")
            {
                mStack.push(context.variableStorage().getValue<long long>(varId));
            }
            else if (typeName == "double")
            {
                mStack.push(context.variableStorage().getValue<double>(varId));
            }
            else if (typeName == "boolean")
            {
                mStack.push(context.variableStorage().getValue<bool>(varId));
            }
            else if (typeName == "string")
            {
                mStack.push(context.variableStorage().getValue<std::string>(varId));
            }
            else
            {
                throw std::runtime_error(__FUNCTION__ ": type is not supported");
            }

        } else if (op.first == Opcode::CALL)
        {
            FunctionId funcId = op.second;

            switch (funcId)
            {
            case ADD_INT_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue(l + r);
                break;
            }
            case ADD_DOUBLE_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue(l + r);
                break;
            }
            case ADD_STRING_STRING:
            {
                std::string r = popValue<std::string>(), l = popValue<std::string>();
                pushValue(l + r);
                break;
            }
            case SUB_INT_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue(l - r);
                break;
            }
            case SUB_DOUBLE_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue(l - r);
                break;
            }
            case MULT_INT_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue(l * r);
                break;
            }
            case MULT_DOUBLE_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue(l * r);
                break;
            }
            case DIV_INT_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                if (r == 0)
                    return std::string("Zero division occurred! Error.");
                pushValue(l / r);
                break;
            }
            case MOD_INT_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                if (r == 0)
                    return std::string("Zero division occurred! Error.");
                pushValue(l % r);
                break;
            }
            case DIV_DOUBLE_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                if (r == 0.0)
                    return std::string("Zero division occurred! Error.");
                pushValue(l / r);
                break;
            }
            case NEGATE_INT:
            {
                pushValue(-popValue<long long>());
                break;
            }
            case NEGATE_DOUBLE:
            {
                pushValue(-popValue<double>());
                break;
            }
            case LESS_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l < r));
                break;
            }
            case LESS_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l < r));
                break;
            }
            case LESSEQUAL_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l <= r));
                break;
            }
            case LESSEQUAL_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l <= r));
                break;
            }
            case GREATER_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l > r));
                break;
            }
            case GREATER_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l > r));
                break;
            }
            case GREATEREQUAL_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l >= r));
                break;
            }
            case GREATEREQUAL_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l >= r));
                break;
            }
            case EQUAL_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l == r));
                break;
            }
            case EQUAL_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l == r));
                break;
            }
            case NOTEQUAL_INT:
            {
                long long r = popValue<long long>(), l = popValue<long long>();
                pushValue((bool)(l != r));
                break;
            }
            case NOTEQUAL_DOUBLE:
            {
                double r = popValue<double>(), l = popValue<double>();
                pushValue((bool)(l != r));
                break;
            }
            case EQUAL_BOOL:
            {
                bool r = popValue<bool>(), l = popValue<bool>();
                pushValue((bool)(l == r));
                break;
            }
            case NOTEQUAL_BOOL:
            {
                bool r = popValue<bool>(), l = popValue<bool>();
                pushValue((bool)(l != r));
                break;
            }
            case AND_BOOL_BOOL:
            {
                bool r = popValue<bool>(), l = popValue<bool>();
                pushValue((bool)(l && r));
                break;
            }
            case OR_BOOL_BOOL:
            {
                bool r = popValue<bool>(), l = popValue<bool>();
                pushValue((bool)(l || r));
                break;
            }
            case NOT_BOOL:
            {
                pushValue(!popValue<bool>());
                break;
            }
            case CAST_INT_DOUBLE:
            {
                pushValue(static_cast<double>(popValue<long long>()));
                break;
            }
            case ABS_INT:
            {
                pushValue(abs(popValue<long long>()));
                break;
            }
            case ABS_DOUBLE:
            {
                pushValue(abs(popValue<double>()));
                break;
            }
            case EXP_DOUBLE:
            {
                pushValue(exp(popValue<double>()));
                break;
            }
            case SIN_DOUBLE:
            {
                pushValue(sin(popValue<double>()));
                break;
            }
            case COS_DOUBLE:
            {
                pushValue(cos(popValue<double>()));
                break;
            }
            case FLOOR_DOUBLE:
            {
                pushValue(static_cast<long long>(floor(popValue<double>())));
                break;
            }
            case CEIL_DOUBLE:
            {
                pushValue(static_cast<long long>(ceil(popValue<double>())));
                break;
            }
            case ROUND_DOUBLE:
            {
                pushValue(static_cast<long long>(round(popValue<double>())));
                break;
            }
            case TO_STRING_INT:
            {
                pushValue(std::to_string(popValue<long long>()));
                break;
            }
            case TO_STRING_DOUBLE:
            {
                pushValue(std::to_string(popValue<double>()));
                break;
            }
            case TO_STRING_BOOL:
            {
                pushValue(std::to_string(popValue<bool>()));
                break;
            }
            default:
                throw std::runtime_error(__FUNCTION__ ": function is not supported.");
            }
        } else
        {
            throw std::runtime_error(__FUNCTION__ ": opcode is not supported.");
        }
    }

    return mStack.top();
}
