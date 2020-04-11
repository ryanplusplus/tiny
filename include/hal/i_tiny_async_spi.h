/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef i_tiny_async_spi_h
#define i_tiny_async_spi_h

#include <stdint.h>

typedef void (*tiny_async_spi_callback_t)(void* context);

struct i_tiny_async_spi_api_t;

typedef struct {
  const struct i_tiny_async_spi_api_t* api;
} i_tiny_async_spi_t;

typedef struct i_tiny_async_spi_api_t {
  /*!
   * Performs a simultaneous write/read. If not reading or writing, the corresponding
   * buffer can be left NULL.
   *
   * Clients should assume that the callback is raised
   * from an interrupt.
   */
  void (*transfer)(
    i_tiny_async_spi_t* self,
    const uint8_t* write_buffer,
    uint8_t* read_buffer,
    uint16_t buffer_size,
    tiny_async_spi_callback_t callback,
    void* context);
} i_tiny_async_spi_api_t;

#define tiny_async_spi_transfer(self, write_buffer, read_buffer, buffer_size, callback, context) \
  (self)->api->transfer((self), (write_buffer), (read_buffer), (buffer_size), (callback), (context))

#endif
