#pragma once
#include <string>
#include "table.h"
#include "type_storage.h"

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
    std::vector<size_t> mMemoryIndices;
    std::vector<uint8_t> mMemory;
    
    const TypeStorage& mTypeStorage;

    size_t mNextLiteralIndex;
public:
    VariablesStorage(const TypeStorage& TypeStorage) : mTypeStorage(TypeStorage) {}

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

        size_t dataSize = mTypeStorage.getTypeInfo(info.type()).value().size();

        VariableId id = static_cast<VariableId>(mInfos.size());
        mInfos.push_back(info);
        mVariableTable.add(info.name(), id);

        size_t memIndex = mMemory.size();
        mMemoryIndices.push_back(memIndex);
        mMemory.resize(mMemory.size() + dataSize);

        *reinterpret_cast<T*>(&mMemory[memIndex]) = defaultValue;

        return id;
    }

    template<typename T>
    VariableId getIdByLiteral(DataTypeId type, const T& value)
    {
        // TODO: think about returning same value for same literals

        if (sizeof(T) != mTypeStorage.getTypeInfo(type).value().size())
            throw std::runtime_error(__FUNCTION__ ": type size doesn't match value size");

        std::string name = "$" + std::to_string(mNextLiteralIndex++);
        VariableInfo info(name, type, true);

        return registerVariable(info, value);
    }

    const VariableInfo& getInfo(VariableId id) const
    {
        size_t index = static_cast<size_t>(id);

        if (index >= mMemoryIndices.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        return mInfos[index];
    }

    template<typename T>
    void setValue(VariableId id, const T& value) {

        size_t index = static_cast<size_t>(id);

        if (index >= mMemoryIndices.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        if (mInfos[index].isConstant())
            throw std::runtime_error(__FUNCTION__ ": can't set value to constant.");

        size_t memIndex = mMemoryIndices[index];
        *reinterpret_cast<T*>(&mMemory[memIndex]) = value;
    }
    
    template<typename T>
    T getValue(VariableId id) const {
        
        size_t index = static_cast<size_t>(id);

        if (index >= mMemoryIndices.size())
            throw std::runtime_error(__FUNCTION__ ": variable doesn't exist.");

        size_t memIndex = mMemoryIndices[index];
        return *reinterpret_cast<T*>(*mMemory[memIndex]);
    }
};