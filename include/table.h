#pragma once
#include <string>
#include <vector>
#include <optional>

template<typename K, typename V>
class Table final
{
private:
    struct Entry
    {
        K key;
        V value;
    };

    std::vector<Entry> mTable;

public:
    std::optional<V> find(const K& name) const {
        if (mTable.size() == 0) return std::nullopt;
        int i = mTable.size() / 2;
        int leftBorder = 0;
        int rightBorder = mTable.size() - 1;

        while (rightBorder >= leftBorder && mTable[i].key != name) // binary search
        {
            if (mTable[i].key > name == 1)
            {
                rightBorder = i - 1;
                i = (leftBorder + rightBorder) / 2;
            } else if (mTable[i].key < name == 1)
            {
                leftBorder = i + 1;
                i = (leftBorder + rightBorder) / 2;
            }
        }
        if (mTable[i].key == name)
            return mTable[i].value;
        return std::nullopt;
    }

    void add(const K& name, const V& pol) {
        if (find(name) != std::nullopt) // uniqueness check
        throw "There already is a polynomial with that name";
        int i = mTable.size() / 2;
        int leftBorder = 0;
        int rightBorder = mTable.size() - 1;
        while (rightBorder >= leftBorder) // binary search
        {
            if (mTable[i].key > name)
            {
                rightBorder = i - 1;
                i = (leftBorder + rightBorder) / 2;
            } else if (mTable[i].key < name)
            {
                leftBorder = i + 1;
                i = (leftBorder + rightBorder) / 2;
            }
        }
        if (mTable.size() == 0)
            mTable.push_back({ name, pol });
        else if (mTable[i].key > name)
            mTable.insert(mTable.begin() + i, { name, pol });
        else if (mTable[i].key < name)
            mTable.insert(mTable.begin() + i + 1, { name, pol });
    }

    void del(const K& name) {
        if (find(name) == std::nullopt) return; // uniqueness check
        int i = mTable.size() / 2;
        int leftBorder = 0;
        int rightBorder = mTable.size() - 1;
        while (rightBorder >= leftBorder && mTable[i].key != name)
        {
            if (mTable[i].key > name)
            {
                rightBorder = i - 1;
                i = (leftBorder + rightBorder) / 2;
            } else if (mTable[i].key < name)
            {
                leftBorder = i + 1;
                i = (leftBorder + rightBorder) / 2;
            }
        }
        mTable.erase(mTable.begin() + i);
    }

    unsigned int size() const {
        return mTable.size();
    }

    bool empty() const {
        return mTable.size() == 0;
    }

    std::vector<std::pair<K, V>> getEntries() const {
        std::vector<std::pair<K, V>> result(mTable.size());
        for (int i = 0; i < mTable.size(); i++)
        {
            result[i].first = mTable[i].key;
            result[i].second = mTable[i].value;
        }
        return result;
    }
};
