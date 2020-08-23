/*!
 * @file
 * @brief Ring/circular buffer. Usable as a FIFO queue for fixed-size elements.
 *
 * Can be used to pass data from the interrupt context to the non-interrupt
 * context if the follow constraints hold:
 * - `unsigned` is an atomic type
 * - The interrupt context only calls `insert` and `count`
 * - The non-interrupt context only calls `remove`, `at`, and `count`
 * - The ring buffer is never allowed to get full
 */

#ifndef tiny_ring_buffer_h
#define tiny_ring_buffer_h

#include <stdbool.h>

typedef struct {
  void* buffer;
  unsigned element_size;
  unsigned head;
  volatile unsigned tail;
  unsigned capacity;
  bool full;
} tiny_ring_buffer_t;

/*!
 * Initializes the ring buffer.
 */
void tiny_ring_buffer_init(
  tiny_ring_buffer_t* self,
  void* buffer,
  unsigned element_size,
  unsigned element_count);

/*!
 * The number of elements currently stored in the ring buffer.
 */
unsigned tiny_ring_buffer_count(tiny_ring_buffer_t* self);

/*!
 * Gets the element at the specified index. If the index is larger than
 * the size then the element buffer will not be written.
 */
void tiny_ring_buffer_at(tiny_ring_buffer_t* self, unsigned index, void* element);

/*!
 * Insert an element into the ring buffer. If the ring buffer is full,
 * the oldest element will be overwritten.
 */
void tiny_ring_buffer_insert(tiny_ring_buffer_t* self, const void* element);

/*!
 * Removes the oldest element from the ring buffer and writes it into the
 * provided buffer. If the ring buffer is empty then the element will not
 * be written.
 */
void tiny_ring_buffer_remove(tiny_ring_buffer_t* self, void* element);

/*!
 * Removes all elements from the ring buffer.
 */
void tiny_ring_buffer_clear(tiny_ring_buffer_t* self);

#endif
