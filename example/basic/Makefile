### Makefile (example/basic/Makefile) ###

# ===== Project Configuration =====
TARGET := basic
BUILD ?= Debug

# ===== Toolchain =====
CC := clang
MKDIR := mkdir -p
RM := rm -rf

# ===== Paths =====
SRC_DIR := .
CORE_DIR := ../../src
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

# ===== Compiler Flags =====
COMMON_FLAGS := $(INC_FLAGS) -MMD -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-format
DEBUG_FLAGS := -O0 -g -DWSH_SHELL_ASSERT_ENABLE
RELEASE_FLAGS := -O2 -DNDEBUG

ifeq ($(findstring clang,$(CC)),clang)
    COMMON_FLAGS += -Wno-gnu-zero-variadic-macro-arguments
endif

ifeq ($(BUILD),Debug)
    CFLAGS := $(COMMON_FLAGS) $(DEBUG_FLAGS)
else
    CFLAGS := $(COMMON_FLAGS) $(RELEASE_FLAGS)
endif

# ===== Targets =====
.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(ALL_OBJS)
	@echo "[LD] $@"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/src/%.o: $(CORE_DIR)/%.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "[CLEAN] Removing build directory"
	@$(RM) $(BUILD_DIR)

# ===== Include Dependencies =====
-include $(DEPS)
