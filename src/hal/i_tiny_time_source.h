/*!
 * @file
 * @brief Abstraction for getting the current system time. This can be implemented
 * using something like Arduino's millis() or by using a system tick interrupt to
 * keep time.
 */

#ifndef i_tiny_time_source_h
#define i_tiny_time_source_h

#include <stdint.h>

typedef uint16_t tiny_time_source_ticks_t;

struct i_tiny_time_source_api_t;

typedef struct {
  const struct i_tiny_time_source_api_t* api;
} i_tiny_time_source_t;

typedef struct i_tiny_time_source_api_t {
  /*!
   * Returns the current time source tick count.
   */
  tiny_time_source_ticks_t (*ticks)(i_tiny_time_source_t* self);
} i_tiny_time_source_api_t;

#define tiny_time_source_ticks(self) \
  (self)->api->ticks((self))

#endif
