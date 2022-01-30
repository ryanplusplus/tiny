cmake_minimum_required(VERSION 3.17)
project(tiny
	VERSION 1.0
	LANGUAGES C
)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)

	if(BUILD_TESTING)
		add_subdirectory(test)
	endif()
endif()

add_subdirectory(src)
