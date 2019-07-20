/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_single_subscriber_event.h"
#include "tiny_utils.h"

static void subscribe(i_tiny_event_t* _self, tiny_event_subscription_t* subscription) {
  reinterpret(self, _self, tiny_single_subscriber_event_t*);
  self->subscriber = subscription;
}

static void unsubscribe(i_tiny_event_t* _self, tiny_event_subscription_t* subscription) {
  (void)subscription;
  reinterpret(self, _self, tiny_single_subscriber_event_t*);
  self->subscriber = NULL;
}

static const i_tiny_event_api_t api = {
  subscribe, unsubscribe
};

void tiny_single_subscriber_event_init(tiny_single_subscriber_event_t* self) {
  self->interface.api = &api;
  self->subscriber = NULL;
}

void tiny_single_subscriber_event_publish(tiny_single_subscriber_event_t* self, const void* args) {
  if(self->subscriber) {
    tiny_event_subscription_publish(self->subscriber, args);
  }
}
