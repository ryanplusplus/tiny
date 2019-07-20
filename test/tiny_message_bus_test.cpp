/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_message_bus.h"
#include "tiny_utils.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_message_bus) {
  tiny_message_bus_t self;

  tiny_event_subscription_t subscription_1;
  tiny_event_subscription_t subscription_2;

  void setup() {
    tiny_message_bus_init(&self);
    tiny_event_subscription_init(&subscription_1, NULL, subscriber_1);
    tiny_event_subscription_init(&subscription_2, NULL, subscriber_2);
  }

  static void subscriber_1(void* context, const void* _args) {
    (void)context;
    reinterpret(args, _args, const tiny_message_bus_on_receive_args_t*);
    mock()
      .actualCall("subscriber_1")
      .withParameter("message", args->message)
      .withParameter("data", args->data);
  }

  static void subscriber_2(void* context, const void* _args) {
    (void)context;
    reinterpret(args, _args, const tiny_message_bus_on_receive_args_t*);
    mock()
      .actualCall("subscriber_2")
      .withParameter("message", args->message)
      .withParameter("data", args->data);
  }
};

TEST(tiny_message_bus, should_send_messages_to_all_subscribers) {
  tiny_event_subscribe(
    tiny_message_bus_on_receive(&self.interface),
    &subscription_1);
  tiny_event_subscribe(
    tiny_message_bus_on_receive(&self.interface),
    &subscription_2);

  mock()
    .expectOneCall("subscriber_1")
    .withParameter("message", 123)
    .withParameter("data", (const void*)0x1234);
  mock()
    .expectOneCall("subscriber_2")
    .withParameter("message", 123)
    .withParameter("data", (const void*)0x1234);

  tiny_message_bus_send(&self.interface, 123, (const void*)0x1234);
}
