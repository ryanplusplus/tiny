/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef i_tiny_spi_h
#define i_tiny_spi_h

#include <stdint.h>

struct i_tiny_spi_api_t;

typedef struct {
  const struct i_tiny_spi_api_t* api;
} i_tiny_spi_t;

typedef struct i_tiny_spi_api_t {
  /*!
   * Writes and reads a byte.
   */
  uint8_t (*transfer)(i_tiny_spi_t* self, uint8_t byte);
} i_tiny_spi_api_t;

#define tiny_spi_transfer(self, byte) \
  (self)->api->transfer((self), (byte), (callback), (context))

#endif
