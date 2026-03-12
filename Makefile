### Makefile (Top-level) ###

# ===== Project Configuration =====
BUILD ?= debug

# ===== Toolchain =====
CC ?= gcc
CP := cp
RM := rm -rf

# ===== Paths =====
SRC_DIR     := src
EXAMPLE_DIR := example

# ===== Source Files =====
SRCS      := $(wildcard $(SRC_DIR)/*.c)
INC_FLAGS := $(addprefix -I, $(shell find $(SRC_DIR) -type d))

# ===== Targets =====
.PHONY: all clean example gen-config format cppcheck

all: example

gen-config:
	@echo "[GEN] $(EXAMPLE_DIR)/wsh_shell_cfg.h from default"
	@$(CP) $(SRC_DIR)/wsh_shell_cfg_def.h $(EXAMPLE_DIR)/wsh_shell_cfg.h

example: gen-config
	@echo "[MAKE] Building example"
	@$(MAKE) -C $(EXAMPLE_DIR) BUILD=$(BUILD) CC=$(CC)

clean:
	@echo "[CLEAN] Removing build artifacts"
	@$(MAKE) -C $(EXAMPLE_DIR) clean

format:
	@echo "[FORMAT] Running clang-format"
	@clang-format --style=file -i $(SRC_DIR)/*.[ch] $(EXAMPLE_DIR)/main.c

cppcheck:
	@echo "[CHECK] Running cppcheck"
	@cppcheck --quiet --enable=all --error-exitcode=1 --check-level=exhaustive $(INC_FLAGS) $(SRCS)
