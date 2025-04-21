#include <table.h>

#define DEFAULT_ORDERED_TABLE_SIZE 4

OrderedTable::OrderedTable()
{}

std::optional<Polynomial> OrderedTable::findPolynomial(const std::string& polName)
{
    if (mTable.size() == 0) return std::nullopt;
    int i = mTable.size() / 2;
    int leftBorder = 0;
    int rightBorder = mTable.size() - 1;

    while (rightBorder >= leftBorder && mTable[i].key != polName) // binary search
    {
        if (mTable[i].key > polName == 1)
        {
            rightBorder = i - 1;
            i = (leftBorder + rightBorder) / 2;
        } else if (mTable[i].key < polName == 1)
        {
            leftBorder = i + 1;
            i = (leftBorder + rightBorder) / 2;
        }
    }
    if (mTable[i].key == polName)
        return mTable[i].value;
    return std::nullopt;
}

void OrderedTable::addPolynomial(const std::string& polName, const Polynomial& pol)
{
    if (findPolynomial(polName) != std::nullopt) // uniqueness check
        throw "There already is a polynomial with that name";
    int i = mTable.size() / 2;
    int leftBorder = 0;
    int rightBorder = mTable.size() - 1;
    while (rightBorder >= leftBorder) // binary search
    {
        if (mTable[i].key > polName)
        {
            rightBorder = i - 1;
            i = (leftBorder + rightBorder) / 2;
        } else if (mTable[i].key < polName)
        {
            leftBorder = i + 1;
            i = (leftBorder + rightBorder) / 2;
        }
    }
    if (mTable.size() == 0)
        mTable.push_back({ polName, pol });
    else if (mTable[i].key > polName)
        mTable.insert(mTable.begin() + i, { polName, pol });
    else if (mTable[i].key < polName)
        mTable.insert(mTable.begin() + i + 1, { polName, pol });
}

void OrderedTable::delPolynomial(const std::string& polName)
{
    if (findPolynomial(polName) == std::nullopt) return; // uniqueness check
    int i = mTable.size() / 2;
    int leftBorder = 0;
    int rightBorder = mTable.size() - 1;
    while (rightBorder >= leftBorder && mTable[i].key != polName)
    {
        if (mTable[i].key > polName)
        {
            rightBorder = i - 1;
            i = (leftBorder + rightBorder) / 2;
        } else if (mTable[i].key < polName)
        {
            leftBorder = i + 1;
            i = (leftBorder + rightBorder) / 2;
        }
    }
    mTable.erase(mTable.begin() + i);
}

unsigned int OrderedTable::size()
{
    return mTable.size();
}

bool OrderedTable::empty()
{
    return mTable.size() == 0;
}

std::vector<std::pair< std::string, Polynomial>> OrderedTable::getPolynomials()
{
    std::vector<std::pair< std::string, Polynomial>> result(mTable.size());
    for (int i = 0; i < mTable.size(); i++)
    {
        result[i].first = mTable[i].key;
        result[i].second = mTable[i].value;
    }
    return result;
}
