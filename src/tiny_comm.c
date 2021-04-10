/*!
 * @file
 * @brief
 */

#include <string.h>
#include "tiny_comm.h"
#include "tiny_crc16.h"

#define crc_seed 0xFFFF

enum {
  stx = 0x02,
  etx = 0x03,
  dle = 0x10
};

enum {
  send_state_stx,
  send_state_payload,
  send_state_crc_msb,
  send_state_crc_lsb,
  send_state_etx
};

typedef tiny_comm_t self_t;

// May be running in an interrupt context
static void send_complete(void* context, const void* args)
{
  reinterpret(self, context, self_t*);
  (void)args;

  // make sure send is in progress

  if(self->send_state == send_state_stx) {
    self->send_state = send_state_payload;
  }

  // handle escapes
  if(self->send_state == send_state_payload) {
    if(self->send_buffer_offset < self->send_buffer_count) {
      tiny_uart_send(self->uart, self->send_buffer[self->send_buffer_offset++]);
      return;
    }
    else {
      self->send_state = send_state_crc_msb;
      tiny_uart_send(self->uart, self->send_crc >> 8);
      return;
    }
  }

  // handle escapes
  if(self->send_state == send_state_crc_msb) {
    self->send_state = send_state_crc_lsb;
    tiny_uart_send(self->uart, (uint8_t)self->send_crc);
    return;
  }

  // handle escapes
  if(self->send_state == send_state_crc_lsb) {
    self->send_state = send_state_etx;
    tiny_uart_send(self->uart, etx);
    return;
  }

  if(self->send_state == send_state_etx) {
    self->send_in_progress = false;
  }
}

static void send(i_tiny_comm_t* _self, const void* payload, uint8_t length)
{
  reinterpret(self, _self, self_t*);

  if(length > self->send_buffer_size) {
    return;
  }

  // Send must be stopped while preparing to send so that send complete
  // ISRs don't wreak havok while we're in an inconsistent state
  self->send_in_progress = false;
  {
    memcpy(self->send_buffer, payload, length);
    self->send_buffer_count = length;
    self->send_buffer_offset = 0;
    self->send_state = send_state_stx;
    self->send_crc = tiny_crc16_block(crc_seed, payload, length);
  }
  self->send_in_progress = true;

  tiny_uart_send(self->uart, stx);
}

static bool sending(i_tiny_comm_t* _self)
{
  reinterpret(self, _self, self_t*);
  return self->send_in_progress;
}

static i_tiny_event_t* on_receive(i_tiny_comm_t* self)
{
  (void)self;
  return NULL;
}

static const i_tiny_comm_api_t api = { send, sending, on_receive };

void tiny_comm_init(
  tiny_comm_t* self,
  i_tiny_uart_t* uart,
  uint8_t* send_buffer,
  uint8_t send_buffer_size,
  uint8_t* receive_buffer,
  uint8_t receive_buffer_size)
{
  self->interface.api = &api;
  self->uart = uart;
  self->send_buffer = send_buffer;
  self->send_buffer_size = send_buffer_size;
  self->receive_buffer = receive_buffer;
  self->receive_buffer_size = receive_buffer_size;

  tiny_event_init(&self->on_receive);

  tiny_event_subscription_init(&self->send_complete, self, send_complete);
  tiny_event_subscribe(tiny_uart_on_send_complete(uart), &self->send_complete);
}

void tiny_comm_run(tiny_comm_t* self)
{
  (void)self;
}
