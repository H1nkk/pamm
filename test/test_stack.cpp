#include <gtest/gtest.h>
#include "stack.h"

TEST(StackTest, can_create_with_default_constructor)
{
    Stack<int> stack;
    EXPECT_EQ(stack.size(), 0);
    EXPECT_EQ(stack.capacity(), 0);
}

TEST(StackTest, can_create_with_positive_length)
{
    Stack<int> stack(10);
    EXPECT_EQ(stack.size(), 0);
    EXPECT_EQ(stack.capacity(), 10);
}

TEST(StackTest, cant_create_too_big)
{
    EXPECT_ANY_THROW(Stack<int> stack(LONG_MAX));
}

TEST(StackTest, can_create_a_copy)
{
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);

    Stack<int> stack2(stack1);
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), 2);

    stack2.pop();
    EXPECT_EQ(stack2.top(), 1);
}

TEST(StackTest, can_create_with_move_constructor)
{
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);

    Stack<int> stack2(std::move(stack1));
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), 2);

    EXPECT_EQ(stack1.size(), 0);
    EXPECT_EQ(stack1.capacity(), 0);
}

TEST(StackTest, can_assign)
{
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);

    Stack<int> stack2;
    stack2 = stack1;
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), 2);

    stack2.pop();
    EXPECT_EQ(stack2.top(), 1);
}

TEST(StackTest, capacity_doesnt_change_on_assign_shorter)
{
    Stack<int> stack1;
    stack1.push(1);

    Stack<int> stack2;
    stack2.push(1);
    stack2.push(2);
    stack2.push(3);
    size_t cap = stack2.capacity();
    stack2 = stack1;
    EXPECT_EQ(stack2.capacity(), cap);
}

TEST(StackTest, can_perform_move_assignment)
{
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);

    Stack<int> stack2;
    stack2 = std::move(stack1);
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), 2);

    EXPECT_EQ(stack1.size(), 0);
    EXPECT_EQ(stack1.capacity(), 0);
}

TEST(StackTest, can_push)
{
    Stack<int> stack;
    stack.push(1);
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 1);
    stack.push(2);
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 2);
}

TEST(StackTest, can_pop)
{
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 1);
    stack.pop();
    EXPECT_TRUE(stack.empty());
}

TEST(StackTest, cant_pop_from_empty)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.pop());
}

TEST(StackTest, cant_get_top_from_empty)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.top());
}

TEST(StackTest, can_reserve_memory)
{
    Stack<int> stack;
    stack.reserve(10);
    EXPECT_EQ(stack.capacity(), 10);
}

TEST(StackTest, doesnt_reserve_less_than_current)
{
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    size_t cap = stack.capacity();
    stack.reserve(2);
    EXPECT_EQ(stack.capacity(), cap);
}

TEST(StackTest, cant_reserve_too_many_elements)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.reserve(LONG_MAX));
}

TEST(StackTest, test_empty)
{
    Stack<int> stack;
    EXPECT_TRUE(stack.empty());
    stack.push(1);
    EXPECT_FALSE(stack.empty());
}