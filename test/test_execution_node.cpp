#include <gtest/gtest.h>
#include "execution_node.h"

TEST(BlockNodeTest, runs_all_child_nodes_in_sequence)
{
    ADD_FAILURE();
}

TEST(BlockNodeTest, does_nothing_on_empty_children_list)
{
    ADD_FAILURE();
}

TEST(WriteNodeTest, prints_all_its_arguments_to_stdout)
{
    ADD_FAILURE();
}

TEST(ReadNodeTest, can_read_int_from_stdin_to_variable)
{
    ADD_FAILURE();
}

TEST(ReadNodeTest, can_read_float_from_stdin_to_variable)
{
    ADD_FAILURE();
}

TEST(AssignNodeTest, can_assign_calculated_value_to_variable)
{
    ADD_FAILURE();
}

TEST(IfNodeTest, execute_first_child_then_condition_is_true)
{
    ADD_FAILURE();
}

TEST(IfNodeTest, do_nothing_then_condition_is_false_and_have_only_one_child)
{
    ADD_FAILURE();
}

TEST(IfNodeTest, execute_second_child_then_condition_is_false_and_have_two_children)
{
    ADD_FAILURE();
}


