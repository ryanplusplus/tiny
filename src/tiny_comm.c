/*!
 * @file
 * @brief
 */

#include "tiny_comm.h"

static void send(i_tiny_comm_t* self, const void* payload, uint8_t length)
{
}

static bool sending(i_tiny_comm_t* self)
{
}

static i_tiny_event_t* on_receive(i_tiny_comm_t* self)
{
}

static const i_tiny_comm_api_t api = { send, sending, on_receive };

void tiny_comm_init(tiny_comm_t* self, i_tiny_uart_t* uart)
{
  self->interface.api = &api;
  self->uart = uart;
  tiny_event_init(&self->on_receive);
}

void tiny_comm_run(tiny_comm_t* self)
{
  (void)self;
}
