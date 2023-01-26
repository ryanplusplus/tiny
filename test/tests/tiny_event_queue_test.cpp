/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_event_queue.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_event_queue)
{
  tiny_event_queue_t self;

  uint8_t buffer[1024];

  bool run_result;

  void setup()
  {
    mock().strictOrder();
  }

  static void callback()
  {
    mock().actualCall("callback");
  }

  static void u16_callback(const void* data)
  {
    CHECK_EQUAL(0, (uintptr_t)data % sizeof(uint16_t));
    mock().actualCall("u16_callback").withParameter("data", *(const uint16_t*)data);
  }

  static void u32_callback(const void* data)
  {
    CHECK_EQUAL(0, (uintptr_t)data % sizeof(uint32_t));
    mock().actualCall("u32_callback").withParameter("data", *(const uint32_t*)data);
  }

  static void unable_to_enqueue()
  {
    mock().actualCall("unable_to_enqueue");
  }

  void given_that_the_queue_has_been_initialized_with_buffer_size(size_t buffer_size)
  {
    tiny_event_queue_init(&self, buffer, buffer_size, unable_to_enqueue);
  }

  void given_that_a_u32_event_has_been_enqueued(uint32_t data)
  {
    when_a_u32_event_is_enqueued(data);
  }

  void when_a_u32_event_is_enqueued(uint32_t data)
  {
    tiny_event_queue_enqueue_with_data(&self.interface, u32_callback, &data, sizeof(data));
  }

  void given_that_a_u16_event_has_been_enqueued(uint16_t data)
  {
    when_a_u16_event_is_enqueued(data);
  }

  void when_a_u16_event_is_enqueued(uint16_t data)
  {
    tiny_event_queue_enqueue_with_data(&self.interface, u16_callback, &data, sizeof(data));
  }

  void given_that_an_event_has_been_enqueued()
  {
    when_an_event_is_enqueued();
  }

  void when_an_event_is_enqueued()
  {
    tiny_event_queue_enqueue(&self.interface, callback);
  }

  void u32_event_should_be_raised(uint32_t data)
  {
    mock().expectOneCall("u32_callback").withParameter("data", data);
  }

  void u16_event_should_be_raised(uint16_t data)
  {
    mock().expectOneCall("u16_callback").withParameter("data", data);
  }

  void event_should_be_raised()
  {
    mock().expectOneCall("callback");
  }

  void given_the_queue_has_been_run()
  {
    mock().disable();
    when_the_queue_is_run();
    mock().enable();
  }

  void when_the_queue_is_run()
  {
    run_result = tiny_event_queue_run(&self);
  }

  void and_the_run_result_should_be(bool expected)
  {
    CHECK_EQUAL(expected, run_result);
  }

  void should_raise_unable_to_enqueue_error()
  {
    mock().expectOneCall("unable_to_enqueue");
  }

  void nothing_should_happen()
  {
  }
};

TEST(tiny_event_queue, should_do_nothing_when_run_with_no_events_queued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);

  nothing_should_happen();
  when_the_queue_is_run();
  and_the_run_result_should_be(false);
}

TEST(tiny_event_queue, should_raise_the_enqueued_event_when_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_an_event_has_been_enqueued();

  event_should_be_raised();
  when_the_queue_is_run();
  and_the_run_result_should_be(true);
}

TEST(tiny_event_queue, should_raise_the_enqueued_event_with_data_when_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_a_u32_event_has_been_enqueued(0x12345678);

  u32_event_should_be_raised(0x12345678);
  when_the_queue_is_run();
  and_the_run_result_should_be(true);
}

TEST(tiny_event_queue, should_enqueue_and_process_multiple_events)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_an_event_has_been_enqueued();
  given_that_a_u16_event_has_been_enqueued(0xABCD);

  event_should_be_raised();
  when_the_queue_is_run();

  u16_event_should_be_raised(0xABCD);
  when_the_queue_is_run();
}

TEST(tiny_event_queue, should_ensure_that_events_can_be_enqueued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_t) + 1);
  nothing_should_happen();
  when_an_event_is_enqueued();

  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_t));
  should_raise_unable_to_enqueue_error();
  when_an_event_is_enqueued();
}

TEST(tiny_event_queue, should_ensure_that_events_are_fully_remove_from_the_queue_after_being_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_t) + 1);
  given_that_an_event_has_been_enqueued();
  given_the_queue_has_been_run();

  nothing_should_happen();
  when_an_event_is_enqueued();
}

TEST(tiny_event_queue, should_ensure_that_events_with_data_can_be_enqueued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_with_data_t) + sizeof(uint8_t) + sizeof(uint16_t) + 1);
  nothing_should_happen();
  when_a_u16_event_is_enqueued(0x1234);

  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_with_data_t) + sizeof(uint8_t) + sizeof(uint16_t));
  should_raise_unable_to_enqueue_error();
  when_a_u16_event_is_enqueued(0x1234);
}

TEST(tiny_event_queue, should_ensure_that_events_with_data_are_fully_remove_from_the_queue_after_being_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(tiny_event_queue_callback_with_data_t) + sizeof(uint8_t) + sizeof(uint16_t) + 1);
  given_that_a_u16_event_has_been_enqueued(0x1234);
  given_the_queue_has_been_run();

  nothing_should_happen();
  when_a_u16_event_is_enqueued(0x1234);
}
