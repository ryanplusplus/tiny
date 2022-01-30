add_library(tiny_doubles)

target_sources(tiny_doubles PRIVATE
  tiny_time_source_double.cpp
  tiny_timer_group_double.cpp
  tiny_uart_double.cpp
)

target_include_directories(tiny_doubles PUBLIC .)

target_link_libraries(tiny_doubles PRIVATE tiny)
