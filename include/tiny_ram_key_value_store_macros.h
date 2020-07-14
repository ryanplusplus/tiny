/*!
 * @file
 * @brief Helper macros for defining a RAM storage type and configuration.
 *
 * Example:
 *
 * #define my_key_value_pair_list(pair) \
 *   pair(key_foo, uint8_t) \
 *   pair(key_bar, uint32_t) \
 *
 * // Generates an enumeration that will allow your application to reference
 * // keys by name
 * enumerate_ram_key_value_pairs(my_key_value_pair_list);
 *
 * // Configures the name of the created storage struct
 * #define _storage_type_name my_storage_type_t
 *
 * // Generates the storage type with the configured name
 * // This is used to generate the configuration and is sized appropriately to
 * // hold all values
 * generate_storage_type_for_ram_key_value_pairs(my_key_value_pair_list);
 *
 * static const tiny_ram_key_value_store_key_value_pair_t my_key_value_pairs[] = {
 *   generate_configuration_pairs_from_ram_key_value_pairs(my_key_value_pair_list)
 * };
 *
 * static const tiny_ram_key_value_store_configuration_t configuration = {
 *   my_key_value_pairs,
 *   element_count(my_key_value_pairs)
 * };
 */

#ifndef tiny_ram_key_value_store_macros_h
#define tiny_ram_key_value_store_macros_h

#include <stdint.h>
#include <stddef.h>

#define ram_key_value_pairs_expand_as_enumeration(_key, value_type) _key,

#define enumerate_ram_key_value_pairs(_key_value_pairs)         \
  enum {                                                        \
    _key_value_pairs(ram_key_value_pairs_expand_as_enumeration) \
  }

#define ram_key_value_pairs_expand_as_storage_item(_key, _value_type) \
  uint8_t _key[sizeof(_value_type)];

#define generate_storage_type_for_ram_key_value_pairs(_key_value_pairs) \
  typedef struct {                                                      \
    _key_value_pairs(ram_key_value_pairs_expand_as_storage_item)        \
  } _storage_type_name

#define ram_key_value_pairs_expand_as_configuration_item(_key, _value_type) \
  { offsetof(_storage_type_name, _key), sizeof(_value_type) },

#define generate_configuration_pairs_from_ram_key_value_pairs(_key_value_pairs) \
  _key_value_pairs(ram_key_value_pairs_expand_as_configuration_item)

#endif
