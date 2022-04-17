__lib_tiny_path := $(call current_path)

$(call create_lib_with_defaults,tiny)

tiny_INC_DIRS := \
  $(__lib_tiny_path)include \

tiny_SYS_INC_DIRS := \

tiny_SRC_FILES := \

tiny_SRC_DIRS := \
  $(__lib_tiny_path)src \

tiny_exported_INC_DIRS := \
  $(__lib_tiny_path)include \

INC_DIRS += \
  $(tiny_exported_INC_DIRS) \
