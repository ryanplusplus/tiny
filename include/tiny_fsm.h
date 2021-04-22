/*!
 * @file
 * @brief Simple implementation of a state chart that models functions as states.
 *
 * Signals are sent to the current state by invoking the state function with the
 * signal and associated data.
 *
 * When the FSM transitions to a new state, the exit signal is sent to the current
 * state, the current state is changed to the new state, then the entry signal is
 * sent to the new state.
 */

#ifndef tiny_fsm_h
#define tiny_fsm_h

#include <stdint.h>
#include <stddef.h>

enum {
  tiny_fsm_signal_entry,
  tiny_fsm_signal_exit,
  tiny_fsm_signal_user_start
};
typedef uint8_t tiny_fsm_signal_t;

struct tiny_fsm_t;

typedef void (*tiny_fsm_state_t)(
  struct tiny_fsm_t* fsm,
  tiny_fsm_signal_t signal,
  const void* data);

typedef struct tiny_fsm_t {
  tiny_fsm_state_t current;
} tiny_fsm_t;

/*!
 * Initializes an FSM with the specified initial state. Sends the entry signal to the
 * initial state.
 */
inline void tiny_fsm_init(tiny_fsm_t* self, tiny_fsm_state_t initial)
{
  self->current = initial;
  self->current(self, tiny_fsm_signal_entry, NULL);
}

/*!
 * Sends a signal and optional signal data to the current state.
 */
inline void tiny_fsm_send_signal(tiny_fsm_t* self, tiny_fsm_signal_t signal, const void* data)
{
  self->current(self, signal, data);
}

/*!
 * Transitions the FSM to the target state. Sends exit to the current state, changes
 * state, then sends entry to the target state.
 */
void tiny_fsm_transition(tiny_fsm_t* self, tiny_fsm_state_t target);

#endif
