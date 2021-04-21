/*!
 * @file
 * @brief Manages the combination of an event callback and context for an event
 * subscriber.
 */

#ifndef tiny_event_subscription_h
#define tiny_event_subscription_h

#include "tiny_list.h"

typedef void (*tiny_event_subscription_callback_t)(void* context, const void* args);

typedef struct {
  tiny_list_node_t node;
  void* context;
  tiny_event_subscription_callback_t callback;
} tiny_event_subscription_t;

/*!
 * Initializes an event subscription.
 */
inline void tiny_event_subscription_init(
  tiny_event_subscription_t* self,
  void* context,
  tiny_event_subscription_callback_t callback)
{
  self->callback = callback;
  self->context = context;
}

/*!
 * Publishes to the event subscriber.
 */
inline void tiny_event_subscription_publish(
  tiny_event_subscription_t* self,
  const void* args)
{
  self->callback(self->context, args);
}

#endif
