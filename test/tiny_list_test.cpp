extern "C" {
#include "tiny_list.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_list) {
  tiny_list_t list;
  tiny_list_node_t node_1, node_2, node_3;

  void setup() {
    mock().strictOrder();

    tiny_list_init(&list);
  }

  static bool for_each_callback(tiny_list_node_t * node, uint16_t index, void* context) {
    return mock()
      .actualCall("for_each_callback")
      .withParameter("node", node)
      .withParameter("index", index)
      .withParameter("context", context)
      .returnBoolValueOrDefault(true);
  }
};

TEST(tiny_list, should_be_empty_after_init) {
  CHECK(0 == tiny_list_count(&list));
}

TEST(tiny_list, count_should_update_as_nodes_are_added) {
  tiny_list_push_front(&list, &node_1);
  CHECK(1 == tiny_list_count(&list));

  tiny_list_push_front(&list, &node_2);
  CHECK(2 == tiny_list_count(&list));

  tiny_list_push_back(&list, &node_3);
  CHECK(3 == tiny_list_count(&list));
}

TEST(tiny_list, should_push_and_pop_nodes_at_front) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_push_and_pop_nodes_at_back) {
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(&node_3 == tiny_list_pop_back(&list));
  CHECK(&node_2 == tiny_list_pop_back(&list));
  CHECK(&node_1 == tiny_list_pop_back(&list));
}

TEST(tiny_list, should_pop_nodes_from_back_in_reverse_order_that_they_were_pushed_front) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  CHECK(&node_1 == tiny_list_pop_back(&list));
  CHECK(&node_2 == tiny_list_pop_back(&list));
  CHECK(&node_3 == tiny_list_pop_back(&list));
}

TEST(tiny_list, should_pop_nodes_from_front_in_reverse_order_that_they_were_pushed_back) {
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(&node_1 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_3 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_front) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_3);

  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_back) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_1);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_2 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_remove_nodes_from_middle) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);
  tiny_list_push_front(&list, &node_3);

  tiny_list_remove(&list, &node_2);

  CHECK(&node_3 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_not_fail_if_removed_node_is_not_in_the_list) {
  tiny_list_push_front(&list, &node_1);
  tiny_list_push_front(&list, &node_2);

  tiny_list_remove(&list, &node_3);

  CHECK(&node_2 == tiny_list_pop_front(&list));
  CHECK(&node_1 == tiny_list_pop_front(&list));
}

TEST(tiny_list, should_call_for_each_callback_for_each_node_in_list) {
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  mock()
    .expectOneCall("for_each_callback")
    .withParameter("node", &node_1)
    .withParameter("index", 0)
    .withParameter("context", (void*)0x1234);
  mock()
    .expectOneCall("for_each_callback")
    .withParameter("node", &node_2)
    .withParameter("index", 1)
    .withParameter("context", (void*)0x1234);
  mock()
    .expectOneCall("for_each_callback")
    .withParameter("node", &node_3)
    .withParameter("index", 2)
    .withParameter("context", (void*)0x1234);

  tiny_list_for_each(&list, for_each_callback, (void*)0x1234);
}

TEST(tiny_list, should_stop_iteration_if_the_for_each_callback_returns_false) {
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  mock()
    .expectOneCall("for_each_callback")
    .withParameter("node", &node_1)
    .withParameter("index", 0)
    .withParameter("context", (void*)0x1234)
    .andReturnValue(false);

  tiny_list_for_each(&list, for_each_callback, (void*)0x1234);
}

TEST(tiny_list, should_indicate_whether_list_contains_a_given_node) {
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
  CHECK(true == tiny_list_contains(&list, &node_3))
}

TEST(tiny_list, should_give_the_index_of_a_specified_node) {
  tiny_list_push_back(&list, &node_1);
  tiny_list_push_back(&list, &node_2);
  tiny_list_push_back(&list, &node_3);

  CHECK(0 == tiny_list_index_of(&list, &node_1));
  CHECK(1 == tiny_list_index_of(&list, &node_2));
  CHECK(2 == tiny_list_index_of(&list, &node_3));
}
