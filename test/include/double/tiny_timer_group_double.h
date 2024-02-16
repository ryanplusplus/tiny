/*!
 * @file
 * @brief Timer group test double.
 */

#ifndef tiny_timer_group_double_h
#define tiny_timer_group_double_h

#include "tiny_time_source_double.h"

extern "C" {
#include "tiny_timer.h"
}

typedef struct {
  tiny_timer_group_t timer_group;
  tiny_time_source_double_t time_source;
} tiny_timer_group_double_t;

/*!
 * Initializes a timer group test double.
 */
void tiny_timer_group_double_init(
  tiny_timer_group_double_t* self);

/*!
 * Elapses time and runs the timer group.
 */
void tiny_timer_group_double_elapse_time(
  tiny_timer_group_double_t* self,
  tiny_timer_ticks_t ticks);

#endif
