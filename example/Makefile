### Makefile (example) ###

# ===== Project Configuration =====
TARGET := example
BUILD ?= Debug

# ===== Toolchain =====
CC := gcc
MKDIR := mkdir -p
RM := rm -rf

# ===== Paths =====
SRC_DIR := .
CORE_DIR := ../src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# ===== Source Files =====
SRCS := $(wildcard *.c)
CORE_SRCS := $(wildcard $(CORE_DIR)/*.c)

OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
CORE_OBJS := $(CORE_SRCS:$(CORE_DIR)/%.c=$(OBJ_DIR)/src/%.o)
ALL_OBJS := $(OBJS) $(CORE_OBJS)
DEPS := $(ALL_OBJS:.o=.d)

# ===== Include Flags =====
INC_FLAGS := -I$(SRC_DIR) -I$(CORE_DIR)

# ===== Debug/Release Flags =====
DEBUG_FLAGS := -O0 -g -DWSH_SHELL_DEBUG_ENABLE -DWSH_SHELL_ASSERT_ENABLE
RELEASE_FLAGS := -O2 -DNDEBUG

# ===== Compiler Flags =====
C_FLAGS += -std=gnu11
C_FLAGS += -Wall
C_FLAGS += -Wextra
C_FLAGS += -Wpedantic
C_FLAGS += -Wno-unused-function
C_FLAGS += -Wno-unused-parameter
C_FLAGS += -Wno-format
C_FLAGS += -MMD

COMPILE_FLAGS += $(INC_FLAGS) $(C_FLAGS)

ifeq ($(BUILD),Debug)
    COMPILE_FLAGS += $(DEBUG_FLAGS) 
else
    COMPILE_FLAGS += $(RELEASE_FLAGS)
endif

ifeq ($(findstring clang,$(CC)),clang)
    COMPILE_FLAGS += -Wno-gnu-zero-variadic-macro-arguments
endif

# ===== Targets =====
.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(ALL_OBJS)
	@echo "[LD] $@"
	@$(MKDIR) $(dir $@)
	@$(CC) $(COMPILE_FLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(COMPILE_FLAGS) -c $< -o $@

$(OBJ_DIR)/src/%.o: $(CORE_DIR)/%.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(COMPILE_FLAGS) -c $< -o $@

clean:
	@echo "[CLEAN] Removing build directory"
	@$(RM) $(BUILD_DIR)

# ===== Include Dependencies =====
-include $(DEPS)
