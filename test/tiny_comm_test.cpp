/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_comm.h"
#include "tiny_crc16.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny_uart_double.h"

TEST_GROUP(tiny_comm)
{
  enum {
    stx = 0x02,
    etx = 0x03,
    dle = 0x10
  };

  enum {
    send_buffer_size = 5,
    receive_buffer_size = 10
  };

  tiny_comm_t self;
  uint8_t send_buffer[send_buffer_size];
  uint8_t receive_buffer[receive_buffer_size];

  tiny_uart_double_t uart;

  void setup()
  {
    tiny_uart_double_init(&uart);
    tiny_comm_init(
      &self,
      &uart.interface,
      send_buffer,
      sizeof(send_buffer),
      receive_buffer,
      receive_buffer_size);
  }

  void after_send_completes()
  {
    tiny_uart_double_trigger_send_complete(&uart);
  }

  void byte_should_be_sent(uint8_t byte)
  {
    mock().expectOneCall("send").onObject(&uart).withParameter("byte", byte);
  }

#define bytes_should_be_sent(...) _bytes_should_be_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _bytes_should_be_sent()
  {
    for(auto byte : { bytes... }) {
      byte_should_be_sent(byte);
    }
  }

#define crc_of(...) crc_msb<__VA_ARGS__>(), crc_lsb<__VA_ARGS__>()

  template <uint8_t... bytes>
  static constexpr uint16_t crc()
  {
    uint16_t crc = 0xFFFF;
    for(uint8_t byte : (uint8_t[]){ bytes... }) {
      crc = calculate_crc(crc, byte);
    }
    return crc;
  }

  static constexpr uint16_t calculate_crc(uint16_t seed, uint8_t byte)
  {
    uint16_t crc = seed;
    byte = crc >> 8 ^ byte;
    byte ^= byte >> 4;
    return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
  }

  template <uint8_t... bytes>
  static constexpr uint8_t crc_msb()
  {
    return crc<bytes...>() >> 8;
  }

  template <uint8_t... bytes>
  static constexpr uint8_t crc_lsb()
  {
    return crc<bytes...>() & 0xFF;
  }

#define when_payload_is_sent(...) _when_payload_is_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _when_payload_is_sent()
  {
    uint8_t payload[] = { bytes... };
    tiny_comm_send(&self.interface, payload, sizeof(payload));
  }

#define given_that_payload_has_been_sent(...) _given_that_payload_has_been_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _given_that_payload_has_been_sent()
  {
    mock().disable();
    _when_payload_is_sent<bytes...>();
    mock().enable();
  }

  void should_be_sending()
  {
    CHECK_TRUE(tiny_comm_sending(&self.interface));
  }

  void should_not_be_sending()
  {
    CHECK_FALSE(tiny_comm_sending(&self.interface));
  }

  void and_all_sends_complete()
  {
    while(tiny_uart_double_sending(&uart)) {
      tiny_uart_double_trigger_send_complete(&uart);
    }
  }

  void and_all_sends_have_completed()
  {
    mock().disable();
    and_all_sends_complete();
    mock().enable();
  }
};

TEST(tiny_comm, should_report_that_send_is_in_process_when_sending)
{
  given_that_payload_has_been_sent(11, 12, 13);
  should_be_sending();
}

TEST(tiny_comm, should_report_that_send_is_not_in_process_after_send_completes)
{
  given_that_payload_has_been_sent(11, 12, 13);
  and_all_sends_have_completed();
  should_not_be_sending();
}

TEST(tiny_comm, should_not_send_payloads_that_are_too_large)
{
  given_that_payload_has_been_sent(11, 12, 13, 14, 15, 16);
  should_not_be_sending();
}

TEST(tiny_comm, should_send_the_next_byte_only_after_previous_send_completes)
{
  byte_should_be_sent(stx);
  when_payload_is_sent(11, 12, 13);

  byte_should_be_sent(11);
  after_send_completes();

  byte_should_be_sent(12);
  after_send_completes();
}

TEST(tiny_comm, should_send_an_empty_payload)
{
  bytes_should_be_sent(stx, crc_of(), etx);
  when_payload_is_sent();
  and_all_sends_complete();
}

TEST(tiny_comm, should_send_a_non_empty_payload)
{
  bytes_should_be_sent(stx, 11, 12, 13, crc_of(11, 12, 13), etx);
  when_payload_is_sent(11, 12, 13);
  and_all_sends_complete();
}

TEST(tiny_comm, should_send_a_full_size_payload)
{
  bytes_should_be_sent(stx, 15, 14, 13, 12, 11, crc_of(15, 14, 13, 12, 11), etx);
  when_payload_is_sent(15, 14, 13, 12, 11);
  and_all_sends_complete();
}

TEST(tiny_comm, should_escape_control_characters_in_the_send_payload)
{
  bytes_should_be_sent(stx, dle, stx, dle, etx, dle, dle, crc_of(stx, etx, dle), etx);
  when_payload_is_sent(stx, etx, dle);
  and_all_sends_complete();
}

TEST(tiny_comm, should_escape_control_characters_in_the_crc)
{
  bytes_should_be_sent(stx, 0x0E, 0x65, dle, stx, dle, etx, etx);
  when_payload_is_sent(0x0E, 0x65);
  and_all_sends_complete();
}

// ignore send complete events when not sending
