/*!
 * @file
 * @brief
 */

#include "tiny_event.h"
#include "tiny_utils.h"

static void subscribe(i_tiny_event_t* _self, tiny_event_subscription_t* subscription) {
  reinterpret(self, _self, tiny_event_t*);
  tiny_list_remove(&self->subscribers, &subscription->node);
  tiny_list_push_back(&self->subscribers, &subscription->node);
}

static void unsubscribe(i_tiny_event_t* _self, tiny_event_subscription_t* subscription) {
  reinterpret(self, _self, tiny_event_t*);
  tiny_list_remove(&self->subscribers, &subscription->node);
}

static const i_tiny_event_api_t api = {
  subscribe, unsubscribe
};

void tiny_event_init(tiny_event_t* self) {
  self->interface.api = &api;
  tiny_list_init(&self->subscribers);
}

typedef struct {
  const void* args;
  tiny_event_t* self;
} publish_context_t;

static bool publish(tiny_list_node_t* node, uint16_t index, void* _context) {
  (void)index;
  reinterpret(context, _context, publish_context_t*);
  reinterpret(subscription, node, tiny_event_subscription_t*);

  tiny_event_subscription_publish(subscription, context->args);

  return true;
}

void tiny_event_publish(tiny_event_t* self, const void* args) {
  publish_context_t context = { args, self };
  tiny_list_for_each(&self->subscribers, publish, &context);
}
