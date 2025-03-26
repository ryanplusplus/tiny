/*!
 * @file
 * @brief
 */

#include "CppUTestExt/MockSupport.h"
#include "double/tiny_buffered_uart_double.hpp"

static void send(i_tiny_buffered_uart_t* _self, const void* buffer, uint16_t buffer_size)
{
  auto self = reinterpret_cast<tiny_buffered_uart_double_t*>(_self);
  self->sending = true;

  if(self->automatic_send_complete) {
    self->send_completed = true;
  }

  mock()
    .actualCall("send")
    .onObject(self)
    .withMemoryBufferParameter("buffer", reinterpret_cast<const uint8_t*>(buffer), buffer_size);
}

static i_tiny_event_t* on_send_complete(i_tiny_buffered_uart_t* _self)
{
  auto self = reinterpret_cast<tiny_buffered_uart_double_t*>(_self);
  return &self->send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_buffered_uart_t* _self)
{
  auto self = reinterpret_cast<tiny_buffered_uart_double_t*>(_self);
  return &self->receive.interface;
}

static void run(i_tiny_buffered_uart_t* _self)
{
  auto self = reinterpret_cast<tiny_buffered_uart_double_t*>(_self);

  if(self->send_completed) {
    self->sending = false;
    self->send_completed = false;
    tiny_event_publish(&self->send_complete, nullptr);
  }

  if(self->receive_pending) {
    self->receive_pending = false;
    tiny_buffered_uart_on_receive_args_t args = { self->receive_buffer, self->receive_buffer_size };
    tiny_event_publish(&self->receive, &args);
  }
}

static const i_tiny_buffered_uart_api_t api = { send, on_send_complete, on_receive, run };

void tiny_buffered_uart_double_init(tiny_buffered_uart_double_t* self)
{
  self->interface.api = &api;
  self->sending = false;
  self->automatic_send_complete = false;

  tiny_event_init(&self->send_complete);
  tiny_event_init(&self->receive);
}

bool tiny_buffered_uart_double_sending(tiny_buffered_uart_double_t* self)
{
  return self->sending;
}

void tiny_buffered_uart_double_trigger_send_complete(tiny_buffered_uart_double_t* self)
{
  self->sending = false;
  self->send_completed = true;
}

void tiny_buffered_uart_double_trigger_receive(tiny_buffered_uart_double_t* self, const void* buffer, uint16_t buffer_size)
{
  self->receive_buffer = buffer;
  self->receive_buffer_size = buffer_size;
  self->receive_pending = true;
}

void tiny_buffered_uart_double_configure_automatic_send_complete(tiny_buffered_uart_double_t* self, bool enabled)
{
  self->automatic_send_complete = enabled;
}
