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

  void should_contain_key(tiny_key_value_store_key_t key) {
    CHECK_TRUE(tiny_key_value_store_contains(&self.interface, key));
  }

  void should_not_contain_key(tiny_key_value_store_key_t key) {
    CHECK_FALSE(tiny_key_value_store_contains(&self.interface, key));
  }

  void value_for_key_should_have_size(tiny_key_value_store_key_t key, uint8_t size) {
    CHECK_EQUAL(size, tiny_key_value_store_size(&self.interface, key));
  }

  void key_should_have_value(tiny_key_value_store_key_t key, uint8_t expected) {
    uint8_t actual;
    tiny_key_value_store_read(&self.interface, key, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void key_should_have_value(tiny_key_value_store_key_t key, uint32_t expected) {
    uint32_t actual;
    tiny_key_value_store_read(&self.interface, key, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void after_key_is_written_with(tiny_key_value_store_key_t key, uint8_t value) {
    tiny_key_value_store_write(&self.interface, key, &value);
  }

  void after_key_is_written_with(tiny_key_value_store_key_t key, uint32_t value) {
    tiny_key_value_store_write(&self.interface, key, &value);
  }

  void given_that_an_on_change_subscription_is_active() {
    tiny_event_subscribe(
      tiny_key_value_store_on_change(&self.interface),
      &on_change_subscription);
  }

  void an_on_change_publication_should_be_received(tiny_key_value_store_key_t key, uint32_t value) {
    mock()
      .expectOneCall("value_changed")
      .withParameter("key", key)
      .withParameter("value", value);
  }
};

TEST(tiny_ram_key_value_store, should_contain_only_configured_keys) {
  should_contain_key(key_foo);
  should_contain_key(key_bar);
  should_not_contain_key(2);
  should_not_contain_key(0xFF);
}

TEST(tiny_ram_key_value_store, should_give_the_size_of_contained_values) {
  value_for_key_should_have_size(key_foo, sizeof(uint8_t));
  value_for_key_should_have_size(key_bar, sizeof(uint32_t));
}

TEST(tiny_ram_key_value_store, should_initialize_all_values_to_zero) {
  key_should_have_value(key_foo, (uint8_t)0);
  key_should_have_value(key_bar, (uint32_t)0);
}

TEST(tiny_ram_key_value_store, should_allow_items_to_be_written_and_read) {
  after_key_is_written_with(key_foo, (uint8_t)0xAB);
  key_should_have_value(key_foo, (uint8_t)0xAB);

  after_key_is_written_with(key_bar, (uint32_t)0x12345678);
  key_should_have_value(key_bar, (uint32_t)0x12345678);
}

TEST(tiny_ram_key_value_store, should_raise_on_change_event_when_a_value_changes) {
  given_that_an_on_change_subscription_is_active();
  an_on_change_publication_should_be_received(key_bar, 0x12345678);
  after_key_is_written_with(key_bar, (uint32_t)0x12345678);
}
