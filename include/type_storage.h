#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <optional>

using DataTypeId = unsigned long long;

class DataType final
{
private:
    std::string mName;
    size_t mSize;
public:
    DataType(const std::string& name, size_t size) : mName(name), mSize(size) {}

    std::string name() const { return mName; }
    size_t size() const { return mSize; }

    bool operator==(const DataType& other) const { return mName == other.mName; }
};

class TypeStorage final
{
private:
    std::unordered_map<std::string, DataTypeId> mTypeTable;
    std::vector<DataType> mTypeInfos;
public:
    bool isTypeName(const std::string& name) const
    {
        return mTypeTable.contains(name);
    }

    DataTypeId registerType(const DataType& type)
    {
        if (isTypeName(type.name()))
            throw std::runtime_error(__FUNCTION__ ": type is already registered.");

        DataTypeId typeId = static_cast<DataTypeId>(mTypeInfos.size());

        mTypeInfos.push_back(type);
        mTypeTable[type.name()] = typeId;

        return typeId;
    }

    std::optional<DataTypeId> getTypeId(const std::string& name) const
    {
        auto iter = mTypeTable.find(name);

        if (iter != mTypeTable.end())
        {
            return iter->second;
        }

        return std::nullopt;
    }

    std::optional<DataType> getTypeInfo(DataTypeId id) const
    {
        size_t index = static_cast<size_t>(id);

        if (id < mTypeInfos.size())
        {
            return mTypeInfos[id];
        }

        return std::nullopt;
    }
};