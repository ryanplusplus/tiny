TARGET ?= tiny_tests
BUILD_DIR ?= ./build

INC_DIRS ?= \
  include \
  test/include \

SRC_DIRS ?= \
  src \
  test/tests \
  test/src \

SRC_FILES ?= \

SRCS := $(SRC_FILES) $(shell find $(SRC_DIRS) -maxdepth 1 -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SANITIZE_FLAGS := -fsanitize=address -fsanitize=undefined

CFLAGS += -std=c99 -pedantic
CPPFLAGS += $(SANITIZE_FLAGS) -fno-omit-frame-pointer
CPPFLAGS += $(INC_FLAGS) -MMD -MP -g -Wall -Wextra -Wcast-qual -Werror
CXXFLAGS += -std=c++17
LDFLAGS := $(SANITIZE_FLAGS)
LDLIBS := -lstdc++ -lCppUTest -lCppUTestExt -lm

BUILD_DEPS += $(MAKEFILE_LIST)

.PHONY: test
test: $(BUILD_DIR)/$(TARGET)
	@echo Running tests...
	@$(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo Linking $@...
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.s.o: %.s $(BUILD_DEPS)
	@echo Assembling $<...
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c $(BUILD_DEPS)
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp $(BUILD_DEPS)
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@rm -rf $(BUILD_DIR)

-include $(DEPS)
