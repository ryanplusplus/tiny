/*!
 * @file
 * @brief
 */

#ifndef tiny_utils_h
#define tiny_utils_h

#define reinterpret(name, cast_me, type) \
  type name = (type)cast_me;

#define element_count(array) \
  (sizeof(array) / sizeof(array[0]))

#define element_size(array) \
  sizeof(array[0])

#endif
