# Build Guide

This guide explains how to build the Wsh-Shell library and run its example applications both on a PC and on embedded hardware.

---

## Prerequisites

Ensure the required toolchain components are installed on your system:

```bash
make --version
gcc --version # for PC builds
arm-none-eabi-gcc --version  # for MCU builds
openocd --version # for MCU flash burn
doxygen --version # for docs generation with `mkdocs serve` on local machine
```

> 💡 You can also use `clang` instead of `gcc` for PC builds.

For exra actions you should use python virtualenv with `requirements.txt`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
```

---

## Changing the Compiler

You can change the compiler in the project’s **Makefile**:

```makefile
# ===== Toolchain =====
CC := gcc
# or
CC := clang
```

---

## Main Build

To build the default library:

```bash
make
```

During the first build, the default configuration file `wsh_shell_cfg_def.h` will be copied to `wsh_shell_cfg.h`, which is used in the build process.

By default, the project is built in **Debug** mode.  
To build with the **Release** preset:

```bash
make BUILD=Release
```

This works for all examples (e.g., `basic` and `blue_pill`).

---

## Cleaning the Build

To remove temporary build files while keeping your configuration:

```bash
make clean
```

> ⚠️ This does **not** delete `wsh_shell_cfg.h`.  
> Remove it manually if you want to regenerate it from the default template.

---

## Building and Running Examples

### Basic Example (PC)

This example runs locally on your machine:

```bash
make clean && make basic
./example/basic/build/basic
```

or

```bash
cd example/basic
make
./build/basic
```

---

### Blue Pill Example (STM32 MCU)

This example builds for the STM32F103 "Blue Pill" board:

```bash
make clean && make blue_pill
```

For vscode launch run `gen_launch.py` script for `launch.json` generation from `launch.template.json`:

```python
python3 .vscode/gen_launch.py 
```

But previously you should setup `.env` file with required variables:

```bash
MAKE_PATH="/opt/homebrew/opt/make/libexec/gnubin/make"
OPENOCD_PATH="/opt/homebrew/bin/openocd"
ARM_NONE_EABI_PATH="/Users/katbert/my-utils/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi/bin"
GDB_PATH="/Users/katbert/my-utils/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi/bin/arm-none-eabi-gdb"
NM_PATH="/Users/katbert/my-utils/arm-gnu-toolchain-14.2.rel1-darwin-arm64-arm-none-eabi/bin/arm-none-eabi-nm"
```

Then flash the binary using OpenOCD:

```bash
openocd -f ./example/blue_pill/stlink.cfg \
        -f ./example/blue_pill/stm32f1x.cfg \
        -c "program example/blue_pill/build/blue_pill.bin 0x08000000 verify reset exit"
```

For the blue pill clones use `ch32f1x.cfg` config instead of `stm32f1x.cfg`

You can analyse firmware `.elf` file for memory usage by running python script (or just call `analyse elf of blue_pill` vscode task):

```bash
python3 ./utils/elf-size-analyse.py ./example/blue_pill/build/blue_pill.elf
```

---

## Additional Notes

- You can adjust build presets or flags in the **Makefile** (e.g., optimization level, debug symbols).
- If using Windows, please do all in WSL enviroment.
