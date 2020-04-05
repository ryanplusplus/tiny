/*!
 * @file
 * @brief
 */

#ifndef i_tiny_analog_input_group_h
#define i_tiny_analog_input_group_h

#include <stdint.h>
#include "i_tiny_analog_input.h"

typedef uint8_t tiny_analog_input_channel_t;

struct i_tiny_analog_input_group_api_t;

typedef struct {
  const struct i_tiny_analog_input_group_api_t* api;
} i_tiny_analog_input_group_t;

typedef struct i_tiny_analog_input_group_api_t {
  /*!
   * Reads the analog input channel.
   */
  tiny_analog_input_counts_t (*read)(i_tiny_analog_input_group_t* self, tiny_analog_input_channel_t channel);
} i_tiny_analog_input_group_api_t;

#define tiny_analog_input_group_read(self, channel) \
  (self)->api->read((self), (channel))

#endif
