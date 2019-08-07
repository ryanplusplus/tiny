/*!
 * @file
 * @brief
 */

#ifndef i_tiny_gpio_h
#define i_tiny_gpio_h

#include <stdint.h>
#include <stdbool.h>

enum {
  tiny_gpio_direction_input,
  tiny_gpio_direction_output
};
typedef uint8_t tiny_gpio_direction_t;

struct i_tiny_gpio_api_t;

typedef struct
{
  const struct i_tiny_gpio_api_t* api;
} i_tiny_gpio_t;

typedef struct i_tiny_gpio_api_t {
  /*!
   * Set the direction of the GPIO.
   */
  void (*set_direction)(i_tiny_gpio_t* self, tiny_gpio_direction_t direction);

  /*!
   * Read the state of the GPIO (must be configured as an input).
   */
  bool (*read)(i_tiny_gpio_t* self);

  /*!
   * Write the state of the GPIO (must be configured as an input).
   */
  void (*write)(i_tiny_gpio_t* self, bool state);
} i_tiny_gpio_api_t;

#define tiny_gpio_set_direction(self, direction) \
  (self)->api->set_direction((self), (direction))

#define tiny_gpio_read(self) \
  (self)->api->read((self))

#define tiny_gpio_write(self, state) \
  (self)->api->write((self), (state))

#endif
