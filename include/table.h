#pragma once
#include "polynomial.h"
#include "red_black_tree.h"
#include <string>
#include <vector>
#include <optional>

class Table
{
public:

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) = 0; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) = 0;
    virtual void delPolynomial(const std::string& polName) = 0;
    virtual unsigned int size() = 0;
    virtual bool empty() = 0;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() = 0;

    virtual ~Table() = 0 {}; // removed {}, may be its bad
};

class LinearArrTable : public Table
{
private:
    struct Pol
    {
        std::string key;
        Polynomial value;
    };

    std::vector<Pol> mTable;

public:
    LinearArrTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty();
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;


    virtual ~LinearArrTable() {};
};


class LinearListTable : public Table
{
private:
    struct Node
    {
        std::string key;
        Polynomial value;
        Node* pNext;
    };
    Node* pFirst;
    size_t mTableSize;

public:
    LinearListTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty() override;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;

    virtual ~LinearListTable();
};


class OrderedTable : public Table
{
private:
    struct Pol
    {
        std::string key;
        Polynomial value;
    };

    std::vector<Pol> mTable;

public:
    OrderedTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty() override;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;

    virtual ~OrderedTable() {};
};


class TreeTable : public Table
{
private:
    RedBlackTree<std::string, Polynomial> mTree;
public:
    TreeTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty() override;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;

    virtual ~TreeTable() {};
};


class OpenAddressHashTable : public Table
{
private:
    struct Node
    {
        int status; // 1 - занята, 0 - пуста, -1 - удалена
        std::string key;
        Polynomial value;
    };

    std::vector<Node> mTable;
    size_t step;
    size_t mTableSize;
    size_t mCurrentSize;

    unsigned int hashFunc(const std::string& key);

public:
    OpenAddressHashTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty() override;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;

    virtual ~OpenAddressHashTable() {};
};


class SeparateChainingHashTable : public Table
{
private:
    struct Node
    {
        std::string key;
        Polynomial value;
        Node* pNextInChain;
    };

    std::vector<Node*> mTable;
    size_t mTableSize;
    size_t mCurrentSize;

    unsigned int hashFunc(const std::string& key);

public:

    SeparateChainingHashTable();

    virtual std::optional<Polynomial> findPolynomial(const std::string& polName) override; // find polynomial named polName
    virtual void addPolynomial(const std::string& polName, const Polynomial& pol) override;
    virtual void delPolynomial(const std::string& polName) override;
    virtual unsigned int size() override;
    virtual bool empty() override;
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials() override;

    virtual ~SeparateChainingHashTable() {};
};


class Aggregator
{
private:
    std::vector<Table*> mTables;
    int mCurrentTable;

public:
    Aggregator();

    void selectTable(const std::string& tableName); // select table by its name: mTables[0] - liar (short for linear array table - contains LinearArrTable object)
    //			 				  mTables[1] - lili (short for linear list table - contains LinearListTable object)
    //		 					  mTables[2] - ordr (short for ordered array table - contains OrderedTable object)
    //							  mTables[3] - mTree (short for mTree table - contains TreeTable object)
    //							  mTables[4] - opha (short for open Address hash table - contains OpenAddressHashTable object)
    //							  mTables[5] - seha (short for separate chaining hash table - contains SeparateChainingHashTable object)
    std::optional<Polynomial> findPolynomial(const std::string& polName); // find polynomial named polName
    void addPolynomial(const std::string& polName, const Polynomial& pol);
    void delPolynomial(const std::string& polName);
    unsigned int size();
    bool empty();
    virtual std::vector<std::pair< std::string, Polynomial>> getPolynomials();

    ~Aggregator();
};

