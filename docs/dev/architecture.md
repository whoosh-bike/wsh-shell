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

## Project Structure

| Module / File                | Purpose |
|------------------------------|---------|
| **wsh_shell_autocomplete.h** | Tab-completion logic |
| **wsh_shell_cfg_def.h**      | Default and user-specific configuration macros |
| **wsh_shell_cmd_def.h**      | Default command for shell internal usage |
| **wsh_shell.h**              | Main shell engine (state machine, event loop) |
| **wsh_shell_esc.h**          | Terminal escape sequence handling |
| **wsh_shell_history.h**      | Circular buffer for command history |
| **wsh_shell_interact.h**     | Command interactive mode handling |
| **wsh_shell_io.h**           | Input/output abstraction layer |
| **wsh_shell_option.h**       | Command-line option parsing helpers |
| **wsh_shell_promptwait.h**   | Custom keys await processing  |
| **wsh_shell_str.h**          | Line editing, cursor movement, interactive editing |
| **wsh_shell_types.h**        | Core data types and enums |
| **wsh_shell_user.h**         | Multi-user access control and permissions |
| **wsh_shell_version.h**      | Version string and build metadata |
| **wsh_shell.h**              | Main shell engine (state machine, event loop) |

---

## Repository Layout

```shell
.
├── LICENSE
├── Makefile
├── README.md
└── src
    ├── wsh_shell.c
    ├── wsh_shell.h
    ├── wsh_shell_autocomplete.c
    ├── wsh_shell_autocomplete.h
    ├── wsh_shell_cfg_def.h
    ├── wsh_shell_cmd.c
    ├── wsh_shell_cmd.h
    ├── wsh_shell_cmd_def.c
    ├── wsh_shell_cmd_def.h
    ├── wsh_shell_esc.c
    ├── wsh_shell_esc.h
    ├── wsh_shell_history.c
    ├── wsh_shell_history.h
    ├── wsh_shell_interact.c
    ├── wsh_shell_interact.h
    ├── wsh_shell_io.c
    ├── wsh_shell_io.h
    ├── wsh_shell_option.h
    ├── wsh_shell_promptwait.c
    ├── wsh_shell_promptwait.h
    ├── wsh_shell_str.c
    ├── wsh_shell_str.h
    ├── wsh_shell_types.h
    ├── wsh_shell_user.c
    ├── wsh_shell_user.h
    └── wsh_shell_version.h
```
