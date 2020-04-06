/*!
 * @file
 * @brief
 */

#ifndef i_tiny_i2c_h
#define i_tiny_i2c_h

#include <stdint.h>
#include <stdbool.h>

struct i_tiny_i2c_api_t;

typedef struct {
  const struct i_tiny_i2c_api_t* api;
} i_tiny_i2c_t;

typedef struct i_tiny_i2c_api_t {
  /*!
   * Writes bytes from a buffer to the specified address. The stop
   * condition can be omitted in order to allow for a repeated start
   * by setting prepare_for_restart.
   *
   * Returns true if the transaction succeeded and false otherwise.
   */
  bool (*write)(
    i_tiny_i2c_t* self,
    uint8_t address,
    bool prepare_for_restart,
    const uint8_t* buffer,
    uint8_t buffer_size);

  /*!
   * Read bytes into a buffer from the specified address. The stop
   * condition can be omitted in order to allow for a repeated start
   * by setting prepare_for_restart.
   *
   * Returns true if the transaction succeeded and false otherwise.
   */
  bool (*read)(
    i_tiny_i2c_t* self,
    uint8_t address,
    bool prepare_for_restart,
    uint8_t* buffer,
    uint8_t buffer_size);

  /*!
   * Resets the bus.
   */
  void (*reset)(i_tiny_i2c_t* self);
} i_tiny_i2c_api_t;

#define tiny_i2c_write(self, address, prepare_for_restart, buffer, buffer_size) \
  (self)->api->write((self), (address), (prepare_for_restart), (buffer), (buffer_size))

#define tiny_i2c_read(self, address, prepare_for_restart, buffer, buffer_size) \
  (self)->api->read((self), (address), (prepare_for_restart), (buffer), (buffer_size))

#define tiny_i2c_reset(self) \
  (self)->api->reset((self))

#endif
