/*!
 * @file
 * @brief Synchronously sends messages from publishers to all subscribers.
 */

#ifndef i_tiny_message_bus_h
#define i_tiny_message_bus_h

#include <stdint.h>
#include "i_tiny_event.h"

typedef uint16_t tiny_message_bus_message_t;

typedef struct {
  tiny_message_bus_message_t message;
  const void* data;
} tiny_message_bus_on_receive_args_t;

struct i_tiny_message_bus_api_t;

typedef struct {
  const struct i_tiny_message_bus_api_t* api;
} i_tiny_message_bus_t;

typedef struct i_tiny_message_bus_api_t {
  /*!
   * Sends a message to all subscribers on the bus.
   */
  void (*send)(i_tiny_message_bus_t* self, tiny_message_bus_message_t message, const void* data);

  /*!
   * Returns the event that clients can use to subscribe to received messages.
   */
  i_tiny_event_t* (*on_receive)(i_tiny_message_bus_t* self);
} i_tiny_message_bus_api_t;

#define tiny_message_bus_send(self, message, data) \
  (self)->api->send((self), (message), (data))

#define tiny_message_bus_on_receive(self) \
  (self)->api->on_receive((self))

#endif
