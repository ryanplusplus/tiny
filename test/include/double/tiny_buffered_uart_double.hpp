/*!
 * @file
 * @brief UART test double.
 */

#ifndef tiny_buffered_uart_double_hpp
#define tiny_buffered_uart_double_hpp

extern "C" {
#include "hal/i_tiny_buffered_uart.h"
#include "tiny_event.h"
}

typedef struct {
  i_tiny_buffered_uart_t interface;
  tiny_event_t receive;
  tiny_event_t send_complete;
  bool automatic_send_complete;
  bool sending;
  bool send_completed;
  bool receive_pending;
  const void* receive_buffer;
  uint16_t receive_buffer_size;
} tiny_buffered_uart_double_t;

/*!
 * Initializes a buffered UART double.
 */
void tiny_buffered_uart_double_init(tiny_buffered_uart_double_t* self);

/*!
 * True if a byte was sent without send completing.
 */
bool tiny_buffered_uart_double_sending(tiny_buffered_uart_double_t* self);

/*!
 * Causes the double to raise a send complete event.
 */
void tiny_buffered_uart_double_trigger_send_complete(tiny_buffered_uart_double_t* self);

/*!
 * Causes the double to raise a receive event.
 */
void tiny_buffered_uart_double_trigger_receive(tiny_buffered_uart_double_t* self, const void* buffer, uint16_t buffer_size);

/*!
 * When enabled, the double will raise a send complete event when a buffer is sent. Defaults to disabled.
 */
void tiny_buffered_uart_double_configure_automatic_send_complete(tiny_buffered_uart_double_t* self, bool enabled);

#endif
