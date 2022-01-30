cmake_minimum_required(VERSION 3.17)
project(tiny
	VERSION 1.0
	LANGUAGES C
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)

	if(BUILD_TESTING)
		add_subdirectory(test)
	endif()
endif()

add_subdirectory(src)
