/*!
 * @file
 * @brief Key value store that stores all values in RAM. Read and write access is
 * constant time because all keys are are required to be consecutive starting from 0.
 *
 * It is highly recommended to use or at least start with the macros defined in
 * tiny_ram_key_value_store_macros.h to create the configuration. It is non-trivial
 * to do this at compile-time so that the configuration can be stored in ROM.
 *
 * @warning If data is written un-aligned it may be published unaligned.
 */

#ifndef tiny_ram_key_value_store_h
#define tiny_ram_key_value_store_h

#include "i_tiny_key_value_store.h"
#include "tiny_ram_key_value_store_macros.h"
#include "tiny_event.h"

typedef struct {
  uint16_t value_offset;
  uint8_t size;
} tiny_ram_key_value_store_key_value_pair_t;

typedef struct {
  const tiny_ram_key_value_store_key_value_pair_t* key_value_pairs;
  uint8_t key_value_pair_count;
} tiny_ram_key_value_store_configuration_t;

typedef struct {
  i_tiny_key_value_store_t interface;
  tiny_event_t on_change;
  const tiny_ram_key_value_store_configuration_t* configuration;
  void* storage;
} tiny_ram_key_value_store_t;

/*!
 * Initializes a key value store. All values are initialized to 0.
 */
void tiny_ram_key_value_store_init(
  tiny_ram_key_value_store_t* self,
  const tiny_ram_key_value_store_configuration_t* configuration,
  void* storage);

#endif
