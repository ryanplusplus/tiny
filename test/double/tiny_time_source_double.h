/*!
 * @file
 * @brief
 */

#ifndef tiny_time_source_double_h
#define tiny_time_source_double_h

#include "i_tiny_time_source.h"

typedef struct {
  i_tiny_time_source_t interface;
  tiny_time_source_ticks_t ticks;
} tiny_time_source_double_t;

/*!
 * Initializes a time source double. Sets ticks to 0.
 */
void tiny_time_source_double_init(tiny_time_source_double_t* self);

/*!
 * Sets the current ticks.
 */
void tiny_time_source_double_set_ticks(tiny_time_source_double_t* self, tiny_time_source_ticks_t ticks);

/*!
 * Moves the current ticks forward by the specified number of ticks.
 */
void tiny_time_source_double_tick(tiny_time_source_double_t* self, tiny_time_source_ticks_t ticks = 1);

#endif
