# Build Guide

This guide explains how to build the Wsh-Shell example application both on a PC and on embedded hardware.

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

For extra actions you should use python virtualenv with `requirements.txt`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
```

Or just run `setup python env` vscode task

---

## Project Structure

```bash
wsh-shell/
├── src/                    # Library source files
│   ├── wsh_shell_cfg_def.h # Default configuration template
│   └── ...
└── example/                # Example application
    ├── wsh_shell_cfg.h     # Generated config (copy of cfg_def.h, gitignored)
    └── ...
```

The configuration file `wsh_shell_cfg.h` is not tracked in git. It is generated automatically in the `example/` directory when building the example.

---

## Building and Running Example

```bash
make example
./example/build/example

# Default example username `root` and password `1234`
```

By default, the project is built in **Debug** mode.
To build with the **Release** preset:

```bash
make example BUILD=release
```

---

## Generating Config Only

To copy the default config template into `example/` without building:

```bash
make gen-config
```

This creates `example/wsh_shell_cfg.h` from `src/wsh_shell_cfg_def.h`.
Edit this file to customize the shell configuration for your project.

---

## Cleaning the Build

```bash
make clean
```

> ⚠️ This does **not** delete `example/wsh_shell_cfg.h`.
> Remove it manually if you want to regenerate it from the default template.

---

## Example on Hardware

It has been moved to separate repos:

- <https://github.com/whoosh-bike/wsh-shell-blue-pill-example>
- <https://github.com/whoosh-bike/wsh-shell-black-pill-example>

---

## Changing the Compiler

By default `gcc` is used. To switch to `clang`:

```bash
make CC=clang
make CC=clang BUILD=release
```

---

## Additional Notes

- You can adjust build flags in `example/Makefile` (optimization level, debug symbols, etc.)
- If using Windows, please use a WSL environment
