/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef i_tiny_async_spi_h
#define i_tiny_async_spi_h

#include <stdint.h>

typedef void (*tiny_async_spi_callback_t)(void* context, uint8_t byte);

struct i_tiny_async_spi_api_t;

typedef struct {
  const struct i_tiny_async_spi_api_t* api;
} i_tiny_async_spi_t;

typedef struct i_tiny_async_spi_api_t {
  /*!
   * Writes and reads a byte. Clients should assume that the callback is raised
   * from an interrupt.
   */
  void (*transfer)(i_tiny_async_spi_t* self, uint8_t byte, tiny_async_spi_callback_t callback, void* context);
} i_tiny_async_spi_api_t;

#define tiny_async_spi_transfer(self, byte, callback, context) \
  (self)->api->transfer((self), (byte), (callback), (context))

#endif
