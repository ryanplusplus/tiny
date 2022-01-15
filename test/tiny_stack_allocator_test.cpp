/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "tiny_stack_allocator.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

static size_t current_size;

TEST_GROUP(tiny_stack_allocator)
{
  enum {
    largest_supported_size = tiny_stack_allocator_largest_supported_size
  };

  static void callback(void* context, void* data)
  {
    CHECK(context == (void*)0x1234);
    memset(data, 0xA5, current_size);
  }
};

TEST(tiny_stack_allocator, should_allocate_aligned_memory_of_the_requested_size)
{
  for(size_t size = 1; size <= largest_supported_size; size++) {
    current_size = size;
    tiny_stack_allocator_allocate_aligned(size, callback, (void*)(0x1234));
  }
}
