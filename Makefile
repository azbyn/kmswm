COMPILER := g++
SRC_DIR := src
BUILD_DIR := build
TARGET := kmswm
BIN_DIR := ~/bin/
#BINDIR := /usr/local/bin

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

THIS_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))



CPPFLAGS := $(INC_FLAGS)
CPPFLAGS += -Wall -Wextra -Wpedantic -g -std=c++17
CPPFLAGS += -I /usr/include/libdrm

LDFLAGS := -pthread -ldrm

$(TARGET): $(OBJS)
	@$(COMPILER) $(OBJS) -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	@$(COMPILER) $(CPPFLAGS) -c $< -o $@

install:
	@cp $(TARGET) $(BIN_DIR)

.PHONY: clean
clean:
	@$(RM) -r $(BUILD_DIR) $(TARGET)

-include $(DEPS)

MKDIR_P ?= mkdir -p


