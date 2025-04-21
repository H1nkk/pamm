#include <gtest/gtest.h>
#include "table.h"

// НАДО БЫ ПЕРЕПИСАТЬ TYPED ТЕСТЫ НА НОРМАЛЬНЫЕ...

template <class Table>
class TableTest : public ::testing::Test
{
protected:
    Table table;
};

using TableTypes = ::testing::Types<OrderedTable>;
TYPED_TEST_SUITE(TableTest, TableTypes);

TYPED_TEST(TableTest, defaultTableConstructor)
{
    EXPECT_TRUE(this->table.empty());
    EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, canAddUnpresentPolynomial)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    std::vector<std::pair< std::string, Polynomial>> pol(6);
    this->table.addPolynomial("Pol1", a);
    this->table.addPolynomial("Pol2", a);
    this->table.addPolynomial("Pol4", a);
    this->table.addPolynomial("Pol6", a);
    this->table.addPolynomial("Pol5", a);
    this->table.addPolynomial("Pol3", a);
    pol[0].first = "Pol1";
    pol[0].second = a;
    pol[1].first = "Pol2";
    pol[1].second = a;
    pol[2].first = "Pol3";
    pol[2].second = a;
    pol[3].first = "Pol4";
    pol[3].second = a;
    pol[4].first = "Pol5";
    pol[4].second = a;
    pol[5].first = "Pol6";
    pol[5].second = a;
    EXPECT_FALSE(this->table.empty());
    EXPECT_EQ(this->table.size(), 6);
}

TYPED_TEST(TableTest, cannotAddPresentPolynomial)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    EXPECT_ANY_THROW(this->table.addPolynomial("Pol1", a));
}

TYPED_TEST(TableTest, canFindExsistingPolynomial)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    EXPECT_EQ(this->table.findPolynomial("Pol1"), a);
}

TYPED_TEST(TableTest, cannotFindUnexsistingPolynomial)
{
    EXPECT_EQ(this->table.findPolynomial("Pol1"), std::nullopt);
}

TYPED_TEST(TableTest, canDelPresentPolynomial)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    this->table.delPolynomial("Pol1");
    EXPECT_TRUE(this->table.empty());
    EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, delOfUnpresentPolynomialDoesNothing)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    Polynomial b = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    this->table.addPolynomial("Pol2", b);
    this->table.delPolynomial("Pol3");
    EXPECT_FALSE(this->table.empty());
    EXPECT_EQ(this->table.size(), 2);
}

TYPED_TEST(TableTest, canGetSizeWhenEmpty)
{
    EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, canGetSizeWhenNonEmpty)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    EXPECT_EQ(this->table.size(), 1);
}

TYPED_TEST(TableTest, emptyTableIsEmpty)
{
    EXPECT_TRUE(this->table.empty());
}

TYPED_TEST(TableTest, nonEmptyTableIsNonEmpty)
{
    Polynomial a = std::get<Polynomial>(Polynomial::fromString("12x2-4y"));
    this->table.addPolynomial("Pol1", a);
    EXPECT_FALSE(this->table.empty());
}
