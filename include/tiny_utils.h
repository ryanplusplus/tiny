/*!
 * @file
 * @brief
 */

#ifndef tiny_utils_h
#define tiny_utils_h

#include <stddef.h>

#define reinterpret(_name, _cast_me, _type) \
  _type _name = (_type)_cast_me

#define element_count(_array) \
  (sizeof(_array) / sizeof(_array[0]))

#define element_size(_array) \
  sizeof(_array[0])

#define container_of(_container_type, _field_name, _field) \
  (_container_type*)((uint8_t*)_field - offsetof(_container_type, _field_name))

#define _concat(_a, _b) _a##_b
#define concat(_a, _b) _concat(_a, _b)

#define static_assert(_condition) \
  typedef int concat(static_assert_, __LINE__)[_condition ? 1 : -1]

#endif
