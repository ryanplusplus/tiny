/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_event.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_event) {
  tiny_event_t self;

  tiny_event_subscription_t subscription_1;
  tiny_event_subscription_t subscription_2;

  void setup() {
    tiny_event_init(&self);
    tiny_event_subscription_init(&subscription_1, NULL, subscriber_1);
    tiny_event_subscription_init(&subscription_2, NULL, subscriber_2);
  }

  static void subscriber_1(void* context, const void* args) {
    (void)context;
    mock().actualCall("subscriber_1").withParameter("args", args);
  }

  static void subscriber_2(void* context, const void* args) {
    (void)context;
    mock().actualCall("subscriber_2").withParameter("args", args);
  }
};

TEST(tiny_event, should_do_nothing_when_published_with_no_subscribers) {
  tiny_event_publish(&self, (const void*)0x1234);
}

TEST(tiny_event, should_publish_to_all_subscribers) {
  tiny_event_subscribe(&self.interface, &subscription_1);
  tiny_event_subscribe(&self.interface, &subscription_2);

  mock().expectOneCall("subscriber_1").withParameter("args", (const void*)0x1234);
  mock().expectOneCall("subscriber_2").withParameter("args", (const void*)0x1234);
  tiny_event_publish(&self, (const void*)0x1234);
}

TEST(tiny_event, should_not_publish_to_subscribers_that_have_unsubscribed) {
  tiny_event_subscribe(&self.interface, &subscription_1);
  tiny_event_subscribe(&self.interface, &subscription_2);
  tiny_event_unsubscribe(&self.interface, &subscription_1);

  mock().expectOneCall("subscriber_2").withParameter("args", (const void*)0x5678);
  tiny_event_publish(&self, (const void*)0x5678);
}

TEST(tiny_event, should_allow_subscribers_to_resubscribe) {
  tiny_event_subscribe(&self.interface, &subscription_1);
  tiny_event_subscribe(&self.interface, &subscription_2);
  tiny_event_subscribe(&self.interface, &subscription_1);

  mock().expectOneCall("subscriber_1").withParameter("args", (const void*)0x5678);
  mock().expectOneCall("subscriber_2").withParameter("args", (const void*)0x5678);
  tiny_event_publish(&self, (const void*)0x5678);
}
