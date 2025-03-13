/*!
 * @file
 * @brief Digital output test double.
 */

#ifndef tiny_digital_output_double_hpp
#define tiny_digital_output_double_hpp

extern "C" {
#include "hal/i_tiny_digital_output.h"
}

typedef struct {
  i_tiny_digital_output_t interface;
  bool state;
} tiny_digital_output_double_t;

/*!
 * Initializes a digital output group test double.
 */
void tiny_digital_output_double_init(
  tiny_digital_output_double_t* self,
  bool initial_state);

/*!
 * The current value of the fake output.
 */
bool tiny_digital_output_double_state(
  tiny_digital_output_double_t* self);

#endif
