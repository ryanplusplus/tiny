/*!
 * @file
 * @brief
 */

#ifndef i_tiny_digital_output_group_h
#define i_tiny_digital_output_group_h

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t tiny_digital_output_channel_t;

struct i_tiny_digital_output_group_api_t;

typedef struct {
  const struct i_tiny_digital_output_group_api_t* api;
} i_tiny_digital_output_group_t;

typedef struct i_tiny_digital_output_group_api_t {
  /*!
   * Writes the digital output channel.
   */
  void (*write)(
    i_tiny_digital_output_group_t* self,
    tiny_digital_output_channel_t channel,
    bool value);
} i_tiny_digital_output_group_api_t;

#define tiny_digital_output_group_write(self, channel, value) \
  (self)->api->write((self), (channel), (value))

#endif
