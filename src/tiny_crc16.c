/*!
 * @file
 * @brief
 */

#include "tiny_crc16.h"

uint16_t tiny_crc16_calculate(uint16_t seed, uint8_t byte) {
  uint16_t crc = seed;
  byte = crc >> 8 ^ byte;
  byte ^= byte >> 4;
  return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
}
