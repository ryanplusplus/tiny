/*!
 * @file
 * @brief
 */

#include <string.h>
#include <stddef.h>
#include "tiny_ram_key_value_store.h"
#include "tiny_utils.h"

static uint16_t offset(tiny_ram_key_value_store_t* self, tiny_key_value_store_key_t key) {
  return self->configuration->key_value_pairs[key].value_offset;
}

static uint8_t size(i_tiny_key_value_store_t* _self, tiny_key_value_store_key_t key) {
  reinterpret(self, _self, tiny_ram_key_value_store_t*);
  return self->configuration->key_value_pairs[key].size;
}

static void read(i_tiny_key_value_store_t* _self, tiny_key_value_store_key_t key, void* value) {
  reinterpret(self, _self, tiny_ram_key_value_store_t*);
  memcpy(value, (uint8_t*)self->storage + offset(self, key), size(&self->interface, key));
}

static void write(i_tiny_key_value_store_t* _self, tiny_key_value_store_key_t key, const void* value) {
  reinterpret(self, _self, tiny_ram_key_value_store_t*);
  uint8_t value_size = size(&self->interface, key);
  void* location = (uint8_t*)self->storage + offset(self, key);

  if(memcmp(location, value, value_size)) {
    memcpy(location, value, value_size);

    tiny_key_value_store_on_change_args_t args = { key, value };
    tiny_event_publish(&self->on_change, &args);
  }
}

static bool contains(i_tiny_key_value_store_t* _self, tiny_key_value_store_key_t key) {
  reinterpret(self, _self, tiny_ram_key_value_store_t*);
  return key < self->configuration->key_value_pair_count;
}

static i_tiny_event_t* on_change(i_tiny_key_value_store_t* _self) {
  reinterpret(self, _self, tiny_ram_key_value_store_t*);
  return &self->on_change.interface;
}

static const i_tiny_key_value_store_api_t api = {
  read, write, contains, size, on_change
};

static void initialize_storage(tiny_ram_key_value_store_t* self) {
  tiny_key_value_store_key_t last_key = self->configuration->key_value_pair_count - 1;
  uint16_t last_offset = offset(self, last_key);
  uint8_t last_size = size(&self->interface, last_key);
  memset(self->storage, 0, last_offset + last_size);
}

void tiny_ram_key_value_store_init(
  tiny_ram_key_value_store_t* self,
  const tiny_ram_key_value_store_configuration_t* configuration,
  void* storage) {
  self->interface.api = &api;
  self->configuration = configuration;
  self->storage = storage;

  tiny_event_init(&self->on_change);

  initialize_storage(self);
}
