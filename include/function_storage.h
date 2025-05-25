#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include "table.h"
#include "type_storage.h"

using FunctionId = unsigned long long;

class FunctionInfo final
{
private:
    std::string mName;
    DataTypeId mReturnType;
    std::vector<DataTypeId> mArgumentTypes;
    FunctionId mInterpreterId;
public:
    FunctionInfo(const std::string& name, const DataTypeId& retType,
        const std::vector<DataTypeId> argTypes, FunctionId intrId) :
        mName(name), mReturnType(retType), mArgumentTypes(argTypes),
        mInterpreterId(intrId)
    {}

    std::string name() const { return mName; }
    DataTypeId returnType() const { return mReturnType; }

    size_t argumentsCount() const { return mArgumentTypes.size(); }
    DataTypeId getArgument(size_t index) const { return mArgumentTypes[index]; }

    FunctionId interpreterId() const { return mInterpreterId; }

    friend class FunctionStorage;
};

struct SuitableFunction
{
    FunctionInfo info;
    std::vector<std::vector<FunctionId>> casts;
};

class FunctionStorage final
{
private:
    const TypeStorage& mTypeStorage;

    std::map<std::string, std::vector<FunctionInfo>> mFunctions;
    std::map<std::pair<DataTypeId, DataTypeId>, FunctionId> mTypeCasts;

public:
    FunctionStorage(const TypeStorage& typeStorage) : mTypeStorage(typeStorage) {}

    void registerFunction(const FunctionInfo& funcInfo)
    {
        mFunctions[funcInfo.name()].push_back(funcInfo);

        if (funcInfo.name().starts_with("$cast"))
        {
            if (funcInfo.argumentsCount() != 1)
                throw std::runtime_error(__FUNCTION__ ": cast function must have exactly one argument");

            DataTypeId fromType = funcInfo.getArgument(0);
            DataTypeId toType = funcInfo.returnType();

            mTypeCasts[{ fromType, toType }] = funcInfo.interpreterId();
        }
    }

    bool isFunctionName(const std::string& name) const
    {
        return mFunctions.count(name);
    }

    std::optional<SuitableFunction> findFunction(const std::string& name,
        const std::vector<DataTypeId>& arguments) const
    {
        auto iter = mFunctions.find(name);

        if (iter == mFunctions.end())
            return std::nullopt;

        int optimalCost = INT_MAX;
        size_t optimalIndex = 0;

        for (size_t i = 0; i < iter->second.size(); i++)
        {
            const auto& info = iter->second[i];

            if (info.argumentsCount() != arguments.size())
                continue;

            int cost = 0;

            for (size_t j = 0; j < info.argumentsCount(); j++)
            {
                DataTypeId targetType = info.getArgument(j);
                DataTypeId currentType = arguments[j];

                if (targetType == currentType) continue;

                // targetType != currentType
                if (!mTypeCasts.contains({ currentType, targetType }))
                {
                    cost = INT_MAX;
                    break;
                } else
                {
                    cost++;
                }
            }

            if (cost < optimalCost)
            {
                optimalCost = cost;
                optimalIndex = i;
            }
        }

        if (optimalCost < INT_MAX)
        {
            const auto& info = iter->second[optimalIndex];
            SuitableFunction res{
                info,
                std::vector<std::vector<FunctionId>>(arguments.size())
            };
            
            for (size_t i = 0; i < info.argumentsCount(); i++)
            {
                DataTypeId targetType = info.getArgument(i);
                DataTypeId currentType = arguments[i];

                if (targetType == currentType) continue;

                res.casts[i].push_back(mTypeCasts.at({ currentType, targetType }));
            }

            return res;
        } else
        {
            return std::nullopt;
        }
    }
};