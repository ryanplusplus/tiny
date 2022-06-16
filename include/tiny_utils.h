/*!
 * @file
 * @brief
 */

#ifndef tiny_utils_h
#define tiny_utils_h

#include <stddef.h>

/*!
 * Casts between pointer types. Useful for casting an abstract type to a concrete
 * type.
 */
#define reinterpret(_name, _cast_me, _type) \
  _type _name = (_type)_cast_me

/*!
 * Calculates the number of elements in an array.
 */
#define element_count(_array) \
  (sizeof(_array) / sizeof(_array[0]))

/*!
 * Calculates the size of the elements in an array.
 */
#define element_size(_array) \
  sizeof(_array[0])

/*!
 * Retrieves a pointer to a struct/container given a pointer to one of the
 * container's fields.
 */
#define container_of(_container_type, _field_name, _field) \
  (_container_type*)((uint8_t*)_field - offsetof(_container_type, _field_name))

/*!
 * Performs concatenation that expands macro arguments.
 */
#define tiny_concat(_a, _b) _tiny_concat(_a, _b)
#define _tiny_concat(_a, _b) _a##_b

/*!
 * Causes a compilation error if the condition expression evaluates to false.
 */
#define tiny_static_assert(_condition) \
  typedef int tiny_concat(tiny_static_assert_, __LINE__)[_condition ? 1 : -1]

#endif
