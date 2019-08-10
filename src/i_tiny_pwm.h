/*!
 * @file
 * @brief
 */

#ifndef i_tiny_pwm_h
#define i_tiny_pwm_h

#include <stdint.h>

typedef uint16_t tiny_pwm_duty_cycle_t;
typedef uint16_t tiny_pwm_frequency_hz_t;

struct i_tiny_pwm_api_t;

typedef struct
{
  const struct i_tiny_pwm_api_t* api;
} i_tiny_pwm_t;

typedef struct i_tiny_pwm_api_t {
  /*!
   * Sets the duty cycle. Range is normalized to 0-0xFFFF.
   */
  void (*set_duty_cycle)(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle);

  /*!
   * Sets the frequency.
   */
  void (*set_frequency)(i_tiny_pwm_t* self, tiny_pwm_frequency_hz_t frequency);
} i_tiny_pwm_api_t;

#define tiny_pwm_set_duty_cycle(self, duty_cycle) \
  (self)->api->set_duty_cycle((self), (duty_cycle))

#define tiny_pwm_set_frequency(self, frequency) \
  (self)->api->set_frequency((self), (frequency))

#endif
