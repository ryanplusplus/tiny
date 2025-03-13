/*!
 * @file
 * @brief
 */

#include "double/tiny_digital_output_double.hpp"

static void write(i_tiny_digital_output_t* _self, bool value)
{
  auto self = reinterpret_cast<tiny_digital_output_double_t*>(_self);
  self->state = value;
}

static const i_tiny_digital_output_api_t api = { write };

void tiny_digital_output_double_init(
  tiny_digital_output_double_t* self,
  bool initial_state)
{
  self->interface.api = &api;
  self->state = initial_state;
}

/*!
 * The current value of the fake output.
 */
bool tiny_digital_output_double_state(
  tiny_digital_output_double_t* self)
{
  return self->state;
}
