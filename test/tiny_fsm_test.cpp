/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_fsm.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#define then

TEST_GROUP(tiny_fsm) {
  tiny_fsm_t fsm;

  enum {
    signal_1 = tiny_fsm_user_signal_start,
    signal_2
  };

  void setup() {
    mock().strictOrder();
  }

  static void state_a(tiny_fsm_t * fsm, tiny_fsm_signal_t signal, const void* data) {
    mock()
      .actualCall("state_a")
      .withParameter("fsm", fsm)
      .withParameter("signal", signal)
      .withParameter("data", data);
  }

  static void state_b(tiny_fsm_t * fsm, tiny_fsm_signal_t signal, const void* data) {
    mock()
      .actualCall("state_b")
      .withParameter("fsm", fsm)
      .withParameter("signal", signal)
      .withParameter("data", data);
  }

  void signal_should_be_sent_to_state(tiny_fsm_state_t state, tiny_fsm_signal_t signal, const void* data) {
    (state == state_a ? mock().expectOneCall("state_a") : mock().expectOneCall("state_b"))
      .withParameter("fsm", &fsm)
      .withParameter("signal", signal)
      .withParameter("data", data);
  }

  void when_the_fsm_is_initialized_with_state(tiny_fsm_state_t state) {
    tiny_fsm_init(&fsm, state);
  }

  void given_that_the_fsm_has_been_initialized_with_state(tiny_fsm_state_t state) {
    mock().disable();
    when_the_fsm_is_initialized_with_state(state);
    mock().enable();
  }

  void when_the_fsm_is_transitioned_to(tiny_fsm_state_t state) {
    tiny_fsm_transition(&fsm, state);
  }

  void given_that_the_fsm_has_been_transitioned_to(tiny_fsm_state_t state) {
    mock().disable();
    when_the_fsm_is_transitioned_to(state);
    mock().enable();
  }

  void when_signal_is_sent(tiny_fsm_signal_t signal, const void* data) {
    tiny_fsm_send_signal(&fsm, signal, data);
  }
};

TEST(tiny_fsm, should_send_entry_to_the_initial_state) {
  signal_should_be_sent_to_state(state_a, tiny_fsm_entry, NULL);
  when_the_fsm_is_initialized_with_state(state_a);
}

TEST(tiny_fsm, should_send_exit_to_current_state_then_entry_to_new_state_during_transition) {
  given_that_the_fsm_has_been_initialized_with_state(state_a);

  signal_should_be_sent_to_state(state_a, tiny_fsm_exit, NULL);
  then signal_should_be_sent_to_state(state_b, tiny_fsm_entry, NULL);
  when_the_fsm_is_transitioned_to(state_b);
}

TEST(tiny_fsm, should_send_signals_to_the_current_state) {
  given_that_the_fsm_has_been_initialized_with_state(state_a);
  signal_should_be_sent_to_state(state_a, signal_1, (const void*)0x1234);
  when_signal_is_sent(signal_1, (const void*)0x1234);

  given_that_the_fsm_has_been_transitioned_to(state_b);
  signal_should_be_sent_to_state(state_b, signal_2, (const void*)0x4567);
  when_signal_is_sent(signal_2, (const void*)0x4567);
}
