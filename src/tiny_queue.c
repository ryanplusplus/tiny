/*!
 * @file
 * @brief
 */

#include "tiny_queue.h"

static void insert_element_size(tiny_queue_t* self, uint16_t element_size)
{
  uint8_t* destination = (uint8_t*)&element_size;
  for(uint16_t i = 0; i < sizeof(uint16_t); i++) {
    tiny_ring_buffer_insert(&self->ring_buffer, destination);
    destination++;
  }
}

static void insert_element(tiny_queue_t* self, const void* element, uint16_t element_size)
{
  const uint8_t* source = element;
  for(uint16_t i = 0; i < element_size; i++) {
    tiny_ring_buffer_insert(&self->ring_buffer, source);
    source++;
  }
}

static void remove_element_size(tiny_queue_t* self, uint16_t* size_placeholder)
{
  uint8_t* destination = (uint8_t*)size_placeholder;
  for(uint16_t i = 0; i < sizeof(uint16_t); i++) {
    tiny_ring_buffer_remove(&self->ring_buffer, destination);
    destination++;
  }
}

static void remove_element(tiny_queue_t* self, void* element, uint16_t element_size)
{
  uint8_t* destination = element;
  for(uint16_t i = 0; i < element_size; i++) {
    tiny_ring_buffer_remove(&self->ring_buffer, destination);
    destination++;
  }
}

static void discard_element(tiny_queue_t* self, uint16_t element_size)
{
  for(uint16_t i = 0; i < element_size; i++) {
    uint8_t discard;
    tiny_ring_buffer_remove(&self->ring_buffer, &discard);
  }
}

static void peek_element_size_at_ring_buffer_index(tiny_queue_t* self, uint16_t* element_size, unsigned location)
{
  uint8_t* destination = (uint8_t*)element_size;
  for(unsigned i = location; i < location + sizeof(uint16_t); i++) {
    tiny_ring_buffer_at(&self->ring_buffer, i, destination);
    destination++;
  }
}

static unsigned ring_buffer_index_for_element_index(tiny_queue_t* self, uint16_t element_index)
{
  uint16_t element_size;
  unsigned location = 0;
  for(uint16_t i = 0; i < element_index; i++) {
    peek_element_size_at_ring_buffer_index(self, &element_size, location);
    location += (uint16_t)(element_size + sizeof(uint16_t));
  }
  return location;
}

static void peek_element_at_ring_buffer_index(tiny_queue_t* self, void* element, uint16_t element_size, unsigned location)
{
  uint8_t* destination = element;
  for(unsigned i = location; i < location + element_size; i++) {
    tiny_ring_buffer_at(&self->ring_buffer, i, destination);
    destination++;
  }
}

bool tiny_queue_enqueue(tiny_queue_t* self, const void* element, uint16_t element_size)
{
  size_t required_space = element_size + sizeof(uint16_t);
  size_t available_space = tiny_ring_buffer_capacity(&self->ring_buffer) - tiny_ring_buffer_count(&self->ring_buffer);

  if(required_space <= available_space) {
    insert_element_size(self, element_size);
    insert_element(self, element, element_size);
    self->element_count++;
    return true;
  }
  else {
    return false;
  }
}

void tiny_queue_dequeue(tiny_queue_t* self, void* element, uint16_t* size_storage)
{
  remove_element_size(self, size_storage);
  remove_element(self, element, *size_storage);
  self->element_count--;
}

void tiny_queue_discard(tiny_queue_t* self)
{
  uint16_t size_storage;
  remove_element_size(self, &size_storage);
  discard_element(self, size_storage);
  self->element_count--;
}

void tiny_queue_peek(tiny_queue_t* self, void* element, uint16_t* size_storage, uint16_t element_index)
{
  unsigned i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_size_at_ring_buffer_index(self, size_storage, i);
  peek_element_at_ring_buffer_index(self, element, *size_storage, i + sizeof(uint16_t));
}

void tiny_queue_peek_partial(tiny_queue_t* self, void* element, uint16_t size, uint16_t offset, uint16_t element_index)
{
  unsigned i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_at_ring_buffer_index(self, element, size, (uint16_t)(i + offset + sizeof(uint16_t)));
}

void tiny_queue_peek_size(tiny_queue_t* self, uint16_t* size_storage, uint16_t element_index)
{
  unsigned i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_size_at_ring_buffer_index(self, size_storage, i);
}

uint16_t tiny_queue_count(tiny_queue_t* self)
{
  return self->element_count;
}

void tiny_queue_init(
  tiny_queue_t* self,
  void* storage,
  unsigned storage_size)
{
  self->element_count = 0;
  tiny_ring_buffer_init(&self->ring_buffer, storage, sizeof(uint8_t), storage_size);
}

tiny_queue_raw_element_iterator_t tiny_queue_raw_element_iterator(
  tiny_queue_t* instance,
  uint16_t index)
{
  unsigned ring_buffer_index = ring_buffer_index_for_element_index(instance, index);
  unsigned offset = ring_buffer_index + (unsigned)sizeof(uint16_t) + instance->ring_buffer.tail;
  if(offset >= tiny_ring_buffer_capacity(&instance->ring_buffer)) {
    offset -= tiny_ring_buffer_capacity(&instance->ring_buffer);
  }

  return (tiny_queue_raw_element_iterator_t){
    .offset = offset,
    .buffer = instance->ring_buffer.buffer,
    .buffer_capacity = tiny_ring_buffer_capacity(&instance->ring_buffer),
  };
}
