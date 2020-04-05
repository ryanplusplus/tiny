/*!
 * @file
 * @brief Abstract event.
 */

#ifndef i_tiny_event_h
#define i_tiny_event_h

#include "tiny_event_subscription.h"

struct i_tiny_event_api_t;

typedef struct {
  const struct i_tiny_event_api_t* api;
} i_tiny_event_t;

typedef struct i_tiny_event_api_t {
  /*!
   * Adds a subscriber to the event.
   */
  void (*subscribe)(i_tiny_event_t* self, tiny_event_subscription_t* subscription);

  /*!
   * Removes a subscriber from the event.
   */
  void (*unsubscribe)(i_tiny_event_t* self, tiny_event_subscription_t* subscription);
} i_tiny_event_api_t;

#define tiny_event_subscribe(self, subscription) \
  (self)->api->subscribe((self), (subscription))

#define tiny_event_unsubscribe(self, subscription) \
  (self)->api->unsubscribe((self), (subscription))

#endif
