/*!
 * @file
 * @brief Runs one-shot and periodic timers using client-allocated memory.
 *
 * The tick resolution is not specified, but will generally be 1 millisecond.
 *
 * Timer durations may be longer than specified but will not be shorter (within
 * the limits of the tick resolution).
 */

#ifndef tiny_timer_h
#define tiny_timer_h

#include <stdbool.h>
#include <stdint.h>
#include "i_tiny_time_source.h"
#include "tiny_list.h"

typedef uint32_t tiny_timer_ticks_t;

struct tiny_timer_group_t;

typedef void (*tiny_timer_callback_t)(void* context);

typedef struct {
  tiny_list_node_t node;
  tiny_timer_callback_t callback;
  void* context;
  tiny_timer_ticks_t start_ticks;
  tiny_timer_ticks_t expiration_ticks;
  bool periodic;
  bool expired;
} tiny_timer_t;

typedef struct tiny_timer_group_t {
  i_tiny_time_source_t* time_source;
  tiny_list_t timers;
  tiny_time_source_ticks_t last_time_source_ticks;
  tiny_timer_ticks_t current_ticks;
} tiny_timer_group_t;

/*!
 * Initializes a timer group.
 */
void tiny_timer_group_init(
  tiny_timer_group_t* self,
  i_tiny_time_source_t* time_source);

/*!
 * Runs a timer group. Services at most one timer per call. Returns the number
 * of ticks until the next timer will be ready to run. This will generally be
 * called in the main loop.
 */
tiny_timer_ticks_t tiny_timer_group_run(
  tiny_timer_group_t* self);

/*!
 * Returns the ticks until the next timer will be ready to execute.
 */
tiny_timer_ticks_t tiny_timer_ticks_until_next_ready(
  tiny_timer_group_t* self);

/*!
 * Starts a timer.
 */
void tiny_timer_start(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  void* context,
  tiny_timer_callback_t callback);

/*!
 * Starts a periodic timer.
 */
void tiny_timer_start_periodic(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  void* context,
  tiny_timer_callback_t callback);

/*!
 * Stops a timer.
 */
inline void tiny_timer_stop(
  tiny_timer_group_t* self,
  tiny_timer_t* timer)
{
  tiny_list_remove(&self->timers, &timer->node);
}

/*!
 * Returns true if the specified timer is running and false otherwise.
 */
inline bool tiny_timer_is_running(
  tiny_timer_group_t* self,
  tiny_timer_t* timer)
{
  return tiny_list_contains(&self->timers, &timer->node);
}

/*!
 * Returns the remaining ticks for a running timer.
 */
tiny_timer_ticks_t tiny_timer_remaining_ticks(
  tiny_timer_group_t* self,
  tiny_timer_t* timer);

#endif
