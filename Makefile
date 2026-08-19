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

# Report file for static analysis (gitignored via build/ and *.log)
CPPCHECK_LOG ?= build/cppcheck.log

# ===== Source Files =====
SRCS      := $(wildcard $(SRC_DIR)/*.c)
INC_FLAGS := $(addprefix -I, $(shell find $(SRC_DIR) -type d))

# ===== Targets =====
.PHONY: all clean example gen-config format cppcheck footprint

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
	@./utils/code-format.sh

footprint:
	@echo "[SIZE] Measuring flash footprint per feature set"
	@python3 utils/measure-footprint.py --markdown

cppcheck:
	@echo "[CHECK] Running cppcheck -> $(CPPCHECK_LOG)"
	@mkdir -p $(dir $(CPPCHECK_LOG))
	@cppcheck --quiet --enable=all --error-exitcode=1 --check-level=exhaustive \
		--output-file=$(CPPCHECK_LOG) $(INC_FLAGS) $(SRCS); status=$$?; \
		cat $(CPPCHECK_LOG); exit $$status
