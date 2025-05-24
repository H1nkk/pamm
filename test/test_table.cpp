#include <gtest/gtest.h>
#include "table.h"

TEST(TableTest, default_table_constructor)
{
    Table<std::string, int> table;

    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(TableTest, can_add_with_not_existing_key)
{
    Table<std::string, int> table;

    int a = 21;
    std::vector<std::pair< std::string, int>> pol(6);
    table.add("Pol1", a);
    table.add("Pol2", a);
    table.add("Pol4", a);
    table.add("Pol6", a);
    table.add("Pol5", a);
    table.add("Pol3", a);
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
    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 6);
}

TEST(TableTest, cant_add_with_existing_key)
{
    Table<std::string, int> table;

    table.add("Pol1", 433);
    EXPECT_ANY_THROW(table.add("Pol1", 433));
}

TEST(TableTest, can_find_with_existing_key)
{
    Table<std::string, int> table;

    table.add("Pol1", 1234);
    EXPECT_EQ(table.find("Pol1"), 1234);
}

TEST(TableTest, cant_find_with_not_existing_key)
{
    Table<std::string, int> table;

    EXPECT_EQ(table.find("Pol1"), std::nullopt);
}

TEST(TableTest, can_del_existing_entry)
{
    Table<std::string, int> table;

    table.add("Pol1", 2);
    table.del("Pol1");
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(TableTest, del_of_not_existing_entry_does_nothing)
{
    Table<std::string, int> table;

    table.add("Pol1", 12);
    table.add("Pol2", 34);
    table.del("Pol3");
    EXPECT_FALSE(table.empty());
    EXPECT_EQ(table.size(), 2);
}

TEST(TableTest, can_get_size_when_empty)
{
    Table<std::string, int> table;

    EXPECT_EQ(table.size(), 0);
}

TEST(TableTest, can_get_size_when_non_empty)
{
    Table<std::string, int> table;

    table.add("Poli", 2234);
    EXPECT_EQ(table.size(), 1);
}

TEST(TableTest, empty_table_is_empty)
{
    Table<std::string, int> table;

    table.add("abc", 12);
    table.del("abc");

    EXPECT_TRUE(table.empty());
}

TEST(TableTest, non_empty_table_is_non_empty)
{
    Table<std::string, int> table;

    table.add("Pol1", 43);
    EXPECT_FALSE(table.empty());
}
