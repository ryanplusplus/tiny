/*!
 * @file
 * @brief Event queue that stores queued events in a ring buffer.
 *
 * This implementation is interrupt-safe provided that events are
 * queued in an interrupt context and dequeued in a non-interrupt
 * context. Queueing of one event must be completed before the next
 * event can be queued so interrupt safety is broken if a high
 * priority interrupt queues while interrupting a low priority
 * interrupt that is also queueing.
 */

#ifndef tiny_event_queue_h
#define tiny_event_queue_h

#include <stdbool.h>
#include <stddef.h>
#include "i_tiny_event_queue.h"
#include "tiny_ring_buffer.h"

typedef void (*tiny_queue_unable_to_enqueue_callback_t)(void);

typedef struct {
  i_tiny_event_queue_t interface;
  tiny_ring_buffer_t ring_buffer;
  tiny_queue_unable_to_enqueue_callback_t unable_to_queue_callback;
} tiny_event_queue_t;

/*!
 * Initializes the event queue with the provided buffer. Larger
 * buffers allow for more events to be queued simultaneously. When
 * an event is unable to be queued, the provided callback is used
 * to notify the client.
 */
void tiny_event_queue_init(
  tiny_event_queue_t* self,
  void* buffer,
  unsigned buffer_size,
  tiny_queue_unable_to_enqueue_callback_t unable_to_queue_callback);

/*!
 * Dequeues and raises at most one queued event. Returns true if an
 * event was raised and false otherwise.
 */
bool tiny_event_queue_run(tiny_event_queue_t* self);

#endif
