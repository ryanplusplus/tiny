/*!
 * @file
 * @brief UART test double.
 */

#ifndef tiny_uart_double_hpp
#define tiny_uart_double_hpp

extern "C" {
#include "hal/i_tiny_uart.h"
#include "tiny_event.h"
}

typedef struct {
  i_tiny_uart_t interface;
  tiny_event_t receive;
  tiny_event_t send_complete;
  bool automatic_send_complete;
  bool sending;
} tiny_uart_double_t;

/*!
 * Initializes a UART double.
 */
void tiny_uart_double_init(tiny_uart_double_t* self);

/*!
 * True if a byte was sent without send completing.
 */
bool tiny_uart_double_sending(tiny_uart_double_t* self);

/*!
 * Causes the double to raise a send complete event.
 */
void tiny_uart_double_trigger_send_complete(tiny_uart_double_t* self);

/*!
 * Causes the double to raise a receive event.
 */
void tiny_uart_double_trigger_receive(tiny_uart_double_t* self, uint8_t byte);

/*!
 * When enabled, the double will raise a send complete event when a byte is sent. Defaults to disabled.
 */
void tiny_uart_double_configure_automatic_send_complete(tiny_uart_double_t* self, bool enabled);

/*!
 * When enabled, the double will raise a receive event when a byte is sent. Defaults to disabled.
 */
void tiny_uart_double_enable_echo(tiny_uart_double_t* self);

#endif
