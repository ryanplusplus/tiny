/*!
 * @file
 * @brief
 */

#include "tiny_event_subscription.h"

void tiny_event_subscription_init(
  tiny_event_subscription_t* self,
  void* context,
  tiny_event_subscription_callback_t callback) {
  self->callback = callback;
  self->context = context;
}

void tiny_event_subscription_publish(tiny_event_subscription_t* self, const void* args) {
  self->callback(self->context, args);
}
