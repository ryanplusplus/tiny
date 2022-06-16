/*!
 * @file
 * @brief
 */

extern "C" {
#include "tiny_crc16.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(tiny_crc16){};

TEST(tiny_crc16, should_seem_to_work_for_bytes)
{
  CHECK_EQUAL(0x1021, tiny_crc16_byte(0x0000, 0x01));
}

TEST(tiny_crc16, should_seem_to_work_for_blocks)
{
  uint8_t block[] = { 1, 2, 3 };
  CHECK_EQUAL(0x6131, tiny_crc16_block(0x0000, block, sizeof(block)));
}
