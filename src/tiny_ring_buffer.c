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
    return (self->head - self->tail) % self->capacity;
  }
}

void tiny_ring_buffer_at(tiny_ring_buffer_t* self, unsigned index, void* element)
{
  unsigned buffer_index = (self->tail + index) % self->capacity;
  uint8_t* source = (uint8_t*)self->buffer + buffer_index * self->element_size;
  memcpy(element, source, self->element_size);
}

void tiny_ring_buffer_insert(tiny_ring_buffer_t* self, const void* element)
{
  uint8_t* destination = (uint8_t*)self->buffer + self->head * self->element_size;
  memcpy(destination, element, self->element_size);

  if(self->head == self->tail && self->full) {
    self->tail = (self->tail + 1) % self->capacity;
  }

  self->head = (self->head + 1) % self->capacity;

  if(self->head == self->tail) {
    self->full = true;
  }
}

void tiny_ring_buffer_remove(tiny_ring_buffer_t* self, void* element)
{
  if(self->head != self->tail || self->full) {
    uint8_t* source = (uint8_t*)self->buffer + self->tail * self->element_size;
    memcpy(element, source, self->element_size);

    self->tail = (self->tail + 1) % self->capacity;
  }
}

void tiny_ring_buffer_clear(tiny_ring_buffer_t* self)
{
  self->head = 0;
  self->tail = 0;
  self->full = false;
}
