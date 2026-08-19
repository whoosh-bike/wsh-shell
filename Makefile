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

# Static analysis: report file (gitignored via build/ and *.log) and suppressions
CPPCHECK_LOG ?= build/cppcheck.log
CPPCHECK_SUPPRESS := --suppressions-list=.cppcheck-suppressions

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

# gen-config first: without example/wsh_shell_cfg.h cppcheck cannot resolve the
# feature macros, drowns in missingInclude noise and misses real findings.
cppcheck: gen-config
	@echo "[CHECK] Running cppcheck -> $(CPPCHECK_LOG)"
	@mkdir -p $(dir $(CPPCHECK_LOG))
	@cppcheck --quiet --enable=all --error-exitcode=1 --check-level=exhaustive \
		$(CPPCHECK_SUPPRESS) --output-file=$(CPPCHECK_LOG) \
		$(INC_FLAGS) -I$(EXAMPLE_DIR) $(SRCS); status=$$?; \
		cat $(CPPCHECK_LOG); exit $$status
