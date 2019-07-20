/*!
 * @file
 * @brief
 */

extern "C"
{
#include "tiny_single_subscriber_event.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_single_subscriber_event) {
  tiny_single_subscriber_event_t self;

  tiny_event_subscription_t subscription;

  void setup() {
    tiny_single_subscriber_event_init(&self);
    tiny_event_subscription_init(&subscription, NULL, subscriber);
  }

  static void subscriber(void* context, const void* args) {
    (void)context;
    mock().actualCall("subscriber").withParameter("args", args);
  }
};

TEST(tiny_single_subscriber_event, should_do_nothing_when_published_with_no_subscribers) {
  tiny_single_subscriber_event_publish(&self, (const void*)0x1234);
}

TEST(tiny_single_subscriber_event, should_publish_to_all_subscribers) {
  tiny_event_subscribe(&self.interface, &subscription);

  mock().expectOneCall("subscriber").withParameter("args", (const void*)0x1234);
  tiny_single_subscriber_event_publish(&self, (const void*)0x1234);
}

TEST(tiny_single_subscriber_event, should_not_publish_to_subscribers_that_have_unsubscribed) {
  tiny_event_subscribe(&self.interface, &subscription);
  tiny_event_unsubscribe(&self.interface, &subscription);

  tiny_single_subscriber_event_publish(&self, (const void*)0x5678);
}
