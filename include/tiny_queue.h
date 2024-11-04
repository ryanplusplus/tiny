/*!
 * @file
 * @brief FIFO queue for arbitrarily-sized elements.
 */

#ifndef tiny_queue_h
#define tiny_queue_h

#include <stddef.h>
#include <stdint.h>
#include "tiny_ring_buffer.h"

typedef struct {
  uint16_t element_count;
  tiny_ring_buffer_t ring_buffer;
} tiny_queue_t;

/*!
 * Initialize a queue with the provided buffer.
 */
void tiny_queue_init(
  tiny_queue_t* self,
  void* buffer,
  size_t buffer_size);

/*!
 * Add an element to the queue. Returns true if the element was successfully enqueued, false otherwise
 */
bool tiny_queue_enqueue(tiny_queue_t* self, const void* element, uint16_t size);

/*!
 * Remove an element from the queue and copy it into the provided element buffer.
 * @pre The queue is not empty
 */
void tiny_queue_dequeue(tiny_queue_t* self, void* element, uint16_t* size);

/*!
 * Similar to Dequeue but the element is not retrieved.
 * @pre The queue is not empty
 */
void tiny_queue_discard(tiny_queue_t* self);

/*!
 * Peeks an element from the queue without removing it and copies it into the provided element buffer.
 * @pre 0 <= index < The count of elements in the queue
 */
void tiny_queue_peek(tiny_queue_t* self, void* element, uint16_t* size, uint16_t index);

/*!
 * Peek part of an element. Only copies out up to the specified number of bytes.
 * @pre 0 <= index < The count of elements in the queue
 */
void tiny_queue_peek_partial(tiny_queue_t* self, void* element, uint16_t size_limit, uint16_t index);

/*!
 * Peek part of an element. Only copies out up to the specified number of bytes.
 * @pre 0 <= index < The count of elements in the queue
 */
void tiny_queue_peek_size(tiny_queue_t* self, uint16_t* size, uint16_t index);

/*!
 * Get the number of elements in the queue.
 */
uint16_t tiny_queue_count(tiny_queue_t* self);

#endif
