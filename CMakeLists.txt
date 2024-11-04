add_library(tiny INTERFACE)

target_sources(tiny INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_comm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_crc16.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_event_queue.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_event_subscription.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_event.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_fsm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_hsm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_list.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_message_bus.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_queue.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_ram_key_value_store.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_ring_buffer.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_single_subscriber_event.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_stack_allocator.c
  ${CMAKE_CURRENT_LIST_DIR}/src/tiny_timer.c
)

target_include_directories(tiny INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include
)
