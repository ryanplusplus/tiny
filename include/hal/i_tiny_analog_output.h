/*!
 * @file
 * @brief
 */

#ifndef i_tiny_analog_output_h
#define i_tiny_analog_output_h

#include <stdint.h>

typedef uint16_t tiny_analog_output_counts_t;

struct i_tiny_analog_output_api_t;

typedef struct {
  const struct i_tiny_analog_output_api_t* api;
} i_tiny_analog_output_t;

typedef struct i_tiny_analog_output_api_t {
  /*!
   * Writes the analog output.
   */
  void (*write)(i_tiny_analog_output_t* self, tiny_analog_output_counts_t counts);
} i_tiny_analog_output_api_t;

#define tiny_analog_output_write(self, counts) \
  (self)->api->write((self), (counts))

#endif
