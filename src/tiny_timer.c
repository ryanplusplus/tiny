/*!
 * @file
 * @brief
 */

#include "tiny_timer.h"
#include "tiny_utils.h"

void tiny_timer_group_init(tiny_timer_group_t* self, i_tiny_time_source_t* time_source) {
  self->time_source = time_source;
  self->last_ticks = tiny_time_source_ticks(time_source);
  tiny_list_init(&self->timers);
}

typedef struct {
  tiny_timer_group_t* group;
  tiny_time_source_ticks_t delta;
  bool callback_has_been_invoked;
} for_each_run_context_t;

static bool for_each_run(tiny_list_node_t* node, uint16_t index, void* _context) {
  (void)index;
  reinterpret(timer, node, tiny_timer_t*);
  reinterpret(context, _context, for_each_run_context_t*);

  if(context->delta < timer->remaining_ticks) {
    timer->remaining_ticks -= context->delta;
  }
  else {
    timer->remaining_ticks = 0;

    if(!context->callback_has_been_invoked) {
      context->callback_has_been_invoked = true;
      tiny_list_remove(&context->group->timers, &timer->node);
      timer->callback(context->group, timer->context);
    }
  }

  return true;
}

bool tiny_timer_group_run(tiny_timer_group_t* self) {
  tiny_time_source_ticks_t current_ticks = tiny_time_source_ticks(self->time_source);
  tiny_time_source_ticks_t delta = current_ticks - self->last_ticks;
  self->last_ticks = current_ticks;

  for_each_run_context_t context = { self, delta, false };
  tiny_list_for_each(&self->timers, for_each_run, &context);

  return context.callback_has_been_invoked;
}

void tiny_timer_start(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  tiny_timer_callback_t callback,
  void* context) {
  timer->callback = callback;
  timer->context = context;
  timer->remaining_ticks = ticks;

  tiny_list_remove(&self->timers, &timer->node);
  tiny_list_push_back(&self->timers, &timer->node);
}

void tiny_timer_stop(tiny_timer_group_t* self, tiny_timer_t* timer) {
  tiny_list_remove(&self->timers, &timer->node);
}

bool tiny_timer_is_running(tiny_timer_group_t* self, tiny_timer_t* timer) {
  return tiny_list_contains(&self->timers, &timer->node);
}

tiny_timer_ticks_t tiny_timer_remaining_ticks(tiny_timer_group_t* self, tiny_timer_t* timer) {
  (void)self;
  return timer->remaining_ticks;
}
