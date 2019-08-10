/*!
 * @file
 * @brief
 */

#ifndef i_tiny_digital_input_h
#define i_tiny_digital_input_h

#include <stdbool.h>

struct i_tiny_digital_input_api_t;

typedef struct {
  const struct i_tiny_digital_input_api_t* api;
} i_tiny_digital_input_t;

typedef struct i_tiny_digital_input_api_t {
  /*!
   * Reads the digital input.
   */
  bool (*read)(i_tiny_digital_input_t* self);
} i_tiny_digital_input_api_t;

#define tiny_digital_input_read(self) \
  (self)->api->read((self))

#endif
