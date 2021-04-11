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
  send_state_send_crc_msb,
  send_state_send_crc_lsb,
  send_state_send_payload,
  send_state_sent_etx
};

typedef tiny_comm_t self_t;

static void send_control_character(self_t* self, uint8_t byte)
{
  tiny_uart_send(self->uart, byte);
}

static bool control_character(uint8_t byte)
{
  return byte == stx || byte == etx || byte == dle;
}

//! Returns true if the character was sent or false if it was escaped
static bool send_byte(self_t* self, uint8_t byte)
{
  if(!self->send_escaped && control_character(byte)) {
    self->send_escaped = true;
    tiny_uart_send(self->uart, dle);
  }
  else {
    self->send_escaped = false;
    tiny_uart_send(self->uart, byte);
  }

  return !self->send_escaped;
}

// May be running in an interrupt context
static void send_complete(void* context, const void* args)
{
  reinterpret(self, context, self_t*);
  (void)args;

  // make sure send is in progress

  switch(self->send_state) {
    case send_state_send_crc_msb:
      if(send_byte(self, self->send_crc >> 8)) {
        self->send_state = send_state_send_crc_lsb;
      }
      break;

    case send_state_send_crc_lsb:
      if(send_byte(self, (uint8_t)self->send_crc)) {
        self->send_state = send_state_send_payload;
      }
      break;

    case send_state_send_payload:
      if(self->send_buffer_offset < self->send_buffer_count) {
        if(send_byte(self, self->send_buffer[self->send_buffer_offset])) {
          self->send_buffer_offset++;
        }
      }
      else {
        self->send_state = send_state_sent_etx;
        send_control_character(self, etx);
      }
      break;

    case send_state_sent_etx:
      self->send_in_progress = false;
      break;
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
    self->send_state = send_state_send_crc_msb;
    self->send_crc = tiny_crc16_block(crc_seed, payload, length);
  }
  self->send_in_progress = true;

  send_control_character(self, stx);
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
