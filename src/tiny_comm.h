/*!
 * @file
 * @brief
 */

#ifndef tiny_comm_h
#define tiny_comm_h

#include "i_tiny_comm.h"
#include "hal/i_tiny_uart.h"
#include "tiny_event.h"

typedef struct {
  i_tiny_comm_t interface;
  i_tiny_uart_t* uart;
  tiny_event_t on_receive;
} tiny_comm_t;

void tiny_comm_init(tiny_comm_t* self, i_tiny_uart_t* uart);
void tiny_comm_run(tiny_comm_t* self);

#endif
