/*!
 * @file
 * @brief
 */

#include "double/tiny_uart_double.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny_utils.h"

static void send(i_tiny_uart_t* _self, uint8_t byte)
{
  auto self = reinterpret_cast<tiny_uart_double_t*>(_self);
  self->sending = true;
  mock().actualCall("send").onObject(self).withParameter("byte", byte);
}

static i_tiny_event_t* on_send_complete(i_tiny_uart_t* _self)
{
  auto self = reinterpret_cast<tiny_uart_double_t*>(_self);
  return &self->send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_uart_t* _self)
{
  auto self = reinterpret_cast<tiny_uart_double_t*>(_self);
  return &self->receive.interface;
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

void tiny_uart_double_init(tiny_uart_double_t* self)
{
  self->interface.api = &api;
  self->sending = false;
  tiny_event_init(&self->send_complete);
  tiny_event_init(&self->receive);
}

bool tiny_uart_double_sending(tiny_uart_double_t* self)
{
  return self->sending;
}

void tiny_uart_double_trigger_send_complete(tiny_uart_double_t* self)
{
  self->sending = false;
  tiny_event_publish(&self->send_complete, nullptr);
}

void tiny_uart_double_trigger_receive(tiny_uart_double_t* self, uint8_t byte)
{
  tiny_uart_on_receive_args_t args = { byte };
  tiny_event_publish(&self->receive, &args);
}
