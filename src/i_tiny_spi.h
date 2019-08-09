/*!
 * @file
 * @brief Simple SPI interface. Assumes chip select is managed by the client.
 */

#ifndef i_tiny_spi_h
#define i_tiny_spi_h

#include <stdint.h>
#include "i_tiny_event.h"

typedef void (*tiny_spi_write_callback_t)(void* context);
typedef void (*tiny_spi_read_callback_t)(void* context, uint8_t byte);

struct i_tiny_spi_api_t;

typedef struct {
  const struct i_tiny_spi_api_t* api;
} i_tiny_spi_t;

typedef struct i_tiny_spi_api_t {
  /*!
   * Writes a byte. Clients should assume that the callback is raised from an
   * interrupt.
   */
  void (*write)(i_tiny_spi_t* self, uint8_t byte, tiny_spi_write_callback_t callback, void* context);

  /*!
   * Reads a byte. Clients should assume that the callback is raised from an
   * interrupt.
   */
  void (*read)(i_tiny_spi_t* self, tiny_spi_read_callback_t callback, void* context);
} i_tiny_spi_api_t;

#define tiny_spi_write(self, byte, callback, context) \
  (self)->api->write((self), (byte), (callback), (context))

#define tiny_spi_read(self, callback, context) \
  (self)->api->read((self), (callback), (context))

#endif
