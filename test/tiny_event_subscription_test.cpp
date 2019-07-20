/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_event_subscription.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_event_subscription) {
  tiny_event_subscription_t self;
  uint8_t context;

  static void callback(void* context, const void* args) {
    mock()
      .actualCall("callback")
      .withParameter("context", context)
      .withParameter("args", args);
  }

  void setup() {
    tiny_event_subscription_init(&self, &context, callback);
  }
};

TEST(tiny_event_subscription, should_invoke_callback_when_published) {
  mock()
    .expectOneCall("callback")
    .withParameter("context", &context)
    .withParameter("args", (const void*)0x1234);

  tiny_event_subscription_publish(&self, (const void*)0x1234);
}
