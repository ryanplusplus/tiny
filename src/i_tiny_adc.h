/*!
 * @file
 * @brief
 */

#ifndef i_tiny_adc_h
#define i_tiny_adc_h

#include <stdint.h>

typedef uint16_t tiny_adc_counts_t;

struct i_tiny_adc_api_t;

typedef struct
{
  const struct i_tiny_adc_api_t* api;
} i_tiny_adc_t;

typedef struct i_tiny_adc_api_t {
  /*!
   * Reads the ADC.
   */
  tiny_adc_counts_t (*read)(i_tiny_adc_t* self);
} i_tiny_adc_api_t;

#define tiny_adc_read(self) \
  (self)->api->read((self))

#endif
