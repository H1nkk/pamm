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

    virtual ~Table() = 0 {};
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
