/*!
 * @file
 * @brief
 */

#include "tiny_time_source_double.h"
#include "tiny_utils.h"

static tiny_time_source_ticks_t ticks(i_tiny_time_source_t* _self)
{
  auto self = reinterpret_cast<tiny_time_source_double_t*>(_self);
  return self->ticks;
}

static const i_tiny_time_source_api_t api = { ticks };

void tiny_time_source_double_init(tiny_time_source_double_t* self)
{
  self->interface.api = &api;
  self->ticks = 0;
}

void tiny_time_source_double_set_ticks(tiny_time_source_double_t* self, tiny_time_source_ticks_t ticks)
{
  self->ticks = ticks;
}

void tiny_time_source_double_tick(tiny_time_source_double_t* self, tiny_time_source_ticks_t ticks)
{
  self->ticks += ticks;
}
