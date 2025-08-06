### Makefile (Top-level) ###

# ===== Project Configuration =====
TARGET := wsh_shell
BUILD ?= Debug

# ===== Toolchain =====
CC := clang
AR := ar
CP := cp
MKDIR := mkdir -p
RM := rm -rf

# ===== Paths =====
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj/$(SRC_DIR)
# TEST_DIR := test
# TEST_BUILD_DIR := $(BUILD_DIR)/obj/$(TEST_DIR)
BASIC_EXAMPLE_DIR := example/basic
BLUE_PILL_EXAMPLE_DIR := example/blue_pill

# ===== Source Files =====
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# ===== Test Sources =====
# TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.c)
# TEST_BINS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%_bin)

# ===== Include Paths =====
INC_FLAGS := $(addprefix -I, $(shell find $(SRC_DIR) -type d))
# TEST_INC := -Iet/Embedded-Test/et -I$(TEST_DIR)

# ===== Compiler Flags =====
COMMON_FLAGS := $(INC_FLAGS) -MMD -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-format

ifeq ($(findstring clang,$(CC)),clang)
    COMMON_FLAGS += -Wno-gnu-zero-variadic-macro-arguments
endif

ifeq ($(BUILD),Debug)
    CFLAGS := $(COMMON_FLAGS) -O0 -g -DWSH_SHELL_ASSERT_ENABLE
else
    CFLAGS := $(COMMON_FLAGS) -O2 -DNDEBUG
endif

# ===== Submodules =====
$(shell git submodule update --init --recursive)
# ET_SRCS := $(wildcard et/Embedded-Test/et/*.c)

# ===== Targets =====
.PHONY: all clean test basic blue_pill format cppcheck

all: $(BUILD_DIR)/lib$(TARGET).a

$(BUILD_DIR)/lib$(TARGET).a: src/wsh_shell_cfg.h $(OBJS)
	@echo "[AR] Creating static library: $@"
	@$(MKDIR) $(dir $@)
	@$(AR) rcs $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

src/wsh_shell_cfg.h:
	@echo "[GEN] $@ from default"
	@$(CP) src/wsh_shell_cfg_def.h $@

# test: $(TEST_BINS)
# 	@for test_exec in $(TEST_BINS); do \
# 		echo "[RUN] $$test_exec"; \
# 		$$test_exec || exit 1; \
# 	done

# $(BUILD_DIR)/test/%_bin: $(TEST_DIR)/%.c $(ET_SRCS) $(SRCS)
# 	@echo "[TEST_CC] $<"
# 	@$(MKDIR) $(dir $@)
# 	@$(CC) $(CFLAGS) $(TEST_INC) $^ -o $@

basic:
	@echo "[MAKE] Building example: basic"
	@$(MAKE) -C $(BASIC_EXAMPLE_DIR)

blue_pill:
	@echo "[MAKE] Building example: blue_pill"
	@$(MAKE) -C $(BLUE_PILL_EXAMPLE_DIR)

clean:
	@echo "[CLEAN] Removing build artifacts"
	@$(RM) $(BUILD_DIR)
	@$(MAKE) -C $(BASIC_EXAMPLE_DIR) clean
	@$(MAKE) -C $(BLUE_PILL_EXAMPLE_DIR) clean

format:
	@echo "[FORMAT] Running clang-format"
	@clang-format --style=file -i $(SRC_DIR)/*.[ch] $(BASIC_EXAMPLE_DIR)/main.c

cppcheck:
	@echo "[CHECK] Running cppcheck"
	@cppcheck --quiet --enable=all --error-exitcode=1 --check-level=exhaustive $(INC_FLAGS) $(SRCS)

# ===== Include Dependencies =====
-include $(DEPS)
