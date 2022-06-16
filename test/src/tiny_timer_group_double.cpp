/*!
 * @file
 * @brief
 */

#include "double/tiny_timer_group_double.h"
#include "tiny_utils.h"

void tiny_timer_group_double_init(
  tiny_timer_group_double_t* self)
{
  tiny_time_source_double_init(&self->time_source);
  tiny_timer_group_init(&self->timer_group, &self->time_source.interface);
}

void tiny_timer_group_double_elapse_time(
  tiny_timer_group_double_t* self,
  tiny_timer_ticks_t ticks,
  tiny_time_source_ticks_t ticks_per_run)
{
  while(ticks) {
    tiny_time_source_ticks_t ticks_to_elapse = ticks >= ticks_per_run ? ticks_per_run : ticks;

    tiny_time_source_double_tick(&self->time_source, ticks_to_elapse);
    tiny_timer_group_run(&self->timer_group);

    ticks -= ticks_to_elapse;
  }
}
