/*!
 * @file
 * @brief Event implementation that supports only a single subscriber. This reduces
 * stack usage and execution time when the event is published, but limits the event to
 * a single subscriber.
 */

#ifndef tiny_single_subscriber_event_h
#define tiny_single_subscriber_event_h

#include "i_tiny_event.h"

typedef struct {
  i_tiny_event_t interface;
  tiny_event_subscription_t* subscriber;
} tiny_single_subscriber_event_t;

/*!
 * Initializes the event.
 */
void tiny_single_subscriber_event_init(tiny_single_subscriber_event_t* self);

/*!
 * Publishes the event with the given arguments.
 */
void tiny_single_subscriber_event_publish(tiny_single_subscriber_event_t* self, const void* args);

#endif
