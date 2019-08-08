/*!
 * @file
 * @brief Simple SPI interface. Assumes chip select is managed by the client.
 */

#ifndef i_tiny_spi_h
#define i_tiny_spi_h

#include <stdint.h>
#include "i_tiny_event.h"

typedef struct {
  uint8_t byte;
} tiny_spi_on_read_complete_args_t;

struct i_tiny_spi_api_t;

typedef struct {
  const struct i_tiny_spi_api_t* api;
} i_tiny_spi_t;

typedef struct i_tiny_spi_api_t {
  /*!
   * Writes a byte.
   */
  void (*write)(i_tiny_spi_t* self, uint8_t byte);

  /*!
   * Reads a byte.
   */
  void (*read)(i_tiny_spi_t* self);

  /*!
   * Event raised when a byte is finished being written. Clients must assume
   * that this is raised from an interrupt.
   */
  i_tiny_event_t* (*on_write_complete)(i_tiny_spi_t* self);

  /*!
   * Event raised when a byte is read. Clients must assume that this is raised
   * from an interrupt.
   */
  i_tiny_event_t* (*on_read_complete)(i_tiny_spi_t* self);
} i_tiny_spi_api_t;

#define tiny_spi_write(self, byte) \
  (self)->api->write((self), (byte))

#define tiny_spi_read(self) \
  (self)->api->read((self))

#define tiny_spi_on_write_complete(self) \
  (self)->api->on_write_complete((self))

#define tiny_spi_on_read_complete(self) \
  (self)->api->on_read_complete((self))

#endif
