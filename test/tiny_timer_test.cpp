/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_timer.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "double/tiny_time_source_double.h"

static tiny_timer_ticks_t restart_ticks;

TEST_GROUP(tiny_timer) {
  tiny_timer_group_t group;

  tiny_timer_t timer_1;
  tiny_timer_t timer_2;
  tiny_timer_t timer_3;
  tiny_timer_t timer_with_restart;

  tiny_time_source_double_t time_source;

  void setup() {
    mock().strictOrder();

    tiny_time_source_double_init(&time_source);
    tiny_time_source_double_set_ticks(&time_source, 1234);

    tiny_timer_group_init(&group, &time_source.interface);
  }

  static void callback(tiny_timer_group_t * group, void* context) {
    mock()
      .actualCall("callback")
      .withParameter("group", group)
      .withParameter("context", context);
  }

  static void callback_with_restart(tiny_timer_group_t * group, void* context) {
    callback(group, context);
    tiny_timer_start(group, (tiny_timer_t*)context, restart_ticks, callback_with_restart, context);
  }

  void after_timer_with_restart_is_started(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    restart_ticks = ticks;
    tiny_timer_start(&group, timer, ticks, callback_with_restart, timer);
  }

  void given_that_timer_with_restart_has_been_started(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    after_timer_with_restart_is_started(timer, ticks);
  }

  void after_timer_is_started(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    tiny_timer_start(&group, timer, ticks, callback, timer);
  }

  void given_that_timer_has_been_started(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    after_timer_is_started(timer, ticks);
  }

  void after_timer_is_stopped(tiny_timer_t * timer) {
    tiny_timer_stop(&group, timer);
  }

  void given_that_timer_has_been_stopped(tiny_timer_t * timer) {
    after_timer_is_stopped(timer);
  }

  void should_invoke_timer_callback(tiny_timer_t * timer) {
    mock()
      .expectOneCall("callback")
      .withParameter("group", &group)
      .withParameter("context", timer);
  }

  void after(tiny_time_source_ticks_t ticks) {
    tiny_time_source_double_tick(&time_source, ticks);
  }

  void after_the_group_is_run() {
    tiny_timer_group_run(&group);
  }

  void after_time_passes_and_the_group_is_run(tiny_time_source_ticks_t ticks) {
    after(ticks);
    after_the_group_is_run();
  }

  void given_that_time_has_passed_and_the_group_has_been_run(tiny_time_source_ticks_t ticks) {
    mock().disable();
    after_time_passes_and_the_group_is_run(ticks);
    mock().enable();
  }

  void should_invoke_timer_callback_after(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    after_time_passes_and_the_group_is_run(ticks - 1);
    should_invoke_timer_callback(timer);
    after_time_passes_and_the_group_is_run(1);
  }

  void timer_should_not_be_running(tiny_timer_t * timer) {
    CHECK_FALSE(tiny_timer_is_running(&group, timer));
  }

  void timer_should_be_running(tiny_timer_t * timer) {
    CHECK_TRUE(tiny_timer_is_running(&group, timer));
  }

  void should_indicate_that_a_callback_was_invoked_during_run() {
    mock().disable();
    CHECK_TRUE(tiny_timer_group_run(&group));
    mock().enable();
  }

  void should_indicate_that_a_callback_was_not_invoked_during_run() {
    mock().disable();
    CHECK_FALSE(tiny_timer_group_run(&group));
    mock().enable();
  }

  void remaining_timer_for_timer_should_be(tiny_timer_t * timer, tiny_timer_ticks_t ticks) {
    CHECK_EQUAL(ticks, tiny_timer_remaining_ticks(&group, timer));
  }

  void nothing_should_happen() {
  }
};

TEST(tiny_timer, should_invoke_timer_callback_on_expiration) {
  given_that_timer_has_been_started(&timer_1, 7);
  should_invoke_timer_callback_after(&timer_1, 7);
}

TEST(tiny_timer, should_invoke_timer_callback_even_if_run_after_expiration) {
  given_that_timer_has_been_started(&timer_1, 7);

  after(10);
  should_invoke_timer_callback(&timer_1);
  after_the_group_is_run();
}

TEST(tiny_timer, should_not_call_back_timer_again_after_expiration) {
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(7);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(100);
}

TEST(tiny_timer, should_allow_a_timer_to_be_stopped_prior_to_expiration) {
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(3);
  given_that_timer_has_been_stopped(&timer_1);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(tiny_timer, should_allow_a_running_timer_to_be_restarted) {
  given_that_timer_has_been_started(&timer_2, 7);
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_3, 9);
  given_that_timer_has_been_started(&timer_1, 5);

  should_invoke_timer_callback_after(&timer_1, 5);
  should_invoke_timer_callback_after(&timer_2, 2);
  should_invoke_timer_callback_after(&timer_3, 2);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(tiny_timer, should_manage_multiple_timers_simultaneously) {
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_2, 3);

  should_invoke_timer_callback_after(&timer_2, 3);
  should_invoke_timer_callback_after(&timer_1, 4);
}

TEST(tiny_timer, should_invoke_at_most_one_callback_per_run) {
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_2, 7);

  after(7);
  should_invoke_timer_callback(&timer_1);
  after_the_group_is_run();

  should_invoke_timer_callback(&timer_2);
  after_the_group_is_run();
}

TEST(tiny_timer, should_indicate_whether_a_callback_was_invoked_during_run) {
  given_that_timer_has_been_started(&timer_1, 3);
  given_that_timer_has_been_started(&timer_2, 5);

  should_indicate_that_a_callback_was_not_invoked_during_run();

  after(3);
  should_indicate_that_a_callback_was_invoked_during_run();
  should_indicate_that_a_callback_was_not_invoked_during_run();

  after(2);
  should_indicate_that_a_callback_was_invoked_during_run();
  should_indicate_that_a_callback_was_not_invoked_during_run();
}

TEST(tiny_timer, should_indicate_whether_a_timer_is_running) {
  timer_should_not_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);

  after_timer_is_started(&timer_1, 5);
  timer_should_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  timer_should_not_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);
}

TEST(tiny_timer, should_give_the_remaining_ticks_for_a_running_timer) {
  given_that_timer_has_been_started(&timer_1, 7);
  remaining_timer_for_timer_should_be(&timer_1, 7);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  remaining_timer_for_timer_should_be(&timer_1, 2);
}

TEST(tiny_timer, should_allow_a_timer_to_be_restarted_in_its_callback) {
  given_that_timer_with_restart_has_been_started(&timer_with_restart, 5);

  should_invoke_timer_callback_after(&timer_with_restart, 5);
  should_invoke_timer_callback_after(&timer_with_restart, 5);
  should_invoke_timer_callback_after(&timer_with_restart, 5);
}

TEST(tiny_timer, should_not_allow_a_timer_to_be_starved) {
  given_that_timer_with_restart_has_been_started(&timer_with_restart, 0);
  given_that_timer_has_been_started(&timer_1, 3);

  should_invoke_timer_callback(&timer_with_restart);
  after_time_passes_and_the_group_is_run(3);

  should_invoke_timer_callback(&timer_with_restart);
  after_the_group_is_run();
}
