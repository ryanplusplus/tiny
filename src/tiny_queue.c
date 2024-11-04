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

static void remove_element_size(tiny_queue_t* self, uint16_t* sizePlaceHolder)
{
  uint8_t* destination = (uint8_t*)sizePlaceHolder;
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

static void peek_element_size_at_ring_buffer_index(tiny_queue_t* self, uint16_t* element_size, uint16_t location)
{
  uint8_t* destination = (uint8_t*)element_size;
  for(uint16_t i = location; i < location + sizeof(uint16_t); i++) {
    tiny_ring_buffer_at(&self->ring_buffer, i, destination);
    destination++;
  }
}

static uint16_t ring_buffer_index_for_element_index(tiny_queue_t* self, uint16_t element_index)
{
  uint16_t element_size;
  uint16_t location = 0;
  for(uint16_t i = 0; i < element_index; i++) {
    peek_element_size_at_ring_buffer_index(self, &element_size, location);
    location += element_size + sizeof(uint16_t);
  }
  return location;
}

static void peek_element_at_ring_buffer_index(tiny_queue_t* self, void* element, uint16_t element_size, uint16_t location)
{
  uint8_t* destination = element;
  for(uint16_t i = location; i < location + element_size; i++) {
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

void tiny_queue_dequeue(tiny_queue_t* self, void* element, uint16_t* sizeStorage)
{
  remove_element_size(self, sizeStorage);
  remove_element(self, element, *sizeStorage);
  self->element_count--;
}

void tiny_queue_discard(tiny_queue_t* self)
{
  uint16_t sizeStorage;
  remove_element_size(self, &sizeStorage);
  discard_element(self, sizeStorage);
  self->element_count--;
}

void tiny_queue_peek(tiny_queue_t* self, void* element, uint16_t* sizeStorage, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_size_at_ring_buffer_index(self, sizeStorage, i);
  peek_element_at_ring_buffer_index(self, element, *sizeStorage, i + sizeof(uint16_t));
}

void tiny_queue_peek_partial(tiny_queue_t* self, void* element, uint16_t size, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_at_ring_buffer_index(self, element, size, i + sizeof(uint16_t));
}

void tiny_queue_peek_size(tiny_queue_t* self, uint16_t* sizeStorage, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(self, element_index);
  peek_element_size_at_ring_buffer_index(self, sizeStorage, i);
}

uint16_t tiny_queue_count(tiny_queue_t* self)
{
  return self->element_count;
}

void tiny_queue_init(
  tiny_queue_t* self,
  void* storage,
  size_t storageSize)
{
  self->element_count = 0;
  tiny_ring_buffer_init(&self->ring_buffer, storage, sizeof(uint8_t), storageSize);
}
