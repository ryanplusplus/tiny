/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_list.h"
#include "tiny_utils.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

static tiny_list_node_t* node_1_pointer;

TEST_GROUP(tiny_list)
{
  tiny_list_t list;
  tiny_list_node_t node_1, node_2, node_3;

  void setup()
  {
    mock().strictOrder();

    node_1_pointer = &node_1;

    tiny_list_init(&list);
  }
};

TEST(tiny_list, should_be_empty_after_init)
{
  CHECK(0 == tiny_list_count(&list));
}

TEST(tiny_list, count_should_update_as_nodes_are_added)
{
  tiny_list_push_front(&list, &node_1);
  CHECK(1 == tiny_list_count(&list));

  tiny_list_push_front(&list, &node_2);
  CHECK(2 == tiny_list_count(&list));

  tiny_list_push_back(&list, &node_3);
  CHECK(3 == tiny_list_count(&list));
}

TEST(tiny_list, should_push_and_pop_nodes_at_front)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_push_and_pop_nodes_at_back)
{
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(&node_3 == tiny_list_pop_back(&list));
  CHECK(&node_2 == tiny_list_pop_back(&list));
  CHECK(&node_1 == tiny_list_pop_back(&list));
}

TEST(tiny_list, should_pop_nodes_from_back_in_reverse_order_that_they_were_pushed_front)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  CHECK(&node_1 == tiny_list_pop_back(&list));
  CHECK(&node_2 == tiny_list_pop_back(&list));
  CHECK(&node_3 == tiny_list_pop_back(&list));
}

TEST(tiny_list, should_pop_nodes_from_front_in_reverse_order_that_they_were_pushed_back)
{
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(&node_1 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_3 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_front)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_3);

  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_back)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_1);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_middle)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_2);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_not_fail_if_removed_node_is_not_in_the_list)
{
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);

  tiny_list_remove(&list, &node_3);

  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_iterate_through_all_nodes_in_order_with_for_each)
{
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  mock().expectOneCall("for_each").withParameter("node", &node_1);
  mock().expectOneCall("for_each").withParameter("node", &node_2);
  mock().expectOneCall("for_each").withParameter("node", &node_3);

  tiny_list_for_each(&list, tiny_list_node_t, node, {
    mock().actualCall("for_each").withParameter("node", node);
  });
}

TEST(tiny_list, should_allow_the_current_node_to_be_removed_during_iteration)
{
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  mock().expectOneCall("for_each").withParameter("node", &node_1);
  mock().expectOneCall("for_each").withParameter("node", &node_2);
  mock().expectOneCall("for_each").withParameter("node", &node_3);

  tiny_list_for_each(&list, tiny_list_node_t, node, {
    if(node == &node_1) {
      tiny_list_remove(&list, node);
    }
    mock().actualCall("for_each").withParameter("node", node);
  });
}

TEST(tiny_list, should_indicate_whether_list_contains_a_given_node)
{
  CHECK(false == tiny_list_contains(&list, &node_1));
  CHECK(false == tiny_list_contains(&list, &node_2));
  CHECK(false == tiny_list_contains(&list, &node_3));

  tiny_list_push_back(&list, &node_1);
  CHECK(true == tiny_list_contains(&list, &node_1));
  CHECK(false == tiny_list_contains(&list, &node_2));
  CHECK(false == tiny_list_contains(&list, &node_3));

  tiny_list_push_back(&list, &node_2);
  CHECK(true == tiny_list_contains(&list, &node_1));
  CHECK(true == tiny_list_contains(&list, &node_2));
  CHECK(false == tiny_list_contains(&list, &node_3));

  tiny_list_push_back(&list, &node_3);
  CHECK(true == tiny_list_contains(&list, &node_1));
  CHECK(true == tiny_list_contains(&list, &node_2));
  CHECK(true == tiny_list_contains(&list, &node_3));
}

TEST(tiny_list, should_give_the_index_of_a_specified_node)
{
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(0 == tiny_list_index_of(&list, &node_1));
  CHECK(1 == tiny_list_index_of(&list, &node_2));
  CHECK(2 == tiny_list_index_of(&list, &node_3));
}
