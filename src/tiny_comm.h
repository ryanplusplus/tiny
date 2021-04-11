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

  tiny_event_subscription_t send_complete;
  tiny_event_subscription_t byte_received;

  uint8_t* send_buffer;
  uint8_t* receive_buffer;
  uint8_t send_buffer_size;
  uint8_t receive_buffer_size;

  uint16_t send_crc;

  uint8_t send_buffer_count;
  uint8_t send_buffer_offset;
  uint8_t send_state;

  bool send_escaped;

  uint16_t receive_crc;
  uint16_t receive_calculated_crc;
  uint8_t receive_count;
  uint8_t receive_state;
  bool receive_escaped;

  // Makes sure we don't handle send complete
  // while we're setting up another send)
  volatile bool send_in_progress;

  // Makes sure we don't handle received bytes
  // while waiting for packet to be processed
  volatile bool received_packet_ready;
} tiny_comm_t;

void tiny_comm_init(
  tiny_comm_t* self,
  i_tiny_uart_t* uart,
  uint8_t* send_buffer,
  uint8_t send_buffer_size,
  uint8_t* receive_buffer,
  uint8_t receive_buffer_size);

void tiny_comm_run(tiny_comm_t* self);

#endif
