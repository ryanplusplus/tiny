/*!
 * @file
 * @brief Allows for stack data to be dynamically allocated without alloca() or VLAs.
 */

#ifndef tiny_stack_allocator_h
#define tiny_stack_allocator_h

#include <stddef.h>

enum {
  tiny_stack_allocator_largest_supported_size = 256
};

typedef void (*tiny_stack_allocator_callback_t)(void* context, void* data);

/*!
 * Invokes the provided callback to an aligned pointer of the requested size.
 *
 * @warning The allocated data is valid only during the callback.
 */
void tiny_stack_allocator_allocate_aligned(
  size_t size,
  void* context,
  tiny_stack_allocator_callback_t callback);

#endif
