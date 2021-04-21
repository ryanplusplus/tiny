/*!
 * @file
 * @brief
 */

#include "tiny_event_subscription.h"

extern inline void tiny_event_subscription_init(
  tiny_event_subscription_t* self,
  void* context,
  tiny_event_subscription_callback_t callback);

extern inline void tiny_event_subscription_publish(tiny_event_subscription_t* self, const void* args);
