/*!
 * @file
 * @brief
 */

#ifndef i_tiny_digital_output_h
#define i_tiny_digital_output_h

#include <stdbool.h>

struct i_tiny_digital_output_api_t;

typedef struct
{
  const struct i_tiny_digital_output_api_t* api;
} i_tiny_digital_output_t;

typedef struct i_tiny_digital_output_api_t {
  /*!
   * Writes the digital output.
   */
  void (*write)(i_tiny_digital_output_t* self, bool value);
} i_tiny_digital_output_api_t;

#define tiny_digital_output_write(self, value) \
  (self)->api->write((self), (value))

#endif
