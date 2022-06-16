/*!
 * @file
 * @brief Abstract analog input.
 */

#ifndef i_tiny_analog_input_h
#define i_tiny_analog_input_h

#include <stdint.h>

typedef uint16_t tiny_analog_input_counts_t;

struct i_tiny_analog_input_api_t;

typedef struct {
  const struct i_tiny_analog_input_api_t* api;
} i_tiny_analog_input_t;

typedef struct i_tiny_analog_input_api_t {
  /*!
   * Reads the analog input.
   */
  tiny_analog_input_counts_t (*read)(i_tiny_analog_input_t* self);
} i_tiny_analog_input_api_t;

#define tiny_analog_input_read(self) \
  (self)->api->read((self))

#endif
