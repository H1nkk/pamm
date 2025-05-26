#pragma once
#include <string>
#include "variant_index.h"
#include "table.h"
#include "type_storage.h"
#include "data_values.h"

using VariableId = unsigned long long;

class VariableInfo final {
private:
    std::string mName;
    DataTypeId mTypeId;
    bool mIsConstant;
public:
    VariableInfo(const std::string& name, DataTypeId typeId, bool isConstant) :
        mName(name), mTypeId(typeId), mIsConstant(isConstant) {}

    std::string name() const { return mName; }
    DataTypeId type() const { return mTypeId; }
    bool isConstant() const { return mIsConstant; }
};

class VariablesStorage final {
private:
    Table<std::string, VariableId> mVariableTable;
    std::vector<VariableInfo> mInfos;
    std::vector<DataValue> mMemory;
    
    const TypeStorage& mTypeStorage;

    size_t mNextLiteralIndex;
public:
    VariablesStorage(const TypeStorage& TypeStorage) : mTypeStorage(TypeStorage),
        mNextLiteralIndex(0) {}

    std::optional<VariableId> getId(const std::string& name) const
    {
        return mVariableTable.find(name);
    }

    bool isVariableName(const std::string& name) const
    {
        return getId(name).has_value();
    }

    template<typename T>
    VariableId registerVariable(const VariableInfo& info, const T& defaultValue) {
        if (isVariableName(info.name()))
            throw std::runtime_error(__FUNCTION__ ": variable is already registered.");

        size_t dataSize = mTypeStorage.getTypeInfo(info.type()).value().index();

        if (variant_index<DataValue, T>() != info.type())
            throw std::runtime_error(__FUNCTION__ ": incorrect data type.");

        VariableId id = static_cast<VariableId>(mInfos.size());
        mInfos.push_back(info);
        mMemory.push_back(defaultValue);
        mVariableTable.add(info.name(), id);

        return id;
    }

    template<typename T>
    VariableId getIdByLiteral(DataTypeId type, const T& value)
    {
        // TODO: think about returning same value for same literals

        if (variant_index<DataValue, T>() != mTypeStorage.getTypeInfo(type).value().index())
            throw std::runtime_error(__FUNCTION__ ": incorrect data type.");
        
        std::string name = "$" + std::to_string(mNextLiteralIndex++);
        VariableInfo info(name, type, true);

        return registerVariable(info, value);
    }

    const VariableInfo& getInfo(VariableId id) const
    {
        size_t index = static_cast<size_t>(id);

        if (index >= mInfos.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        return mInfos[index];
    }

    template<typename T>
    void setValue(VariableId id, const T& value) {

        size_t index = static_cast<size_t>(id);

        if (variant_index<DataValue, T>() != getInfo(id).type())
            throw std::runtime_error(__FUNCTION__ ": incorrect data type.");

        if (index >= mMemory.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        if (mInfos[index].isConstant())
            throw std::runtime_error(__FUNCTION__ ": can't set value to constant.");

        mMemory[index] = value;
    }
    
    template<typename T>
    T getValue(VariableId id) const {
        
        size_t index = static_cast<size_t>(id);

        if (variant_index<DataValue, T>() != getInfo(id).type())
            throw std::runtime_error(__FUNCTION__ ": incorrect data type.");

        if (index >= mMemory.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        return std::get<T>(mMemory[index]);
    }
};