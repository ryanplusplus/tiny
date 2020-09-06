/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_list_iterator.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_list_iterator)
{
  tiny_list_t list;
  tiny_list_node_t node_1, node_2, node_3;

  void setup()
  {
    tiny_list_init(&list);
    tiny_list_push_back(&list, &node_1);
    tiny_list_push_back(&list, &node_2);
    tiny_list_push_back(&list, &node_3);
  }
};

TEST(tiny_list_iterator, should_iterate)
{
  uint8_t i = 0;
  tiny_list_node_t* expected[] = { &node_1, &node_2, &node_3 };

  tiny_list_iterate(&list, tiny_list_node_t, item, {
    CHECK(expected[i++] == item);
  });

  CHECK_EQUAL(element_count(expected), i);
}
