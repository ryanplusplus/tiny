/*!
 * @file
 * @brief
 */

#ifndef i_tiny_i2c_h
#define i_tiny_i2c_h

#include <stdint.h>
#include <stdbool.h>

typedef void (*tiny_i2c_callback_t)(void* context, bool success);

struct i_tiny_i2c_api_t;

typedef struct {
  const struct i_tiny_i2c_api_t* api;
} i_tiny_i2c_t;

typedef struct i_tiny_i2c_api_t {
  /*!
   * Writes bytes from a buffer to the specified address
   */
  void (*write)(
    i_tiny_i2c_t* self,
    uint8_t address,
    const uint8_t* buffer,
    uint8_t buffer_size,
    tiny_i2c_callback_t callback,
    void* context);

  /*!
   * Writes bytes into a buffer from the specified address
   */
  void (*read)(
    i_tiny_i2c_t* self,
    uint8_t address,
    uint8_t* buffer,
    uint8_t buffer_size,
    tiny_i2c_callback_t callback,
    void* context);

  /*!
   * Resets the bus'
   */
  void (*reset)(i_tiny_i2c_t* self);
} i_tiny_i2c_api_t;

#define tiny_i2c_write(self, address, buffer, buffer_size, callback, context) \
  (self)->api->write((self), (address), (buffer), (buffer_size), (callback), (context))

#define tiny_i2c_read(self, address, buffer, buffer_size, callback, context) \
  (self)->api->read((self), (address), (buffer), (buffer_size), (callback), (context))

#define tiny_i2c_reset(self) \
  (self)->api->reset((self))

#endif
