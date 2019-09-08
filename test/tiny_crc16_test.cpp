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

TEST(tiny_crc16, should_seem_to_work) {
  CHECK_EQUAL(0x1021, tiny_crc16_calculate(0x0000, 0x01));
}
