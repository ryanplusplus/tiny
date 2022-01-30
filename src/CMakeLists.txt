add_library(tiny INTERFACE)

target_sources(tiny INTERFACE
  tiny_comm.c
  tiny_crc16.c
  tiny_event_queue.c
  tiny_event_subscription.c
  tiny_event.c
  tiny_fsm.c
  tiny_hsm.c
  tiny_list.c
  tiny_message_bus.c
  tiny_ram_key_value_store.c
  tiny_ring_buffer.c
  tiny_single_subscriber_event.c
  tiny_stack_allocator.c
  tiny_timer.c
)

target_include_directories(tiny INTERFACE
  ../include
)
