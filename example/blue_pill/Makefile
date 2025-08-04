### Makefile (example/blue_pill/Makefile) ###

# ===== Project Configuration =====
TARGET := blue_pill
BUILD ?= Debug

# ===== Toolchain =====
TOOLCHAIN_PREFIX := arm-none-eabi-
CC := $(TOOLCHAIN_PREFIX)gcc
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
SIZE := $(TOOLCHAIN_PREFIX)size
MKDIR := mkdir -p
RM := rm -rf

# ===== Paths =====
SRC_DIR := -I.
SRC_DIR += -I./platform
SRC_DIR += -I./src
SRC_DIR += -I./system
SRC_DIR += -I./STM32CubeF1
SRC_DIR += -I./STM32CubeF1/Drivers
SRC_DIR += -I./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver
SRC_DIR += -I./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc
SRC_DIR += -I./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
SRC_DIR += -I./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src
SRC_DIR += -I./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/Legacy
CORE_DIR := ../../src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# ===== Source Files =====
SRCS := $(wildcard src/*.c)
SRCS += ./system/stm32f1xx_it.c
SRCS += ./system/system_stm32f1xx.c
SRCS += ./system/syscalls.c
SRCS += ./platform/startup_stm32f103xb.s
SRCS += ./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_gpio.c
SRCS += ./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rcc.c
SRCS += ./STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_utils.c
CORE_SRCS := $(wildcard $(CORE_DIR)/*.c)

OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
CORE_OBJS := $(CORE_SRCS:$(CORE_DIR)/%.c=$(OBJ_DIR)/src/%.o)
ALL_OBJS := $(OBJS) $(CORE_OBJS)
DEPS := $(ALL_OBJS:.o=.d)

# ===== Compiler Flags =====
COMMON_FLAGS := $(SRC_DIR) -I$(CORE_DIR) -MMD -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-format
CPU_FLAGS := -mcpu=cortex-m3 -mfloat-abi=soft -mthumb -DSTM32F103xB -DUSE_FULL_LL_DRIVER

LINKER_FLAGS += -Wl,-cref
LINKER_FLAGS += -Wl,-u,Reset_Handler
LINKER_FLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LINKER_FLAGS += -specs=nano.specs
LINKER_FLAGS += -specs=nosys.specs
LINKER_FLAGS += -u _printf_float
LINKER_FLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
LINKER_FLAGS += -Wl,--gc-sections
LINKER_FLAGS += -Wl,--print-memory-usage
LINKER_FLAGS += -Wl,--no-warn-rwx-segments

LINKER_SCRIPT := ./platform/STM32F103XB_FLASH.ld

DEBUG_FLAGS := -O0 -g -DWSH_SHELL_ASSERT_ENABLE
RELEASE_FLAGS := -O2 -DNDEBUG

ifeq ($(BUILD),Debug)
    CFLAGS := $(COMMON_FLAGS) $(CPU_FLAGS) $(DEBUG_FLAGS)
else
    CFLAGS := $(COMMON_FLAGS) $(CPU_FLAGS) $(RELEASE_FLAGS)
endif

# ===== Targets =====
.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(ALL_OBJS)
	@echo "[LD] $@"
	@$(MKDIR) $(dir $@)
	@$(CC) -T$(LINKER_SCRIPT) $^ -o $@.elf $(LINKER_FLAGS) $(CPU_FLAGS)
	@echo "[OBJCOPY] $@.elf -> $@.hex"
	@$(OBJCOPY) -O ihex $@.elf $@.hex
	@echo "[OBJCOPY] $@.elf -> $@.bin"
	@$(OBJCOPY) -O binary $@.elf $@.bin
	@$(SIZE) $@.elf

$(OBJ_DIR)/%.c.o: %.c
	@echo "[CC] $<"
	@$(MKDIR) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.s.o: %.s
	@echo "[AS] $<"
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
