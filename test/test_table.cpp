#include <gtest/gtest.h>
#include "table.h"

template <class Table>
class TableTest : public ::testing::Test
{
protected:
    Table table;
};

using TableTypes = ::testing::Types<LinearArrTable, LinearListTable, OrderedTable, TreeTable, OpenAddressHashTable, SeparateChainingHashTable>;
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
    //EXPECT_EQ(this->table.getPolynomials(), pol);
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


TEST(Aggregator, defaultAggregatorConstructor)
{
    Aggregator a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0);
}

TEST(Aggregator, canSelectCorrectTable)
{
    Aggregator aggr;
    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };

    for (int i = 0; i < 6; i++)
        EXPECT_NO_THROW(aggr.selectTable(tableNames[i]));
}

TEST(Aggregator, cannotSelectIncorrectTable)
{
    Aggregator a;
    EXPECT_ANY_THROW(a.selectTable("fooo"));
}

TEST(Aggregator, canFindExsistingPolynomialUsingAggregator)
{
    Aggregator* pAggregator = new Aggregator();

    std::vector<Polynomial> pols;
    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    int polyCount = 50;
    for (int i = 1; i < polyCount + 1; i++)
    {
        Polynomial a;
        std::string st;
        switch (i % 4)
        {
        case 0:
            st += "w";
            break;
        case 1:
            st += "x";
            break;
        case 2:
            st += "y";
            break;
        case 3:
            st += "z";
            break;
        }
        st += std::to_string(i);
        a = std::get<Polynomial>(Polynomial::fromString(st));
        pols.push_back(a);
    }

    for (int i = 0; i < polyCount; i++)
    {
        Polynomial toDel = pols[i];
        pAggregator->addPolynomial(std::to_string(i), pols[i]);
    }

    for (int i = 0; i < polyCount; i++)
    {
        pAggregator->selectTable(tableNames[i % 6]);

        EXPECT_EQ((pAggregator->findPolynomial(std::to_string(i))).value(), pols[i]);
    }
}

TEST(Aggregator, cannotFindUnexsistingPolynomialUsingAggregator)
{
    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    Aggregator aggr;

    aggr.addPolynomial("fir", std::get<Polynomial>(Polynomial::fromString("x")));
    aggr.addPolynomial("sec", std::get<Polynomial>(Polynomial::fromString("w")));
    aggr.addPolynomial("trd", std::get<Polynomial>(Polynomial::fromString("z")));
    aggr.addPolynomial("frt", std::get<Polynomial>(Polynomial::fromString("y")));

    for (int i = 0; i < 6; i++)
    {
        aggr.selectTable(tableNames[i]);
        EXPECT_EQ(aggr.findPolynomial("fft"), std::nullopt);
    }
}

TEST(Aggregator, canAddUnpresentPolynomialUsingAggregator)
{
    Aggregator aggr;

    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    int polyCount = 100; // TODO ������� 100
    for (int i = 1; i < polyCount + 1; i++)
    {
        Polynomial a;
        std::string st;
        switch (i % 4)
        {
        case 0:
            st += "w";
            break;
        case 1:
            st += "x";
            break;
        case 2:
            st += "y";
            break;
        case 3:
            st += "z";
            break;
        }
        st += std::to_string(i);
        a = std::get<Polynomial>(Polynomial::fromString(st));
        aggr.addPolynomial(std::to_string(i - 1), a);
    }

    int sz = aggr.size();
    aggr.addPolynomial("newpol", std::get<Polynomial>(Polynomial::fromString("xyzw2")));

    EXPECT_EQ(aggr.size(), sz + 1);
}

TEST(Aggregator, cannotAddPresentPolynomialUsingAggregator)
{
    Aggregator aggr;

    aggr.addPolynomial("existing", std::get<Polynomial>(Polynomial::fromString("x3y5z4w2")));

    EXPECT_ANY_THROW(aggr.addPolynomial("existing", std::get<Polynomial>(Polynomial::fromString("x35y53z42w21"))));
}

TEST(Aggregator, canDelPresentPolynomialUsingAggregator)
{
    Aggregator aggr;

    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    std::vector<std::string> polynomials = { "x3y5z4w2", "xy", "zy", "wyz2", "xy5", "-x4y5", "45+ xy", "78", "234x + 6y", "53xyz", "wx", "w" };

    for (int i = 0; i < polynomials.size(); i++)
        aggr.addPolynomial(std::to_string(i), std::get<Polynomial>(Polynomial::fromString(polynomials[i])));


    aggr.addPolynomial("existing", std::get<Polynomial>(Polynomial::fromString("x3y5z4w2")));
    aggr.addPolynomial("existing1", std::get<Polynomial>(Polynomial::fromString("x34y5z4w2")));
    aggr.addPolynomial("existing2", std::get<Polynomial>(Polynomial::fromString("x")));

    int sz = aggr.size();

    for (int i = 3; i < 9; i++)
    {
        aggr.selectTable(tableNames[i % 6]);
        aggr.delPolynomial(std::to_string(i));
    }

    for (int i = 0; i < 6; i++)
    {
        aggr.selectTable(tableNames[i]);
        int newSz = aggr.size();
        EXPECT_EQ(newSz, sz - 6);
    }
}

TEST(Aggregator, delOfUnpresentPolynomialDoesNothingUsingAggregator)
{
    Aggregator aggr;
    OpenAddressHashTable tab; // TODO debug, to be deleted

    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    std::vector<std::string> polynomials = { "x3y5z4w2", "xy", "zy", "wyz2", "xy5", "-x4y5", "45+ xy", "78", "234x + 6y", "53xyz", "wx", "w" };

    for (int i = 0; i < polynomials.size(); i++)
    {
        aggr.addPolynomial(std::to_string(i), std::get<Polynomial>(Polynomial::fromString(polynomials[i])));
        tab.addPolynomial(std::to_string(i), std::get<Polynomial>(Polynomial::fromString(polynomials[i])));// TODO debug, to be deleted
    }

    aggr.addPolynomial("existing", std::get<Polynomial>(Polynomial::fromString("x3y5z4w2")));
    aggr.addPolynomial("existing1", std::get<Polynomial>(Polynomial::fromString("x34y5z4w2")));
    aggr.addPolynomial("existing2", std::get<Polynomial>(Polynomial::fromString("x")));

    int sz = aggr.size();

    for (int i = 453; i < 459; i++)
    {
        aggr.selectTable(tableNames[i % 6]);
        tab.delPolynomial(std::to_string(i)); // TODO debug, to be deleted BUG IS HERE
        aggr.delPolynomial(std::to_string(i));
    }

    EXPECT_EQ(aggr.size(), sz);
}

TEST(Aggregator, canGetSizeWhenEmptyUsingAggregator)
{
    Aggregator aggr;

    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    std::vector<std::string> polynomials = { "x3y5z4w2", "xy", "zy", "wyz2", "xy5", "-x4y5", "45+ xy", "78", "234x + 6y", "53xyz", "wx", "w" };

    for (int i = 0; i < polynomials.size(); i++)
        aggr.addPolynomial(std::to_string(i), std::get<Polynomial>(Polynomial::fromString(polynomials[i])));

    for (int i = 0; i < polynomials.size(); i++)
    {
        aggr.selectTable(tableNames[i % 6]);
        aggr.delPolynomial(std::to_string(i));
    }

    for (int i = 0; i < 6; i++)
    {
        aggr.selectTable(tableNames[i]);
        EXPECT_EQ(aggr.size(), 0);
    }
}

TEST(Aggregator, canGetSizeWhenNonEmptyUsingAggregator)
{
    Aggregator aggr;

    std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
    std::vector<std::string> polynomials = { "x3y5z4w2", "xy", "zy", "wyz2", "xy5", "-x4y5", "45+ xy", "78", "234x + 6y", "53xyz", "wx", "w" };

    int resSize = 6;

    for (int i = 0; i < polynomials.size(); i++)
        aggr.addPolynomial(std::to_string(i), std::get<Polynomial>(Polynomial::fromString(polynomials[i])));

    for (int i = 0; i < polynomials.size() - resSize; i++)
    {
        aggr.selectTable(tableNames[i % 6]);
        aggr.delPolynomial(std::to_string(i));
    }

    for (int i = 0; i < 6; i++)
    {
        aggr.selectTable(tableNames[i]);
        EXPECT_EQ(aggr.size(), resSize);
    }
}
