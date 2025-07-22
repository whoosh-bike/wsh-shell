ifeq ($(OS),Windows_NT)
	CMAKE := cmake
	CMAKE_GENERATOR := Ninja
else
	CMAKE := $(shell (command -v cmake3 || command -v cmake || echo cmake))
	CMAKE_GENERATOR ?= "$(shell (command -v ninja > /dev/null 2>&1 && echo "Ninja") || echo "Unix Makefiles")"
endif

MKDIR := mkdir -p
RM := rm -rf
CP := cp
SRCS := $(wildcard src/*.c)
CMAKE_C_COMPILER := gcc
CMAKE_BUILD_TYPE ?= Debug

ifneq ("$(wildcard .env)","")
	include .env
endif

.PHONY: clean purge test cppcheck format

all: src/wsh_shell_cfg.h 
	@$(MKDIR) build
	$(CMAKE) -B build -G $(CMAKE_GENERATOR) --preset $(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$(CMAKE_C_COMPILER)
	$(CMAKE) --build build

src/wsh_shell_cfg.h:
	$(CP) src/wsh_shell_cfg_def.h src/wsh_shell_cfg.h

clean:
ifneq ($(wildcard build),)
	@$(CMAKE) --build build --target clean
endif

purge:
ifneq ($(wildcard build),)
	@$(RM) build/*
endif

test:
	@ctest --output-on-failure --test-dir ./build/

cppcheck:
	@cppcheck \
	--quiet \
	--enable=all \
	--error-exitcode=1 \
	--check-level=exhaustive \
	-Isrc \
	$(SRCS)

format:
	@clang-format -i -- src/*.c src/*.h
