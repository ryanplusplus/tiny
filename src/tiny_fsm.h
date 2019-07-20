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

enum {
  tiny_fsm_entry,
  tiny_fsm_exit,
  tiny_fsm_user_signal_start
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
void tiny_fsm_init(tiny_fsm_t* self, tiny_fsm_state_t initial);

/*!
 * Sends a signal and optional signal data to the current state.
 */
void tiny_fsm_send_signal(tiny_fsm_t* self, tiny_fsm_signal_t signal, const void* data);

/*!
 * Transitions the FSM to the target state. Sends exit to the current state, changes
 * state, then sends entry to the target state.
 */
void tiny_fsm_transition(tiny_fsm_t* self, tiny_fsm_state_t target);

#endif
