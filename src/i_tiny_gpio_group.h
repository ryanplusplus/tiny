/*!
 * @file
 * @brief
 */

#ifndef i_tiny_gpio_group_h
#define i_tiny_gpio_group_h

#include <stdint.h>
#include <stdbool.h>
#include "i_tiny_gpio.h"

typedef uint8_t tiny_gpio_channel_t;

struct i_tiny_gpio_group_api_t;

typedef struct {
  const struct i_tiny_gpio_group_api_t* api;
} i_tiny_gpio_group_t;

typedef struct i_tiny_gpio_group_api_t {
  /*!
   * Set the direction of the GPIO channel.
   */
  void (*set_direction)(
    i_tiny_gpio_group_t* self,
    tiny_gpio_channel_t channel,
    tiny_gpio_direction_t direction);

  /*!
   * Read the state of the GPIO channel (must be configured as an input).
   */
  bool (*read)(i_tiny_gpio_group_t* self, tiny_gpio_channel_t channel);

  /*!
   * Write the state of the GPIO channel (must be configured as an input).
   */
  void (*write)(i_tiny_gpio_group_t* self, tiny_gpio_channel_t channel, bool state);
} i_tiny_gpio_group_api_t;

#define tiny_gpio_group_set_direction(self, channel, direction) \
  (self)->api->set_direction((self), (channel)(direction))

#define tiny_gpio_group_read(self, channel) \
  (self)->api->read((self), (channel))

#define tiny_gpio_group_write(self, channel, state) \
  (self)->api->write((self), (channel)(state))

#endif
