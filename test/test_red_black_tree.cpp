#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include "red_black_tree.h"

using namespace std;

TEST(RedBlackTreeTest, default_constructor)
{
    RedBlackTree<string, int> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find("a"), std::nullopt);
}

TEST(RedBlackTreeTest, can_insert_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), 12);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
    EXPECT_EQ(tree.find("d"), std::nullopt);
}

TEST(RedBlackTreeTest, can_rewrite_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), 12);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
    tree.insert("ab", -20);
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), -20);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
}

TEST(RedBlackTree, can_erase_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.size(), 1);
    tree.erase("a");
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), std::nullopt);
    EXPECT_EQ(tree.size(), 0);
}

TEST(RedBlackTreeTest, erasing_does_not_corrupt_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), 12);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
    tree.erase("ab");
    EXPECT_TRUE(tree.isValidTree());
    tree.erase("c");
    EXPECT_TRUE(tree.isValidTree());
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), std::nullopt);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), std::nullopt);
}

TEST(RedBlackTreeTest, can_insert_after_erased)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    tree.erase("A");
    tree.erase("a");
    tree.erase("ab");
    tree.erase("c");

    EXPECT_NO_THROW(tree.insert("a", 120));
}

TEST(RedBlackTreeTest, can_convert_to_vector)
{
    RedBlackTree<int, int> tree;

    const int n = 100000;
    vector<int> data(n);
    iota(data.begin(), data.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    g.seed(123);

    shuffle(data.begin(), data.end(), g);
    for (auto v : data) tree.insert(v, v + 1);
    EXPECT_EQ(tree.size(), n);
    EXPECT_TRUE(tree.isValidTree());

    auto res = tree.toVector();
    EXPECT_EQ(res.size(), n);
    for (size_t i = 0; i < n; i++)
    {
        EXPECT_EQ(res[i], std::make_pair(i, i + 1));
    }
}

//TEST(RedBlackTreeTest, tree_stress_test)
//{
//    RedBlackTree<int, int> mTree;
//
//    const int n = 10000000;
//    vector<int> data(n);
//    iota(data.begin(), data.end(), 0);
//    for (auto v : data) mTree.insert(v, v + 1);
//    EXPECT_EQ(mTree.size(), n);
//    EXPECT_TRUE(mTree.isValidTree());
//
//    reverse(data.begin(), data.end());
//    for (auto v : data)
//    {
//        EXPECT_EQ(mTree.find(v), v + 1);
//    }
//
//    std::random_device rd;
//    std::mt19937 g(rd());
//    g.seed(123);
//
//    shuffle(data.begin(), data.end(), g);
//    for (auto v : data)
//    {
//        EXPECT_TRUE(mTree.erase(v));
//    }
//    EXPECT_TRUE(mTree.isValidTree());
//    EXPECT_EQ(mTree.size(), 0);
//}