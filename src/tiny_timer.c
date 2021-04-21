/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_timer.h"
#include "tiny_utils.h"

void tiny_timer_group_init(tiny_timer_group_t* self, i_tiny_time_source_t* time_source)
{
  self->time_source = time_source;
  self->last_ticks = tiny_time_source_ticks(time_source);
  tiny_list_init(&self->timers);
}

static void add_timer(tiny_timer_group_t* self, tiny_timer_t* timer)
{
  tiny_list_remove(&self->timers, &timer->node);
  tiny_list_push_back(&self->timers, &timer->node);

  if(timer->remaining_ticks < self->next_ready) {
    self->next_ready = timer->remaining_ticks;
  }
}

static void start_timer(
  tiny_timer_group_t* self,
  bool periodic,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  tiny_timer_callback_t callback,
  void* context)
{
  timer->periodic = periodic;
  timer->callback = callback;
  timer->context = context;
  timer->start_ticks = ticks;
  timer->remaining_ticks = ticks;

  add_timer(self, timer);
}

tiny_timer_ticks_t tiny_timer_group_run(tiny_timer_group_t* self)
{
  tiny_time_source_ticks_t current_ticks = tiny_time_source_ticks(self->time_source);
  tiny_time_source_ticks_t delta = current_ticks - self->last_ticks;
  self->last_ticks = current_ticks;

  self->next_ready = UINT16_MAX;
  bool timer_ready = false;

  tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
    if(delta < timer->remaining_ticks) {
      timer->remaining_ticks -= delta;

      if(timer->remaining_ticks < self->next_ready) {
        self->next_ready = timer->remaining_ticks;
      }
    }
    else {
      timer->remaining_ticks = 0;

      if(timer_ready) {
        self->next_ready = 0;
      }

      timer_ready = true;
    }
  });

  if(timer_ready) {
    tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
      if(timer->remaining_ticks == 0) {
        if(!timer->periodic) {
          tiny_list_remove(&self->timers, &timer->node);
        }

        timer->callback(self, timer->context);

        if(timer->periodic && tiny_timer_is_running(self, timer)) {
          timer->remaining_ticks = timer->start_ticks;
          add_timer(self, timer);
        }

        break;
      }
    });
  }

  return self->next_ready;
}

void tiny_timer_start(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  tiny_timer_callback_t callback,
  void* context)
{
  start_timer(self, false, timer, ticks, callback, context);
}

void tiny_timer_start_periodic(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  tiny_timer_callback_t callback,
  void* context)
{
  start_timer(self, true, timer, ticks, callback, context);
}

extern inline void tiny_timer_stop(tiny_timer_group_t* self, tiny_timer_t* timer);

extern inline bool tiny_timer_is_running(tiny_timer_group_t* self, tiny_timer_t* timer);

extern inline tiny_timer_ticks_t tiny_timer_remaining_ticks(tiny_timer_group_t* self, tiny_timer_t* timer);
