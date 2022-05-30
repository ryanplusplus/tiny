/*!
 * @file
 * @brief
 */

#ifndef tiny_stack_allocator_h
#define tiny_stack_allocator_h

#include <stddef.h>

enum {
  tiny_stack_allocator_largest_supported_size = 256
};

typedef void (*tiny_stack_allocator_callback_t)(void* context, void* data);

/*!
 * Invokes the provided callback to an aligned pointer of the requested
 */
void tiny_stack_allocator_allocate_aligned(
  size_t size,
  void* context,
  tiny_stack_allocator_callback_t callback);

#endif
