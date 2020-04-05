/*!
 * @file
 * @brief Default implementation of a message bus.
 */

#ifndef tiny_message_bus_h
#define tiny_message_bus_h

#include "i_tiny_message_bus.h"
#include "tiny_event.h"

typedef struct {
  i_tiny_message_bus_t interface;
  tiny_event_t on_receive;
} tiny_message_bus_t;

/*!
 * Initializes the message bus.
 */
void tiny_message_bus_init(tiny_message_bus_t* self);

#endif
