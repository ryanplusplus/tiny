/*!
 * @file
 * @brief Abstract event queue.
 *
 * Allows event callbacks and data to be queued for later processing.
 */

#ifndef i_tiny_event_queue_h
#define i_tiny_event_queue_h

#include <stdint.h>

typedef void (*tiny_event_queue_callback_t)(void);
typedef void (*tiny_event_queue_callback_with_data_t)(const void* data);

struct i_tiny_event_queue_api_t;

typedef struct {
  const struct i_tiny_event_queue_api_t* api;
} i_tiny_event_queue_t;

typedef struct i_tiny_event_queue_api_t {
  /*!
   * Enqueues an event to be raised later by invoking the provided callback.
   */
  void (*enqueue)(
    i_tiny_event_queue_t* self,
    tiny_event_queue_callback_t callback);

  /*!
   * Enqueues an event with data to be raised later by invoking the provided callback.
   *
   * @note data is copied and does not have to remain valid after the call
   */
  void (*enqueue_with_data)(
    i_tiny_event_queue_t* self,
    tiny_event_queue_callback_with_data_t callback,
    const void* data,
    uint8_t data_size);
} i_tiny_event_queue_api_t;

#define tiny_event_queue_enqueue(self, callback) \
  (self)->api->enqueue((self), (callback))

#define tiny_event_queue_enqueue_with_data(self, callback, data, data_size) \
  (self)->api->enqueue_with_data((self), (callback), (data), (data_size))

#endif
