/*!
 * @file
 * @brief Abstraction for performing a synchronous SPI transfer.
 *
 * Assumes chip select is managed by the client.
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
   * Perform a simultaneous write/read. If not reading or writing, the corresponding
   * buffer can be left NULL.
   */
  void (*transfer)(
    i_tiny_spi_t* self,
    const void* write_buffer,
    void* read_buffer,
    uint16_t buffer_size);
} i_tiny_spi_api_t;

#define tiny_spi_transfer(self, write_buffer, read_buffer, buffer_size) \
  (self)->api->transfer((self), (write_buffer), (read_buffer), (buffer_size))

#endif
