/*!
 * @file
 * @brief
 */

#ifndef tiny_uart_double_h
#define tiny_uart_double_h

extern "C" {
#include "hal/i_tiny_uart.h"
#include "tiny_event.h"
}

typedef struct {
  i_tiny_uart_t interface;
  tiny_event_t receive;
  tiny_event_t send_complete;
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

#endif
