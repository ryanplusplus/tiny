/*!
 * @file
 * @brief
 */

#ifndef tiny_timer_group_double_h
#define tiny_timer_group_double_h

#include "tiny_time_source_double.h"

extern "C" {
#include "tiny_timer.h"
}

typedef struct
{
  tiny_timer_group_t timer_group;
  tiny_time_source_double_t time_source;
} tiny_timer_group_double_t;

void tiny_timer_group_double_init(
  tiny_timer_group_double_t* self);

void tiny_timer_group_double_elapse_time(
  tiny_timer_group_double_t* self,
  tiny_timer_ticks_t ticks,
  tiny_time_source_ticks_t ticks_per_run = 1);

#endif
