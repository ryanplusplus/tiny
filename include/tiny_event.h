/*!
 * @file
 * @brief Default implementation of an event. Supports an arbitrary number of
 * subscribers.
 */

#ifndef tiny_event_h
#define tiny_event_h

#include "i_tiny_event.h"
#include "tiny_list.h"

typedef struct {
  i_tiny_event_t interface;
  tiny_list_t subscribers;
} tiny_event_t;

/*!
 * Initializes the event.
 */
void tiny_event_init(tiny_event_t* self);

/*!
 * Publishes the event with the given arguments.
 */
void tiny_event_publish(tiny_event_t* self, const void* args);

#endif
