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
# Type `exit` in the shell to quit — the example runs the terminal in raw mode
# with signals disabled, so Ctrl+C is delivered to the shell as a cancel key
# rather than killing the process.
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

## Measuring the Memory Footprint

Feature flags are only interesting if their cost is known, and toggling them by
hand in a real firmware project is slow and hard to compare. `utils/measure-footprint.py`
does it mechanically: for every configuration it renders a temporary
`wsh_shell_cfg.h` from the default template, rebuilds the library for the target
CPU, links it against a synthetic stub that touches every public entry point, and
sums the linker-map sections that came from the shell's own object files. libc,
startup code and the stub are never counted, so the numbers are comparable across
configurations and machines.

```bash
make footprint                                  # cumulative table (README-style)
python3 utils/measure-footprint.py --mode drop  # what disabling one feature saves
python3 utils/measure-footprint.py --mode each  # what one feature costs on its own
python3 utils/measure-footprint.py --mode single --enable WSH_SHELL_HISTORY --json
```

Useful options:

| Option           | Meaning                                                    |
| ---------------- | ---------------------------------------------------------- |
| `--cpu`, `--opt` | Target and optimization level (default `cortex-m7`, `-O1`) |
| `--cc`, `--nm`   | Toolchain binaries (default `arm-none-eabi-gcc`/`-nm`)     |
| `--markdown`     | Markdown table, ready to paste into the README             |
| `--json`         | Raw numbers for CI or plotting                             |

> [!IMPORTANT]
> `--mode drop` is the number to trim flash by. It rebuilds the **full** configuration
> with one feature removed, so it answers the only question that matters in practice —
> "what do I actually save by switching this off?" Feature costs are not additive:
> code is shared between features, and a flag that looks cheap on a bare build can be
> the most expensive one in a full build (`WSH_SHELL_SUBCOMMANDS`: ~0.6 KB alone,
> ~6.4 KB once the default `wsh` command tree exists).

The script also reports `sizeof(WshShell_t)` for each configuration, read back
from a probe symbol in the linked ELF, so RAM cost is measured on the target's
word size rather than the host's.

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

## Formatting

```bash
make format            # or: ./utils/code-format.sh
```

Both routes run the same script, so `src/` and `example/` are always formatted
identically. The generated `example/wsh_shell_cfg.h` is skipped on purpose — it must
stay a byte-identical copy of `src/wsh_shell_cfg_def.h`.

---

## Static Analysis

```bash
make cppcheck                             # report also written to build/cppcheck.log
make cppcheck CPPCHECK_LOG=/tmp/cc.log    # elsewhere
```

Findings are printed and saved to the log file; the exit code is cppcheck's own, so
the target still fails the build on findings (`--error-exitcode=1`). The log path is
covered by `.gitignore`.

The target depends on `gen-config` and analyses `src/` with `example/wsh_shell_cfg.h`
on the include path. Without a resolvable config cppcheck cannot evaluate the feature
macros: it drowns the report in `missingInclude` noise, checks far more `#ifdef`
combinations (slow), and still misses real findings that only appear once the buffer
sizes are known.

Class-wide suppressions live in `.cppcheck-suppressions`, each with the reason it is
there — mostly artefacts of analysing a library without its callers (`unusedFunction`,
`staticFunction`). One-off exceptions are inline `// cppcheck-suppress <id>` comments
next to the code they excuse (`--inline-suppr`).

---

## Additional Notes

- You can adjust build flags in `example/Makefile` (optimization level, debug symbols, etc.)
- If using Windows, please use a WSL environment
