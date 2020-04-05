/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_crc16_h
#define tiny_crc16_h

#include <stdint.h>

/*!
 * Calculate CRC given seed and next byte.
 */
uint16_t tiny_crc16_calculate(uint16_t seed, uint8_t byte);

#endif
