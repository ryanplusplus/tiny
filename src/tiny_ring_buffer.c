/*!
 * @file
 * @brief
 *
 * In order to fulfill the interrupt-safety guarantees made in the header:
 * - insert must only write to the head pointer when the ring buffer is not full
 * - remove must only write to the tail pointer
 * - remove must write to the tail pointer after copying the element
 */

#include <string.h>
#include <stdint.h>
#include "tiny_ring_buffer.h"

static inline unsigned increment_with_wrap(unsigned x, unsigned limit)
{
  if(++x >= limit) {
    x = 0;
  }
  return x;
}

void tiny_ring_buffer_init(
  tiny_ring_buffer_t* self,
  void* buffer,
  unsigned element_size,
  unsigned element_count)
{
  self->buffer = buffer;
  self->element_size = element_size;
  self->capacity = element_count;

  tiny_ring_buffer_clear(self);
}

extern inline unsigned tiny_ring_buffer_capacity(tiny_ring_buffer_t* self);

unsigned tiny_ring_buffer_count(tiny_ring_buffer_t* self)
{
  if(self->full) {
    return self->capacity;
  }
  else {
    unsigned count = self->head - self->tail;

    if(count > self->capacity) {
      count += self->capacity;
    }

    return count;
  }
}

void tiny_ring_buffer_at(tiny_ring_buffer_t* self, unsigned index, void* element)
{
  unsigned buffer_index = (self->tail + index);
  if(buffer_index >= self->capacity) {
    buffer_index -= self->capacity;
  }

  uint8_t* source = (uint8_t*)self->buffer + buffer_index * self->element_size;
  memcpy(element, source, self->element_size);
}

void tiny_ring_buffer_insert(tiny_ring_buffer_t* self, const void* element)
{
  uint8_t* destination = (uint8_t*)self->buffer + self->head * self->element_size;
  memcpy(destination, element, self->element_size);
  unsigned initial_head = self->head;
  unsigned initial_tail = self->tail;

  if(initial_head == initial_tail && self->full) {
    self->tail = increment_with_wrap(initial_tail, self->capacity);
  }

  unsigned new_head = increment_with_wrap(initial_head, self->capacity);
  self->head = new_head;

  if(new_head == self->tail) {
    self->full = true;
  }
}

void tiny_ring_buffer_remove(tiny_ring_buffer_t* self, void* element)
{
  unsigned initial_tail = self->tail;

  if(self->head != initial_tail || self->full) {
    uint8_t* source = (uint8_t*)self->buffer + initial_tail * self->element_size;
    memcpy(element, source, self->element_size);

    self->tail = increment_with_wrap(initial_tail, self->capacity);
  }
}

void tiny_ring_buffer_clear(tiny_ring_buffer_t* self)
{
  self->head = 0;
  self->tail = 0;
  self->full = false;
}
