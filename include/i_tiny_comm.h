/*!
 * @file
 * @brief
 */

#ifndef i_tiny_comm_h
#define i_tiny_comm_h

#include <stdint.h>
#include <stdbool.h>
#include "i_tiny_event.h"

typedef struct {
  const void* payload;
  uint8_t length;
} tiny_comm_on_receive_args_t;

struct i_tiny_comm_api_t;

typedef struct
{
  const struct i_tiny_comm_api_t* api;
} i_tiny_comm_t;

typedef struct i_tiny_comm_api_t {
  /*!
   * Sends a message. Only valid when sending() is false.
   */
  void (*send)(i_tiny_comm_t* self, const void* payload, uint8_t length);

  /*!
   * Indicates whether a message is currently being sent.
   */
  bool (*sending)(i_tiny_comm_t* self);

  /*!
   * Event raised when a message is received.
   */
  i_tiny_event_t* (*on_receive)(i_tiny_comm_t* self);
} i_tiny_comm_api_t;

#define tiny_comm_send(self, payload, length) \
  (self)->api->send((self), (payload), (length))

#define tiny_comm_sending(self) \
  (self)->api->sending((self))

#define tiny_comm_on_receive(self) \
  (self)->api->on_receive((self))

#endif
