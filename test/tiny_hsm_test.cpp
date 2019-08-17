/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_hsm.h"
#include "tiny_utils.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/*!
 * +---------------------+ +---+
 * | A                   | | F |
 * | +-------+ +-------+ | +---+
 * | | B     | | D     | |
 * | | +---+ | | +---+ | |
 * | | | C | | | | E | | |
 * | | +---+ | | +---+ | |
 * | +-------+ +-------+ |
 * +---------------------+
 */

enum {
  state_a,
  state_b,
  state_c,
  state_d,
  state_e,
  state_f
};
typedef uint8_t state_t;

#define state_function(_name)                                                                      \
  static tiny_hsm_result_t _##_name(tiny_hsm_t* hsm, tiny_hsm_signal_t signal, const void* data) { \
    return mock()                                                                                  \
      .actualCall(#_name)                                                                          \
      .withParameter("hsm", hsm)                                                                   \
      .withParameter("signal", signal)                                                             \
      .withParameter("data", data)                                                                 \
      .returnIntValue();                                                                           \
  }

state_function(state_a);
state_function(state_b);
state_function(state_c);
state_function(state_d);
state_function(state_e);
state_function(state_f);

static const char* state_names[] = {
  "state_a",
  "state_b",
  "state_c",
  "state_d",
  "state_e",
  "state_f"
};

static const tiny_hsm_state_t states[] = {
  _state_a,
  _state_b,
  _state_c,
  _state_d,
  _state_e,
  _state_f
};

static const tiny_hsm_state_descriptor_t descriptors[] = {
  { _state_a, NULL },
  { _state_b, _state_a },
  { _state_c, _state_b },
  { _state_d, _state_a },
  { _state_e, _state_d },
  { _state_f, NULL }
};

static const tiny_hsm_configuration_t configuration = {
  descriptors,
  element_count(descriptors)
};

TEST_GROUP(tiny_hsm) {
  tiny_hsm_t hsm;

  enum {
    signal_1 = tiny_hsm_signal_user_start,
    signal_2
  };

  enum {
    consumed = tiny_hsm_result_signal_consumed,
    deferred = tiny_hsm_result_signal_deferred
  };

  void setup() {
    mock().strictOrder();
  }

  void signal_should_be_sent_to_state(
    state_t state,
    tiny_hsm_signal_t signal,
    const void* data = NULL,
    tiny_hsm_result_t result = tiny_hsm_result_signal_consumed) {
    mock()
      .expectOneCall(state_names[state])
      .withParameter("hsm", &hsm)
      .withParameter("signal", signal)
      .withParameter("data", data)
      .andReturnValue(result);
  }

  void when_the_hsm_is_initialized_with_state(state_t state) {
    tiny_hsm_init(&hsm, &configuration, states[state]);
  }

  void given_that_the_hsm_has_been_initialized_with_state(state_t state) {
    mock().disable();
    when_the_hsm_is_initialized_with_state(state);
    mock().enable();
  }

  void when_the_hsm_is_transitioned_to(state_t state) {
    tiny_hsm_transition(&hsm, states[state]);
  }

  void given_that_the_hsm_has_been_transitioned_to(state_t state) {
    mock().disable();
    when_the_hsm_is_transitioned_to(state);
    mock().enable();
  }

  void when_signal_is_sent(tiny_hsm_signal_t signal, const void* data) {
    tiny_hsm_send_signal(&hsm, signal, data);
  }
};

TEST(tiny_hsm, should_enter_the_initial_state_when_it_is_top_level) {
  signal_should_be_sent_to_state(state_a, tiny_hsm_signal_entry);
  when_the_hsm_is_initialized_with_state(state_a);
}

TEST(tiny_hsm, should_enter_the_initial_state_and_its_parents) {
  signal_should_be_sent_to_state(state_a, tiny_hsm_signal_entry);
  signal_should_be_sent_to_state(state_b, tiny_hsm_signal_entry);
  signal_should_be_sent_to_state(state_c, tiny_hsm_signal_entry);
  when_the_hsm_is_initialized_with_state(state_c);
}

TEST(tiny_hsm, should_send_a_signal_to_the_current_state) {
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_1, (void*)0x1234);
  when_signal_is_sent(signal_1, (void*)0x1234);
}

TEST(tiny_hsm, should_propagate_signals_when_they_are_not_consumed) {
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_2, (void*)0x5678, deferred);
  signal_should_be_sent_to_state(state_b, signal_2, (void*)0x5678, deferred);
  signal_should_be_sent_to_state(state_a, signal_2, (void*)0x5678, deferred);
  when_signal_is_sent(signal_2, (void*)0x5678);
}

TEST(tiny_hsm, should_stop_propagating_a_signal_once_consumed) {
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_2, (void*)0x5678, deferred);
  signal_should_be_sent_to_state(state_b, signal_2, (void*)0x5678, consumed);
  when_signal_is_sent(signal_2, (void*)0x5678);
}

TEST(tiny_hsm, should_exit_and_reenter_state_during_a_self_transition) {
  given_that_the_hsm_has_been_initialized_with_state(state_d);
  signal_should_be_sent_to_state(state_d, tiny_hsm_signal_exit);
  signal_should_be_sent_to_state(state_d, tiny_hsm_signal_entry);
  when_the_hsm_is_transitioned_to(state_d);
}

TEST(tiny_hsm, should_transition_between_peers) {
  given_that_the_hsm_has_been_initialized_with_state(state_b);
  signal_should_be_sent_to_state(state_b, tiny_hsm_signal_exit);
  signal_should_be_sent_to_state(state_d, tiny_hsm_signal_entry);
  when_the_hsm_is_transitioned_to(state_d);
}

TEST(tiny_hsm, should_transition_to_parents) {
  given_that_the_hsm_has_been_initialized_with_state(state_e);
  signal_should_be_sent_to_state(state_e, tiny_hsm_signal_exit);
  signal_should_be_sent_to_state(state_d, tiny_hsm_signal_exit);
  when_the_hsm_is_transitioned_to(state_a);
}

TEST(tiny_hsm, should_transition_between_cousins) {
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, tiny_hsm_signal_exit);
  signal_should_be_sent_to_state(state_b, tiny_hsm_signal_exit);
  signal_should_be_sent_to_state(state_d, tiny_hsm_signal_entry);
  signal_should_be_sent_to_state(state_e, tiny_hsm_signal_entry);
  when_the_hsm_is_transitioned_to(state_e);
}

TEST(tiny_hsm, should_update_state_during_a_transition) {
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  given_that_the_hsm_has_been_transitioned_to(state_e);
  signal_should_be_sent_to_state(state_e, signal_1, (void*)0x1234);
  when_signal_is_sent(signal_1, (void*)0x1234);
}
