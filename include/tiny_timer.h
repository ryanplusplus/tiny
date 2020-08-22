/*!
 * @file
 * @brief Runs one-shot timers using client-allocated memory.
 */

#ifndef tiny_timer_h
#define tiny_timer_h

#include <stdint.h>
#include <stdbool.h>
#include "i_tiny_time_source.h"
#include "tiny_list.h"

typedef uint16_t tiny_timer_ticks_t;

struct tiny_timer_group_t;

typedef void (*tiny_timer_callback_t)(struct tiny_timer_group_t* group, void* context);

typedef struct {
  tiny_list_node_t node;
  tiny_timer_callback_t callback;
  void* context;
  tiny_timer_ticks_t remaining_ticks;
} tiny_timer_t;

typedef struct tiny_timer_group_t {
  i_tiny_time_source_t* time_source;
  tiny_list_t timers;
  tiny_time_source_ticks_t last_ticks;
} tiny_timer_group_t;

/*!
 * Initializes a timer group.
 */
void tiny_timer_group_init(
  tiny_timer_group_t* self,
  i_tiny_time_source_t* time_source);

/*!
 * Runs a timer group. Services at most one timer per call. Returns the number
 * of ticks until the next timer will be ready to run.
 */
tiny_timer_ticks_t tiny_timer_group_run(
  tiny_timer_group_t* self);

/*!
 * Starts a timer.
 */
void tiny_timer_start(
  tiny_timer_group_t* self,
  tiny_timer_t* timer,
  tiny_timer_ticks_t ticks,
  tiny_timer_callback_t callback,
  void* context);

/*!
 * Stops a timer.
 */
void tiny_timer_stop(
  tiny_timer_group_t* self,
  tiny_timer_t* timer);

/*!
 * Returns true if the specified timer is running and false otherwise.
 */
bool tiny_timer_is_running(
  tiny_timer_group_t* self,
  tiny_timer_t* timer);

/*!
 * Returns the remaining ticks for a running timer.
 */
tiny_timer_ticks_t tiny_timer_remaining_ticks(
  tiny_timer_group_t* self,
  tiny_timer_t* timer);

#endif
