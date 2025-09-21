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

## Building and Running Example

### Default Example (PC)

This example runs locally on your machine:

```bash
make clean && make example
./example/build/example
```

or

```bash
cd example
make clean
make
./build/example
```

### Example on Hardware

It has been moved to <https://github.com/katbert-92/wsh-shell-blue-pill-example> repo

---

## Additional Notes

- You can adjust build presets or flags in the **Makefile** (e.g., optimization level, debug symbols).
- If using Windows, please do all in WSL enviroment.
