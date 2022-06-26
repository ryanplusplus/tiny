/*!
 * @file
 * @brief Abstract group of analog outputs.
 */

#ifndef i_tiny_analog_output_group_h
#define i_tiny_analog_output_group_h

#include <stdint.h>
#include "i_tiny_analog_output.h"

typedef uint8_t tiny_analog_output_channel_t;

struct i_tiny_analog_output_group_api_t;

typedef struct {
  const struct i_tiny_analog_output_group_api_t* api;
} i_tiny_analog_output_group_t;

typedef struct i_tiny_analog_output_group_api_t {
  /*!
   * Writes the analog output channel.
   *
   * The output counts are normalized to the full scale range of Counts.
   */
  void (*write)(
    i_tiny_analog_output_group_t* self,
    tiny_analog_output_channel_t channel,
    tiny_analog_output_counts_t counts);
} i_tiny_analog_output_group_api_t;

#define tiny_analog_output_group_write(self, channel, counts) \
  (self)->api->write((self), (channel), (counts))

#endif
