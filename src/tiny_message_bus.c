/*!
 * @file
 * @brief
 */

#include "tiny_message_bus.h"
#include "tiny_utils.h"

static void send(i_tiny_message_bus_t* _self, tiny_message_bus_message_t message, const void* data) {
  reinterpret(self, _self, tiny_message_bus_t*);
  tiny_message_bus_on_receive_args_t args = { message, data };
  tiny_event_publish(&self->on_receive, &args);
}

static i_tiny_event_t* on_receive(i_tiny_message_bus_t* _self) {
  reinterpret(self, _self, tiny_message_bus_t*);
  return &self->on_receive.interface;
}

static const i_tiny_message_bus_api_t api = { send, on_receive };

void tiny_message_bus_init(tiny_message_bus_t* self) {
  self->interface.api = &api;
  tiny_event_init(&self->on_receive);
}
