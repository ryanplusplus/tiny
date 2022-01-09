/*!
 * @file
 * @brief
 */

#ifndef i_tiny_buffered_uart_h
#define i_tiny_buffered_uart_h

#include <stdint.h>
#include "i_tiny_event.h"

typedef struct {
  const void* buffer;
  uint16_t buffer_size;
} tiny_buffered_uart_on_receive_args_t;

struct i_tiny_buffered_uart_api_t;

typedef struct {
  const struct i_tiny_buffered_uart_api_t* api;
} i_tiny_buffered_uart_t;

typedef struct i_tiny_buffered_uart_api_t {
  /*!
   * Sends a buffer. The buffer must remain valid until the send completes.
   */
  void (*send)(i_tiny_buffered_uart_t* self, const void* buffer, uint16_t buffer_size);

  /*!
   * Event raised when a byte is finished being sent. This is raised only
   * during a call to run().
   */
  i_tiny_event_t* (*on_send_complete)(i_tiny_buffered_uart_t* self);

  /*!
   * Event raised when a byte is received. This is raised only
   * during a call to run().
   */
  i_tiny_event_t* (*on_receive)(i_tiny_buffered_uart_t* self);

  /*!
   * Raises receive and send complete events as necessary.
   */
  void (*run)(i_tiny_buffered_uart_t* self);
} i_tiny_buffered_uart_api_t;

#define tiny_buffered_uart_send(self, buffer, buffer_size) \
  (self)->api->send((self), (buffer), (buffer_size))

#define tiny_buffered_uart_on_send_complete(self) \
  (self)->api->on_send_complete((self))

#define tiny_buffered_uart_on_receive(self) \
  (self)->api->on_receive((self))

#define tiny_buffered_uart_run(self) \
  (self)->api->run((self))

#endif
