/*!
 * @file
 * @brief
 */

extern "C" const char* __asan_default_options()
{
  return "new_delete_type_mismatch=0:alloc_dealloc_mismatch=0:abort_on_error=1";
}
