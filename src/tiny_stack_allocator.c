/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdint.h>
#include "tiny_stack_allocator.h"
#include "tiny_utils.h"

#define max(a, b) ((a) > (b) ? a : b)

#define define_worker(_size)                                                          \
  static void worker_##_size(tiny_stack_allocator_callback_t callback, void* context) \
  {                                                                                   \
    max_align_t data[max(_size, sizeof(max_align_t)) / sizeof(max_align_t)];          \
    callback(context, data);                                                          \
  }                                                                                   \
  typedef int dummy##_size

define_worker(8);
define_worker(16);
define_worker(32);
define_worker(64);
define_worker(128);
define_worker(256);

typedef struct
{
  size_t size;
  void (*worker)(tiny_stack_allocator_callback_t callback, void* context);
} worker_t;

static const worker_t workers[] = {
  { 8, worker_8 },
  { 16, worker_16 },
  { 32, worker_32 },
  { 64, worker_64 },
  { 128, worker_128 },
  { 256, worker_256 },
};

void tiny_stack_allocator_allocate_aligned(
  size_t size,
  void* context,
  tiny_stack_allocator_callback_t callback)
{
  for(uint8_t i = 0; i < element_count(workers); i++) {
    if(size <= workers[i].size) {
      workers[i].worker(callback, context);
      return;
    }
  }
}
