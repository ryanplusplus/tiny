/*!
 * @file
 * @brief
 */

#include "CppUTestExt/MockSupport.h"
#include "double/tiny_i2c_double.hpp"

static bool write(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  const void* buffer,
  uint16_t buffer_size)
{
  return mock()
    .actualCall("write")
    .onObject(self)
    .withParameter("address", address)
    .withParameter("prepare_for_restart", prepare_for_restart)
    .withMemoryBufferParameter("buffer", reinterpret_cast<const uint8_t*>(buffer), buffer_size)
    .returnBoolValue();
}

static bool read(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  void* buffer,
  uint16_t buffer_size)
{
  (void)buffer_size;

  return mock()
    .actualCall("read")
    .onObject(self)
    .withParameter("address", address)
    .withParameter("prepare_for_restart", prepare_for_restart)
    .withOutputParameter("buffer", buffer)
    .returnBoolValue();
}

static void reset(i_tiny_i2c_t* self)
{
  mock()
    .actualCall("reset")
    .onObject(self);
}

static const i_tiny_i2c_api_t api = { write, read, reset };

void tiny_i2c_double_init(tiny_i2c_double_t* self)
{
  self->interface.api = &api;
}
