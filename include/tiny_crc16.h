/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_crc16_h
#define tiny_crc16_h

#include <stddef.h>
#include <stdint.h>

/*!
 * Calculate CRC given seed and next byte.
 */
uint16_t tiny_crc16_byte(uint16_t seed, uint8_t byte);

/*!
 * Calculate the CRC of a block given the seed.
 */
uint16_t tiny_crc16_block(uint16_t seed, const uint8_t* bytes, size_t byte_count);

#endif
