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
  self->last_time_source_ticks = tiny_time_source_ticks(time_source);
  tiny_list_init(&self->timers);
}

static void add_timer(tiny_timer_group_t* self, tiny_timer_t* timer)
{
  tiny_list_remove(&self->timers, &timer->node);

  timer->expired = false;

  tiny_list_node_t* after = NULL;
  tiny_timer_ticks_t to_add_remaining_ticks = timer->expiration_ticks - self->current_ticks;

  tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
    tiny_timer_ticks_t remaining_ticks = timer->expiration_ticks - self->current_ticks;

    if(timer->expired || (to_add_remaining_ticks >= remaining_ticks)) {
      after = &timer->node;
    }
    else {
      break;
    }
  });

  if(after) {
    tiny_list_insert_after(&self->timers, after, &timer->node);
  }
  else {
    tiny_list_push_front(&self->timers, &timer->node);
  }
}

static tiny_time_source_ticks_t pending_ticks(tiny_timer_group_t* self)
{
  return tiny_time_source_ticks(self->time_source) - self->last_time_source_ticks;
}

static void start_timer(
  tiny_timer_group_t* self,
  bool periodic,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  void* context,
  tiny_timer_callback_t callback)
{
  timer->periodic = periodic;
  timer->callback = callback;
  timer->context = context;
  timer->start_ticks = ticks;
  timer->expiration_ticks = self->current_ticks + ticks + pending_ticks(self);

  add_timer(self, timer);
}

tiny_timer_ticks_t tiny_timer_group_run(tiny_timer_group_t* self)
{
  tiny_time_source_ticks_t current_ticks = tiny_time_source_ticks(self->time_source);
  tiny_time_source_ticks_t delta = current_ticks - self->last_time_source_ticks;
  self->last_time_source_ticks = current_ticks;

  tiny_timer_ticks_t last_ticks = self->current_ticks;
  self->current_ticks += delta;

  tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
    if(timer->expired) {
      continue;
    }

    tiny_timer_ticks_t remaining_ticks = timer->expiration_ticks - last_ticks;

    if(remaining_ticks <= delta) {
      timer->expired = true;
    }
    else {
      break;
    }
  });

  tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
    if(timer->expired) {
      if(!timer->periodic) {
        tiny_list_remove(&self->timers, &timer->node);
      }

      timer->callback(timer->context);

      if(timer->periodic && tiny_timer_is_running(self, timer)) {
        timer->expiration_ticks = self->current_ticks + timer->start_ticks;
        add_timer(self, timer);
      }
    }

    break;
  });

  tiny_list_for_each(&self->timers, tiny_timer_t, timer, {
    return tiny_timer_remaining_ticks(self, timer);
  });

  return (tiny_timer_ticks_t)-1;
}

void tiny_timer_start(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  void* context,
  tiny_timer_callback_t callback)
{
  start_timer(self, false, timer, ticks, context, callback);
}

void tiny_timer_start_periodic(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  void* context,
  tiny_timer_callback_t callback)
{
  start_timer(self, true, timer, ticks, context, callback);
}

extern inline void tiny_timer_stop(tiny_timer_group_t* self, tiny_timer_t* timer);

extern inline bool tiny_timer_is_running(tiny_timer_group_t* self, tiny_timer_t* timer);

tiny_timer_ticks_t tiny_timer_remaining_ticks(
  tiny_timer_group_t* self,
  tiny_timer_t* timer)
{
  tiny_timer_ticks_t remaining = timer->expiration_ticks - self->current_ticks;
  tiny_time_source_ticks_t pending = pending_ticks(self);

  if(!timer->expired && (remaining > pending)) {
    return remaining - pending;
  }
  else {
    return 0;
  }
}
