/*!
 * @file
 * @brief
 */

#ifndef tiny_i2c_double_hpp
#define tiny_i2c_double_hpp

extern "C" {
#include "hal/i_tiny_i2c.h"
}

typedef struct {
  i_tiny_i2c_t interface;
} tiny_i2c_double_t;

/*!
 * Initializes an I2C double.
 */
void tiny_i2c_double_init(tiny_i2c_double_t* self);

#endif
