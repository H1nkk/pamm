#include "linked_list.h"
#include <exception>
#include <gtest/gtest.h>

TEST(LinkedListTest, can_create_with_default_constructor)
{
    LinkedList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedListTest, can_create_with_initializer_list)
{
    LinkedList<int> list = { 1, 2, 3 };
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, can_create_with_copy_constructor)
{
    LinkedList<int> original = { 1, 2, 3 };
    LinkedList<int> copy(original);

    EXPECT_EQ(copy.size(), original.size());
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end() && it2 != copy.end())
    {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }
}

TEST(LinkedListTest, can_create_with_move_constructor)
{
    LinkedList<int> original = { 1, 2, 3 };
    LinkedList<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);

    EXPECT_TRUE(original.empty());
}

TEST(LinkedListTest, can_assign_lists)
{
    LinkedList<int> original = { 1, 2, 3 };
    LinkedList<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end() && it2 != copy.end())
    {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }
}

TEST(LinkedListTest, can_perform_move_assignment)
{
    LinkedList<int> original = { 1, 2, 3 };
    LinkedList<int> moved;
    moved = std::move(original);

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);

    EXPECT_TRUE(original.empty());
}

TEST(LinkedListTest, self_assignment_does_nothing)
{
    LinkedList<int> list = { 1, 2, 3 };
    list = list;

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, self_move_assignment_does_nothing)
{
    LinkedList<int> list = { 1, 2, 3 };
    list = std::move(list);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, can_push_front)
{
    LinkedList<int> list;
    list.pushFront(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 10);

    list.pushFront(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 20);
}

TEST(LinkedListTest, can_push_back)
{
    LinkedList<int> list;
    list.pushBack(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 10);

    list.pushBack(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 20);
}

TEST(LinkedListTest, can_pop_front)
{
    LinkedList<int> list = { 1, 2, 3 };
    list.popFront();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.popFront();
    list.popFront();
    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.popFront(), std::out_of_range);
}

TEST(LinkedListTest, can_pop_back)
{
    LinkedList<int> list = { 1, 2, 3 };
    list.popBack();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.popBack();
    list.popBack();
    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.popBack(), std::out_of_range);
}

TEST(LinkedListTest, can_get_front)
{
    LinkedList<int> list = { 10, 20 };
    EXPECT_EQ(list.front(), 10);

    list.popFront();
    EXPECT_EQ(list.front(), 20);

    list.popFront();
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(LinkedListTest, can_get_back)
{
    LinkedList<int> list = { 10, 20 };
    EXPECT_EQ(list.back(), 20);

    list.popBack();
    EXPECT_EQ(list.back(), 10);

    list.popBack();
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST(LinkedListTest, can_insert_elements)
{
    LinkedList<int> list = { 1, 3 };
    auto it = list.begin();
    list.insertAfter(it, 2);
    EXPECT_EQ(list.size(), 3);

    it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
}

TEST(LinkedListTest, can_erase_elements)
{
    LinkedList<int> list = { 1, 2, 3 };
    auto it = list.begin();
    list.eraseAfter(it);
    EXPECT_EQ(list.size(), 2);

    it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 3);
}

TEST(LinkedListTest, can_use_iterators)
{
    LinkedList<int> list = { 1, 2, 3 };
    auto it = list.begin();
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);

    ++it;
    EXPECT_THROW(*it, std::out_of_range);

    it = list.before_end();
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_THROW(*it, std::out_of_range);
}

TEST(LinkedListTest, can_erase_first)
{
    LinkedList<int> list = { 1, 2, 3 };
    auto it = list.before_begin();
    EXPECT_EQ(list.front(), 1);
    list.eraseAfter(it);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2);
}

TEST(LinkedListTest, check_iterator_errors)
{
    LinkedList<int> list;
    auto it = list.begin();
    EXPECT_THROW(++it, std::out_of_range);
    EXPECT_THROW(*it, std::out_of_range);
}

TEST(LinkedListTest, can_clear_list)
{
    LinkedList<int> list = { 1, 2, 3 };
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.front(), std::out_of_range);
    EXPECT_THROW(list.back(), std::out_of_range);
}