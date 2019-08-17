/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "tiny_hsm.h"

#define top NULL

static tiny_hsm_state_t parent_of(tiny_hsm_t* self, tiny_hsm_state_t child) {
  for(uint8_t i = 0; i < self->configuration->state_count; i++) {
    if(self->configuration->states[i].state == child) {
      return self->configuration->states[i].parent;
    }
  }

  return top;
}

static uint8_t distance_between(tiny_hsm_t* self, tiny_hsm_state_t child, tiny_hsm_state_t parent) {
  uint8_t distance = 0;
  tiny_hsm_state_t current = child;

  while(current != parent) {
    distance++;
    current = parent_of(self, current);
  }

  return distance;
}

static tiny_hsm_state_t nth_parent(tiny_hsm_t* self, tiny_hsm_state_t state, uint8_t n) {
  tiny_hsm_state_t current = state;

  for(uint8_t i = 0; i < n; i++) {
    current = parent_of(self, current);
  }

  return current;
}

static void send_entries(tiny_hsm_t* self, tiny_hsm_state_t after, tiny_hsm_state_t to) {
  if(after == to) {
    return;
  }

  for(uint8_t n = distance_between(self, to, after) - 1; n > 0; n--) {
    nth_parent(self, to, n)(self, tiny_hsm_signal_entry, NULL);
  }

  to(self, tiny_hsm_signal_entry, NULL);
}

static void send_exits(tiny_hsm_t* self, tiny_hsm_state_t from, tiny_hsm_state_t before) {
  tiny_hsm_state_t current = from;

  while(current != before) {
    current(self, tiny_hsm_signal_exit, NULL);
    current = parent_of(self, current);
  }
}

static tiny_hsm_state_t nearest_common_ancestor_of(tiny_hsm_t* self, tiny_hsm_state_t a, tiny_hsm_state_t b) {
  while(a != top) {
    tiny_hsm_state_t bb = b;

    while(bb != top) {
      if(a == bb) {
        return a;
      }

      bb = parent_of(self, bb);
    }

    a = parent_of(self, a);
  }

  return top;
}

void tiny_hsm_init(
  tiny_hsm_t* self,
  const tiny_hsm_configuration_t* configuration,
  tiny_hsm_state_t initial) {
  self->configuration = configuration;
  self->current = initial;

  send_entries(self, NULL, initial);
}

void tiny_hsm_send_signal(tiny_hsm_t* self, tiny_hsm_signal_t signal, const void* data) {
  tiny_hsm_state_t current = self->current;

  while(current != top) {
    if(current(self, signal, data) == tiny_hsm_result_signal_consumed) {
      return;
    }
    current = parent_of(self, current);
  }
}

void tiny_hsm_transition(tiny_hsm_t* self, tiny_hsm_state_t target) {
  if(self->current == target) {
    self->current(self, tiny_hsm_signal_exit, NULL);
    self->current(self, tiny_hsm_signal_entry, NULL);
  }
  else {
    tiny_hsm_state_t nearest_common_ancestor = nearest_common_ancestor_of(self, self->current, target);
    send_exits(self, self->current, nearest_common_ancestor);
    self->current = target;
    send_entries(self, nearest_common_ancestor, target);
  }
}
