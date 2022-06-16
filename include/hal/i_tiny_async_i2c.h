/*!
 * @file
 * @brief Abstract asynchronous I2C peripheral.
 */

#ifndef i_tiny_async_i2c_h
#define i_tiny_async_i2c_h

#include <stdint.h>
#include <stdbool.h>

typedef void (*tiny_async_i2c_callback_t)(void* context, bool success);

struct i_tiny_async_i2c_api_t;

typedef struct {
  const struct i_tiny_async_i2c_api_t* api;
} i_tiny_async_i2c_t;

typedef struct i_tiny_async_i2c_api_t {
  /*!
   * Writes bytes from a buffer to the specified address. The stop
   * condition can be omitted in order to allow for a repeated start
   * by setting prepare_for_restart.
   *
   * Clients should assume that the callback could be invoked from
   * the interrupt context.
   */
  void (*write)(
    i_tiny_async_i2c_t* self,
    uint8_t address,
    bool prepare_for_restart,
    const void* buffer,
    uint16_t buffer_size,
    void* context,
    tiny_async_i2c_callback_t callback);

  /*!
   * Read bytes into a buffer from the specified address. The stop
   * condition can be omitted in order to allow for a repeated start
   * by setting prepare_for_restart.
   *
   * Clients should assume that the callback could be invoked from
   * the interrupt context.
   */
  void (*read)(
    i_tiny_async_i2c_t* self,
    uint8_t address,
    bool prepare_for_restart,
    void* buffer,
    uint16_t buffer_size,
    void* context,
    tiny_async_i2c_callback_t callback);

  /*!
   * Resets the bus.
   */
  void (*reset)(i_tiny_async_i2c_t* self);
} i_tiny_async_i2c_api_t;

#define tiny_async_i2c_write(self, address, prepare_for_restart, buffer, buffer_size, context, callback) \
  (self)->api->write((self), (address), (prepare_for_restart), (buffer), (buffer_size), (context), (callback))

#define tiny_async_i2c_read(self, address, prepare_for_restart, buffer, buffer_size, context, callback) \
  (self)->api->read((self), (address), (prepare_for_restart), (buffer), (buffer_size), (context), (callback))

#define tiny_async_i2c_reset(self) \
  (self)->api->reset((self))

#endif
