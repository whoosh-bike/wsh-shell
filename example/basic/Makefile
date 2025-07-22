ifeq ($(OS),Windows_NT)
	CMAKE := cmake
else
	CMAKE := $(shell (command -v cmake3 || command -v cmake || echo cmake))
endif

MKDIR := mkdir -p
RM := rm -rf
CMAKE_C_COMPILER := gcc
CMAKE_BUILD_TYPE ?= Debug

$(shell mkdir -p build)

.PHONY: clean cppcheck

all: 
	@$(MKDIR) build
	$(CMAKE) -B build --preset $(CMAKE_BUILD_TYPE)
	$(CMAKE) --build build

clean:
ifneq ($(wildcard build),)
	@$(CMAKE) --build build --target clean
endif

purge:
ifneq ($(wildcard build),)
	@$(RM) build/*
endif

-include $(OBJS:.o=.d)
