/*!
 * @file
 * @brief
 */

#ifndef i_tiny_uart_h
#define i_tiny_uart_h

#include <stdint.h>
#include "i_tiny_event.h"

typedef struct {
  uint8_t byte;
} tiny_uart_on_receive_args_t;

struct i_tiny_uart_api_t;

typedef struct
{
  const struct i_tiny_uart_api_t* api;
} i_tiny_uart_t;

typedef struct i_tiny_uart_api_t {
  /*!
   * Sends a byte.
   */
  void (*send)(i_tiny_uart_t* self, uint8_t byte);

  /*!
   * Event raised when a byte is finished being sent.
   */
  i_tiny_event_t* (*on_send_complete)(i_tiny_uart_t* self);

  /*!
   * Event raised when a byte is received.
   */
  i_tiny_event_t* (*on_receive)(i_tiny_uart_t* self);
} i_tiny_uart_api_t;

#define tiny_uart_send(self, byte) \
  (self)->api->send((self), (byte))

#define tiny_uart_on_send_complete(self) \
  (self)->api->on_send_complete((self))

#define tiny_uart_on_receive(self) \
  (self)->api->on_receive((self))

#endif
