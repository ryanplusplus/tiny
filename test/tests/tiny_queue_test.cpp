/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "tiny_queue.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  large_element_size = 25,
  small_element_size = 1,

  some_small_element = 0x4D,
  another_small_element = 0xC3,

  extra_buffering_size = 5,
  arbitrary_byte = 0xAF,

  enough_space_to_fit_two_large_elements = large_element_size * 2 + sizeof(uint16_t) * 2,
  enough_large_elements_to_fill_up_storage_space = 2,
  enough_small_elements_to_fill_up_storage_space = enough_space_to_fit_two_large_elements / (sizeof(uint16_t) + small_element_size),
};

TEST_GROUP(tiny_queue)
{
  tiny_queue_t self;
  uint8_t buffer[enough_space_to_fit_two_large_elements + extra_buffering_size];
  uint8_t some_large_element_1[large_element_size];
  uint8_t some_large_element_2[large_element_size];
  uint8_t some_large_element_3[large_element_size];

  void setup()
  {
    fill_the_extra_space_in_the_buffer_with_known_data();
    fill_large_elements_with_arbitrary_data();
  }

  void teardown()
  {
    verify_that_the_extra_space_was_not_modified();
  }

  void fill_the_extra_space_in_the_buffer_with_known_data()
  {
    for(uint8_t i = enough_space_to_fit_two_large_elements; i < enough_space_to_fit_two_large_elements + extra_buffering_size; i++) {
      buffer[i] = arbitrary_byte;
    }
  }

  void verify_that_the_extra_space_was_not_modified()
  {
    for(uint8_t i = enough_space_to_fit_two_large_elements; i < enough_space_to_fit_two_large_elements + extra_buffering_size; i++) {
      CHECK_EQUAL(arbitrary_byte, buffer[i]);
    }
  }

  void fill_large_elements_with_arbitrary_data()
  {
    for(uint8_t i = 0; i < large_element_size; i++) {
      memset(some_large_element_1 + i, i, sizeof(uint8_t));
    }

    for(uint8_t i = 0; i < large_element_size; i++) {
      memset(some_large_element_2 + i, i + 5, sizeof(uint8_t));
    }

    for(uint8_t i = 0; i < large_element_size; i++) {
      memset(some_large_element_3 + i, i + 10, sizeof(uint8_t));
    }
  }

  void given_that_the_queue_has_been_initialized()
  {
    tiny_queue_init(
      &self,
      buffer,
      sizeof(buffer));
  }

  void given_that_small_element_has_been_enqueued(uint8_t element)
  {
    bool wasTheOperationSuccessful = tiny_queue_enqueue(&self, (void*)&element, sizeof(element));
    CHECK_TRUE(wasTheOperationSuccessful);
  }

  void small_element_shoulde_be_dequeued(uint8_t element)
  {
    uint8_t dequeued_element_storage = 0;
    uint16_t dequeued_element_size = 0;
    tiny_queue_dequeue(&self, &dequeued_element_storage, &dequeued_element_size);

    CHECK_EQUAL(dequeued_element_size, sizeof(element));
    CHECK_EQUAL(element, dequeued_element_storage);
  }

  bool given_that_large_element_has_been_enqueued(uint8_t * element)
  {
    return tiny_queue_enqueue(&self, (void*)element, large_element_size);
  }

  void this_many_large_elements_are_enqueued(uint8_t element_count)
  {
    for(uint8_t i = 0; i < element_count; i++) {
      given_that_large_element_has_been_enqueued(some_large_element_1);
    }
  }

  void enqueuing_another_large_element_should_fail()
  {
    CHECK_FALSE(given_that_large_element_has_been_enqueued(some_large_element_1));
  }

  void dequeueing_should_yield(uint8_t * element)
  {
    uint8_t dequeued_element_storage[large_element_size];
    memset(dequeued_element_storage, 0, sizeof(dequeued_element_storage));
    uint16_t dequeued_element_size = 0;

    tiny_queue_dequeue(&self, &dequeued_element_storage, &dequeued_element_size);

    CHECK_EQUAL(dequeued_element_size, large_element_size);
    for(uint8_t i = 0; i < large_element_size; i++) {
      CHECK_EQUAL(element[i], dequeued_element_storage[i]);
    }
  }

  void should_be_able_to_discard_an_element()
  {
    tiny_queue_discard(&self);
  }

  void peeking_should_yield(uint8_t * element, size_t element_size, uint16_t element_index)
  {
    uint8_t peekedElementStorage[element_size];
    memset(peekedElementStorage, 0, sizeof(peekedElementStorage));
    uint16_t dequeued_element_size = 0;

    tiny_queue_peek(&self, &peekedElementStorage, &dequeued_element_size, element_index);

    CHECK_EQUAL(dequeued_element_size, element_size);
    for(uint8_t i = 0; i < element_size; i++) {
      CHECK_EQUAL(element[i], peekedElementStorage[i]);
    }
  }

  void peeking_from_the_front_of_the_queue_should_yield(uint8_t * element)
  {
    peeking_should_yield(element, large_element_size, 0);
  }

  void peeking_size_of_the_element_at_index_should_yield(uint16_t index, uint16_t expected)
  {
    uint16_t actual;
    tiny_queue_peek_size(&self, &actual, index);
    CHECK(expected == actual);
  }

  void should_be_able_to_peek_n_bytes_of(uint16_t index, uint16_t partial_size)
  {
    uint8_t full[large_element_size];
    uint8_t partial[large_element_size];
    uint8_t empty[large_element_size + 1];
    uint16_t full_size;

    memset(partial, 0xA5, sizeof(partial));
    memset(empty, 0xA5, sizeof(empty));

    tiny_queue_peek(&self, full, &full_size, index);
    tiny_queue_peek_partial(&self, partial, partial_size, index);

    MEMCMP_EQUAL(full, partial, partial_size);
    MEMCMP_EQUAL(empty + partial_size, partial + partial_size, sizeof(partial) - partial_size);
    CHECK(empty[sizeof(empty) - 1] == 0xA5);
  }

  void enqueue_one_byte_elements_such_that_each_element_is_incremented(uint8_t startingNumber, uint8_t element_count)
  {
    uint8_t element = startingNumber;
    for(uint8_t count = 0; count < element_count; count++) {
      tiny_queue_enqueue(&self, &element, sizeof(element));
      element++;
    }
  }

  void given_that_n_elements_have_been_enqueued(uint16_t element_count)
  {
    enqueue_one_byte_elements_such_that_each_element_is_incremented(0, element_count);
  }

  void the_queue_is_filled_up_with_elements()
  {
    enqueue_one_byte_elements_such_that_each_element_is_incremented(45, enough_small_elements_to_fill_up_storage_space);
  }

  void the_peek_operation_should_be_able_to_peek_every_element_in_the_queue()
  {
    uint8_t element = 45;
    for(uint8_t index = 0; index < enough_small_elements_to_fill_up_storage_space; index++) {
      peeking_should_yield(&element, sizeof(element), index);
      element++;
    }
  }

  void given_that_n_elements_have_been_dequeued(uint8_t elements_to_dequeue)
  {
    uint8_t element = 0;
    uint16_t size = 0;
    for(uint8_t count = 0; count < elements_to_dequeue; count++) {
      tiny_queue_dequeue(&self, &element, &size);
    }
  }

  void given_that_n_elements_have_been_discarded(uint8_t elements_to_dequeue)
  {
    for(uint8_t count = 0; count < elements_to_dequeue; count++) {
      tiny_queue_discard(&self);
    }
  }

  void the_element_count_should_be(uint8_t expected)
  {
    CHECK_EQUAL(expected, tiny_queue_count(&self));
  }
};

TEST(tiny_queue, should_be_able_to_enqueue_and_dequeue_a_small_element)
{
  given_that_the_queue_has_been_initialized();
  given_that_small_element_has_been_enqueued(some_small_element);
  small_element_shoulde_be_dequeued(some_small_element);
}

TEST(tiny_queue, should_be_able_to_enqueue_and_discard_a_small_element)
{
  given_that_the_queue_has_been_initialized();
  given_that_small_element_has_been_enqueued(some_small_element);
  should_be_able_to_discard_an_element();
}

TEST(tiny_queue, should_be_able_to_enqueue_and_dequeue_a_large_element)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  dequeueing_should_yield(some_large_element_1);
}

TEST(tiny_queue, should_be_able_to_enqueue_three_elements_and_then_dequeue_them)
{
  given_that_the_queue_has_been_initialized();
  given_that_small_element_has_been_enqueued(some_small_element);
  given_that_large_element_has_been_enqueued(some_large_element_1);
  given_that_small_element_has_been_enqueued(another_small_element);
  small_element_shoulde_be_dequeued(some_small_element);
  dequeueing_should_yield(some_large_element_1);
  small_element_shoulde_be_dequeued(another_small_element);
}

TEST(tiny_queue, should_be_able_to_discard_some_elements_and_then_dequeue_others)
{
  given_that_the_queue_has_been_initialized();
  given_that_small_element_has_been_enqueued(some_small_element);
  given_that_large_element_has_been_enqueued(some_large_element_1);
  given_that_small_element_has_been_enqueued(another_small_element);
  should_be_able_to_discard_an_element();
  dequeueing_should_yield(some_large_element_1);
  small_element_shoulde_be_dequeued(another_small_element);
}

TEST(tiny_queue, should_be_able_to_enqueue_more_elements_after_dequeuing_and_discarding)
{
  given_that_the_queue_has_been_initialized();
  given_that_small_element_has_been_enqueued(some_small_element);
  given_that_large_element_has_been_enqueued(some_large_element_1);
  should_be_able_to_discard_an_element();
  dequeueing_should_yield(some_large_element_1);

  given_that_small_element_has_been_enqueued(some_small_element);
  small_element_shoulde_be_dequeued(some_small_element);
}

TEST(tiny_queue, should_not_enqueue_elements_that_will_not_fit_in_the_queue)
{
  given_that_the_queue_has_been_initialized();
  this_many_large_elements_are_enqueued(enough_large_elements_to_fill_up_storage_space);
  enqueuing_another_large_element_should_fail();
}

TEST(tiny_queue, should_be_able_to_peek_at_an_element_multiple_times)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  given_that_large_element_has_been_enqueued(some_large_element_2);
  peeking_from_the_front_of_the_queue_should_yield(some_large_element_1);
  peeking_from_the_front_of_the_queue_should_yield(some_large_element_1);
  peeking_from_the_front_of_the_queue_should_yield(some_large_element_1);
}

TEST(tiny_queue, should_be_able_to_peek_at_any_element_that_is_queued)
{
  given_that_the_queue_has_been_initialized();
  the_queue_is_filled_up_with_elements();
  the_peek_operation_should_be_able_to_peek_every_element_in_the_queue();
}

TEST(tiny_queue, should_be_able_to_peek_at_an_element_and_then_dequeue_it)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  peeking_from_the_front_of_the_queue_should_yield(some_large_element_1);
  dequeueing_should_yield(some_large_element_1);
}

TEST(tiny_queue, should_be_able_to_peek_just_part_of_an_element)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  should_be_able_to_peek_n_bytes_of(0, 1);
  should_be_able_to_peek_n_bytes_of(0, 10);
  should_be_able_to_peek_n_bytes_of(0, 15);
  should_be_able_to_peek_n_bytes_of(0, large_element_size);
}

TEST(tiny_queue, should_be_able_to_peek_size_of_an_element_multiple_times)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  given_that_large_element_has_been_enqueued(some_large_element_2);
  peeking_size_of_the_element_at_index_should_yield(0, large_element_size);
  peeking_size_of_the_element_at_index_should_yield(0, large_element_size);
  peeking_size_of_the_element_at_index_should_yield(0, large_element_size);
}

TEST(tiny_queue, should_be_able_to_peek_size_of_any_element_that_is_queued)
{
  given_that_the_queue_has_been_initialized();
  given_that_large_element_has_been_enqueued(some_large_element_1);
  given_that_small_element_has_been_enqueued(21);
  given_that_small_element_has_been_enqueued(5);
  peeking_size_of_the_element_at_index_should_yield(0, large_element_size);
  peeking_size_of_the_element_at_index_should_yield(1, small_element_size);
  peeking_size_of_the_element_at_index_should_yield(2, small_element_size);
}

TEST(tiny_queue, should_know_how_many_elements_are_queued)
{
  given_that_the_queue_has_been_initialized();
  given_that_n_elements_have_been_enqueued(4);
  the_element_count_should_be(4);
}

TEST(tiny_queue, should_know_how_many_elements_are_queued_after_some_have_been_dequeued)
{
  given_that_the_queue_has_been_initialized();
  given_that_n_elements_have_been_enqueued(4);
  given_that_n_elements_have_been_dequeued(2);
  the_element_count_should_be(2);
}

TEST(tiny_queue, should_know_how_many_elements_are_queued_after_some_have_been_discarded)
{
  given_that_the_queue_has_been_initialized();
  given_that_n_elements_have_been_enqueued(4);
  given_that_n_elements_have_been_discarded(2);
  the_element_count_should_be(2);
}
