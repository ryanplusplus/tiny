/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdint.h>
#include "tiny_stack_allocator.h"
#include "tiny_utils.h"

typedef struct {
  uint8_t data;
  union {
    int _int;
    long long _long_long;
    float _float;
    double _double;
    long double _long_double;
    void* _pointer;
  } alignment;
} alignment_t;

enum {
  alignment = offsetof(alignment_t, alignment),
};

#define define_worker(_size)                                                          \
  static void worker_##_size(tiny_stack_allocator_callback_t callback, void* context) \
  {                                                                                   \
    uint8_t data[_size + alignment - 1];                                              \
    uintptr_t offset = alignment - ((uintptr_t)data % alignment);                     \
    if(offset == alignment) {                                                         \
      offset = 0;                                                                     \
    }                                                                                 \
    callback(context, &data[offset]);                                                 \
  }                                                                                   \
  typedef int dummy##_size

define_worker(8);
define_worker(16);
define_worker(32);
define_worker(64);
define_worker(128);
define_worker(256);

typedef struct {
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
