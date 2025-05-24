#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include "table.h"
#include "type_storage.h"

using FunctionId = unsigned long long;

class FunctionInfo final {
private:
    std::string mName;
    DataTypeId mReturnType;
    std::vector<DataTypeId> mArgumentTypes;
    int32_t mCost; // only for type casting now
    FunctionId mInterpreterId;
public:
    FunctionInfo(const std::string& name, const DataTypeId& retType,
        const std::vector<DataTypeId> argTypes, FunctionId intrId, int32_t cost = 0) :
        mName(name), mReturnType(retType), mArgumentTypes(argTypes),
        mInterpreterId(intrId), mCost(cost) {}

    std::string name() const { return mName; }
    DataTypeId returnType() const { return mReturnType; }
    
    size_t argumentsCount() const { return mArgumentTypes.size(); }
    DataTypeId getArgument(size_t index) const { return mArgumentTypes[index]; }

    int32_t cost() const { return mCost; }
    FunctionId interpreterId() const { return mInterpreterId; }

    friend class FunctionStorage;
};

class FunctionStorage final {
private:
    std::map<std::string, std::vector<FunctionInfo>> mFunctions;

    const TypeStorage& mTypeStorage;
public:
    FunctionStorage(const TypeStorage& typeStorage) : mTypeStorage(typeStorage) {}

    void registerFunction(const FunctionInfo& funcInfo) {
        mFunctions[funcInfo.name()].push_back(funcInfo);
    }

    bool isFunctionName(const std::string& name) const
    {
        return mFunctions.count(name);
    }

    std::optional<FunctionInfo> findFunction(const std::string& name, 
        const std::vector<DataTypeId>& arguments) const {

        auto iter = mFunctions.find(name);

        if (iter != mFunctions.end())
        {
            // add type casting management
            for (const auto& row : iter->second)
            {
                if (name == row.name() &&
                    std::equal(arguments.begin(), arguments.end(), row.mArgumentTypes.begin())
                    )
                {
                    return row;
                }
            }
        }

        return std::nullopt;
    }
};