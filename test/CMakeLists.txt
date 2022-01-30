enable_language(CXX)

set_property(GLOBAL PROPERTY C_STANDARD 99)
set_property(GLOBAL PROPERTY CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

set(SANITIZE_FLAGS -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -Wcast-qual)
add_compile_options(-Wall -Wextra -pedantic -Werror ${SANITIZE_FLAGS})
add_link_options(${SANITIZE_FLAGS})

add_executable(tiny_tests)

add_test(test_build
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target tiny_tests
)

set_tests_properties(test_build PROPERTIES FIXTURES_SETUP test_fixture)
add_test(NAME test_run COMMAND ./tiny_tests)
set_tests_properties(test_run PROPERTIES FIXTURES_REQUIRED test_fixture)

find_package(PkgConfig REQUIRED)
pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)

add_subdirectory(double)

target_include_directories(tiny_tests PRIVATE . ${CPPUTEST_INCLUDE_DIRS})

link_directories(${CPPUTEST_LIBRARIES})
target_link_libraries(tiny_tests PRIVATE tiny tiny_doubles ${CPPUTEST_LDFLAGS})

target_sources(tiny_tests PRIVATE
  asan_options.cpp
  CMakeLists.txt
  test_runner.cpp
  tiny_comm_test.cpp
  tiny_crc16_test.cpp
  tiny_event_queue_test.cpp
  tiny_event_subscription_test.cpp
  tiny_event_test.cpp
  tiny_fsm_test.cpp
  tiny_hsm_test.cpp
  tiny_list_test.cpp
  tiny_message_bus_test.cpp
  tiny_ram_key_value_store_test.cpp
  tiny_ring_buffer_test.cpp
  tiny_single_subscriber_event_test.cpp
  tiny_stack_allocator_test.cpp
  tiny_timer_test.cpp
)
