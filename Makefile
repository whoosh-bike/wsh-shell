### Makefile (Top-level) ###

# ===== Project Configuration =====
TARGET := wsh_shell
BUILD ?= Debug

# ===== Toolchain =====
CC := gcc
AR := ar
CP := cp
MKDIR := mkdir -p
RM := rm -rf

# ===== Paths =====
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj/$(SRC_DIR)
EXAMPLE_DIR := example

# ===== Source Files =====
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# ===== Include Paths =====
INC_FLAGS := $(addprefix -I, $(shell find $(SRC_DIR) -type d))

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
    COMMON_FLAGS += -Wno-gnu-zero-variadic-macro-arguments
endif

# ===== Submodules =====
$(shell git submodule update --init --recursive)
# ET_SRCS := $(wildcard et/Embedded-Test/et/*.c)

# ===== Targets =====
.PHONY: all clean example format cppcheck

all: $(BUILD_DIR)/lib$(TARGET).a

$(BUILD_DIR)/lib$(TARGET).a: src/wsh_shell_cfg.h $(OBJS)
	@echo "[AR] Creating static library: $@"
	@$(MKDIR) $(dir $@)
	@$(AR) rcs $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(COMPILE_FLAGS) -c $< -o $@

src/wsh_shell_cfg.h:
	@echo "[GEN] $@ from default"
	@$(CP) src/wsh_shell_cfg_def.h $@

clean:
	@echo "[CLEAN] Removing build artifacts"
	@$(RM) $(BUILD_DIR)
	@$(MAKE) -C $(EXAMPLE_DIR) clean

example:
	@echo "[MAKE] Building example"
	@$(MAKE) -C $(EXAMPLE_DIR)

format:
	@echo "[FORMAT] Running clang-format"
	@clang-format --style=file -i $(SRC_DIR)/*.[ch] $(EXAMPLE_DIR)/main.c

cppcheck:
	@echo "[CHECK] Running cppcheck"
	@cppcheck --quiet --enable=all --error-exitcode=1 --check-level=exhaustive $(INC_FLAGS) $(SRCS)

# ===== Include Dependencies =====
-include $(DEPS)
