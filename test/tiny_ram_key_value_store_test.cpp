/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "tiny_ram_key_value_store.h"
#include "tiny_utils.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

// clang-format off
#define my_key_value_pair_list(pair) \
  pair(key_foo, uint8_t)             \
  pair(key_bar, uint32_t)
// clang-format on

enumerate_ram_key_value_pairs(my_key_value_pair_list);

#define _storage_type_name storage_t

generate_storage_type_for_ram_key_value_pairs(my_key_value_pair_list);

static const tiny_ram_key_value_store_key_value_pair_t my_key_value_pairs[] = {
  generate_configuration_pairs_from_ram_key_value_pairs(my_key_value_pair_list)
};

static const tiny_ram_key_value_store_configuration_t configuration = {
  my_key_value_pairs,
  element_count(my_key_value_pairs)
};

TEST_GROUP(tiny_ram_key_value_store) {
  tiny_ram_key_value_store_t self;
  tiny_event_subscription_t on_change_subscription;
  storage_t storage;

  static void value_changed(void* context, const void* _args) {
    (void)context;
    reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
    mock()
      .actualCall("value_changed")
      .withParameter("key", args->key)
      .withParameter("value", *(const uint32_t*)args->value);
  }

  void setup() {
    memset(&storage, 0xA5, sizeof(storage));
    tiny_event_subscription_init(&on_change_subscription, NULL, value_changed);
    tiny_ram_key_value_store_init(&self, &configuration, &storage);
  }
};

TEST(tiny_ram_key_value_store, should_contain_only_configured_keys) {
  CHECK_TRUE(tiny_key_value_store_contains(&self.interface, key_foo));
  CHECK_TRUE(tiny_key_value_store_contains(&self.interface, key_bar));
  CHECK_FALSE(tiny_key_value_store_contains(&self.interface, 2));
  CHECK_FALSE(tiny_key_value_store_contains(&self.interface, 0xFF));
}

TEST(tiny_ram_key_value_store, should_give_the_size_of_contained_values) {
  CHECK_EQUAL(sizeof(uint8_t), tiny_key_value_store_size(&self.interface, key_foo));
  CHECK_EQUAL(sizeof(uint32_t), tiny_key_value_store_size(&self.interface, key_bar));
}

TEST(tiny_ram_key_value_store, should_initialize_all_values_to_zero) {
  uint8_t foo;
  tiny_key_value_store_read(&self.interface, key_foo, &foo);
  CHECK_EQUAL(0, foo);

  uint32_t bar;
  tiny_key_value_store_read(&self.interface, key_bar, &bar);
  CHECK_EQUAL(0, bar);
}

TEST(tiny_ram_key_value_store, should_allow_items_to_be_written_and_read) {
  uint8_t foo_expected = 0xAB;
  tiny_key_value_store_write(&self.interface, key_foo, &foo_expected);
  uint8_t foo_actual;
  tiny_key_value_store_read(&self.interface, key_foo, &foo_actual);
  CHECK_EQUAL(foo_expected, foo_actual);

  uint32_t bar_expected = 0x12345678;
  tiny_key_value_store_write(&self.interface, key_bar, &bar_expected);
  uint32_t bar_actual;
  tiny_key_value_store_read(&self.interface, key_bar, &bar_actual);
  CHECK_EQUAL(bar_expected, bar_actual);
}

TEST(tiny_ram_key_value_store, should_raise_on_change_event_when_a_value_changes) {
  tiny_event_subscribe(
    tiny_key_value_store_on_change(&self.interface),
    &on_change_subscription);

  mock()
    .expectOneCall("value_changed")
    .withParameter("key", key_bar)
    .withParameter("value", 0x12345678);
  uint32_t bar = 0x12345678;
  tiny_key_value_store_write(&self.interface, key_bar, &bar);
}
