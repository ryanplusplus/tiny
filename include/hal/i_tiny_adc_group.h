/*!
 * @file
 * @brief
 */

#ifndef i_tiny_adc_group_h
#define i_tiny_adc_group_h

#include <stdint.h>
#include "i_tiny_adc.h"

typedef uint8_t tiny_adc_channel_t;

struct i_tiny_adc_group_api_t;

typedef struct {
  const struct i_tiny_adc_group_api_t* api;
} i_tiny_adc_group_t;

typedef struct i_tiny_adc_group_api_t {
  /*!
   * Reads the ADC channel.
   */
  tiny_adc_counts_t (*read)(i_tiny_adc_group_t* self, tiny_adc_channel_t channel);
} i_tiny_adc_group_api_t;

#define tiny_adc_group_read(self, channel) \
  (self)->api->read((self), (channel))

#endif
