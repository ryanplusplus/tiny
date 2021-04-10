/*!
 * @file
 * @brief
 */

#include "tiny_crc16.h"

uint16_t tiny_crc16_byte(uint16_t seed, uint8_t byte)
{
  uint16_t crc = seed;
  byte = crc >> 8 ^ byte;
  byte ^= byte >> 4;
  return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
}

uint16_t tiny_crc16_block(uint16_t seed, const uint8_t* bytes, size_t byte_count)
{
  uint16_t crc = seed;
  for(size_t i = 0; i < byte_count; i++) {
    crc = tiny_crc16_byte(crc, bytes[i]);
  }
  return crc;
}
