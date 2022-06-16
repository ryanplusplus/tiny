/*!
 * @file
 * @brief Abstract key value store using pre-defined keys and fixed-sized values.
 *
 * Provides notifications when stored values change.
 */

#ifndef i_tiny_key_value_store_h
#define i_tiny_key_value_store_h

#include <stdint.h>
#include <stdbool.h>
#include "i_tiny_event.h"

typedef uint8_t tiny_key_value_store_key_t;

typedef struct {
  tiny_key_value_store_key_t key;
  const void* value;
} tiny_key_value_store_on_change_args_t;

struct i_tiny_key_value_store_api_t;

typedef struct {
  const struct i_tiny_key_value_store_api_t* api;
} i_tiny_key_value_store_t;

typedef struct i_tiny_key_value_store_api_t {
  /*!
   * Reads the value corresponding to the key into the provided buffer. Note that ensuring
   * that the buffer is large enough to store the value is the responsibility of the client.
   */
  void (*read)(i_tiny_key_value_store_t* self, tiny_key_value_store_key_t key, void* value);

  /*!
   * Writes the value from the provided buffer to the storage associated with the provided
   * key. Note that ensuring that the provided buffer is sized appropriately is the
   * responsibility of the client.
   */
  void (*write)(i_tiny_key_value_store_t* self, tiny_key_value_store_key_t key, const void* value);

  /*!
   * Returns true if the key value store contains data corresponding to the provided key.
   */
  bool (*contains)(i_tiny_key_value_store_t* self, tiny_key_value_store_key_t key);

  /*!
   * Returns the size in bytes of the value associated with the provided key.
   */
  uint8_t (*size)(i_tiny_key_value_store_t* self, tiny_key_value_store_key_t key);

  /*!
   * Provides access to an event raised each time the value for a key-value pair changes.
   */
  i_tiny_event_t* (*on_change)(i_tiny_key_value_store_t* self);
} i_tiny_key_value_store_api_t;

#define tiny_key_value_store_read(self, key, value) \
  (self)->api->read((self), (key), (value))

#define tiny_key_value_store_write(self, key, value) \
  (self)->api->write((self), (key), (value))

#define tiny_key_value_store_contains(self, key) \
  (self)->api->contains((self), (key))

#define tiny_key_value_store_size(self, key) \
  (self)->api->size((self), (key))

#define tiny_key_value_store_on_change(self) \
  (self)->api->on_change((self))

#endif
