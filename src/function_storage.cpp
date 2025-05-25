#include "function_storage.h"

std::optional<SuitableFunction> FunctionStorage::findFunction(const std::string& name,
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