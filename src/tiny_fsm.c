/*!
 * @file
 * @brief
 */

#include "tiny_fsm.h"

extern inline void tiny_fsm_init(tiny_fsm_t* self, tiny_fsm_state_t initial);

extern inline void tiny_fsm_send_signal(tiny_fsm_t* self, tiny_fsm_signal_t signal, const void* data);

void tiny_fsm_transition(tiny_fsm_t* self, tiny_fsm_state_t target)
{
  self->current(self, tiny_fsm_signal_exit, NULL);
  self->current = target;
  self->current(self, tiny_fsm_signal_entry, NULL);
}
