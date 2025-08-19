# Architecture

## Core Principles

- **Single-Instance State**  
    - All shell state is stored inside a single `WshShell_t` structure.  
    - Multiple instances can be created independently if needed.

- **Modular Design**  
    - Each feature is implemented as an isolated module (`cmd`, `user`, `history`, `autocomplete`, etc).
    - Modules can be enabled or disabled to reduce code size and RAM usage.

- **Configurable at Compile-Time**  
    - Main shell parameters (buffer sizes, enabled features, color schemes, etc.) are controlled via configuration macros, allowing customization without changing the source.

- **Static Memory Only**  
    - No dynamic allocation (`malloc`/`free`).
    - All commands, users, callbacks, and other resources are passed into the shell from outside.

- **Platform-Independent**  
    - Runs on bare-metal systems and under RTOS.
    - No dependencies on specific hardware or CPU architecture.

- **Lightweight Integration**  
    - The shell is provided as a **git submodule** and can be easily integrated into any project.

---

## Sources Structure & Repository Layout

| Module / File                     | Purpose |
|-----------------------------------|---------|
| **wsh_shell_autocomplete.h / .c** | Tab-completion logic for commands and options |
| **wsh_shell_cfg_def.h**           | Default and user-specific configuration macros |
| **wsh_shell_cmd_def.h / .c**      | Default/internal shell commands |
| **wsh_shell_cmd.h / .c**          | Command registration, parsing, and execution |
| **wsh_shell_esc.h / .c**          | Terminal escape sequence handling |
| **wsh_shell_history.h / .c**      | Circular buffer for command history, search, and recall |
| **wsh_shell_interact.h / .c**     | Interactive command mode handling (e.g., `wireless -i`) |
| **wsh_shell_io.h / .c**           | Input/output abstraction |
| **wsh_shell_option.h**            | Command-line option parsing helpers and context structures |
| **wsh_shell_promptwait.h / .c**   | Wait for specific keys (Enter, Yes/No) during command input |
| **wsh_shell_ps1_custom.h / .c**   | Custom shell prompt (`PS1`) generation and formatting |
| **wsh_shell_str.h / .c**          | String utilities, line editing, cursor movement, interactive editing |
| **wsh_shell_types.h**             | Core typedefs, enums, boolean and status codes |
| **wsh_shell_user.h / .c**         | Multi-user access control, authentication, and permissions |
| **wsh_shell_version.h**           | Version string, build metadata, and project identification |
| **wsh_shell.h / wsh_shell.c**     | Main shell engine: state machine, event loop, command dispatching, authentication |

```shell
.
├── LICENSE
├── Makefile
├── README.md
├── mkdocs.yml
├── requirements.txt
├── src
│   ├── wsh_shell.c
│   ├── wsh_shell.h
│   ├── wsh_shell_autocomplete.c
│   ├── wsh_shell_autocomplete.h
│   ├── wsh_shell_cfg.h
│   ├── wsh_shell_cfg_def.h
│   ├── wsh_shell_cmd.c
│   ├── wsh_shell_cmd.h
│   ├── wsh_shell_cmd_def.c
│   ├── wsh_shell_cmd_def.h
│   ├── wsh_shell_esc.c
│   ├── wsh_shell_esc.h
│   ├── wsh_shell_history.c
│   ├── wsh_shell_history.h
│   ├── wsh_shell_interact.c
│   ├── wsh_shell_interact.h
│   ├── wsh_shell_io.c
│   ├── wsh_shell_io.h
│   ├── wsh_shell_option.h
│   ├── wsh_shell_promptwait.c
│   ├── wsh_shell_promptwait.h
│   ├── wsh_shell_ps1_custom.c
│   ├── wsh_shell_ps1_custom.h
│   ├── wsh_shell_str.c
│   ├── wsh_shell_str.h
│   ├── wsh_shell_types.h
│   ├── wsh_shell_user.c
│   ├── wsh_shell_user.h
│   └── wsh_shell_version.h
└── utils
    ├── elf-size-analyse.py
    ├── gen-launch.py
    └── shell-banner-gen.py
```
