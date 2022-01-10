__lib_tiny_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

LIBS := tiny $(LIBS)

INC_DIRS += \
  $(__lib_tiny_path)include \

tiny_ASFLAGS := $(ASFLAGS)
tiny_CPPFLAGS := $(CPPFLAGS)
tiny_CFLAGS := $(CFLAGS)
tiny_CXXFLAGS := $(CXXFLAGS)

tiny_INC_DIRS := \
  $(__lib_tiny_path)include \

tiny_SYS_INC_DIRS := \

tiny_SRC_FILES := \

tiny_SRC_DIRS := \
  $(__lib_tiny_path)src \
