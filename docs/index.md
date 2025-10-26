# Wsh-Shell

Wsh-Shell is a lightweight, portable, and fully static shell interpreter written in C, designed for embedded systems. It requires no dynamic memory allocation and is built to run in constrained environments like microcontrollers, either bare-metal or under RTOS (e.g., FreeRTOS).

## 🚀 Features

- **Cross-platform, Highly Portable** — only one header file to include
- **Single State Structure** — all shell state contained in a single `WshShell_t` instance  
- **Static Memory Only** — no `malloc`, no heap; all buffers are statically allocated  
- **Modular Design** — ability to disable submodules for memory footprint optimization
- **Command-line Editing** — supports cursor movement, character deletion, and insert mode  
- **Command Parsing & Options**:  
    - Supports short (`-h`) and long (`--help`) flags  
    - Supports int, float, string and other option types
    - Supports double-quoted strings
- **Group-based Access Control** — each command belongs to one or more logical groups; users are granted access only if their group set intersects with the command’s group set
- **Fine-grained Option Access Rights** — every command option (-f, --reset, etc.) has an associated access flag (read, write, execute, admin); the shell enforces these permissions at runtime and reports mismatches
- **Escape Sequence Handling**:  
    - Parses VT100/ANSI sequences  
    - Supports arrow keys, delete, backspace, sound alerts, etc.  
    - Handles key combinations (Ctrl+C, Ctrl+D, etc.)  
- **Command History**:  
    - Implemented as a circular buffer  
    - Efficient with hash-based integrity checks  
    - Navigable with arrow keys (↑, ↓)  
- **Autocomplete**:  
    - Tab / double-Tab completion for commands and flags  
- **Multi-User Support** — groups, access rights, and more  
- **Customizable PS1 Prompt** — user-defined templates for prompt appearance
- **Await Prompt** — await for a specific key press
- **Different New Line Support** - handle different terminals setup (`\r`, `\n` or `\r\n`)
- **Passwords Stored Salted & Hashed** — passwords are supplied and verified through a user-provided callback and always stored in a salted, hashed form; by default the module uses a lightweight Jenkins (non-cryptographic) hash, and no plaintext passwords are written to flash unless the integrator explicitly chooses to do so
- **Command Option Validation** — during command registration, the shell automatically checks for duplicate short or long option flags within the same command and triggers an ASSERT if duplicates are detected

## 👾 Demo

![Demo](img/demo.gif)

## 💾 Memory footprint

- Build options: cortex-m7, `-O1` optimization
- sizeof(WshShell_t) = 404 bytes

| Config                                | FLASH, KB | Comment                                              |
| --------------------------------------| --------- | ---------------------------------------------------- |
| All features disabled                 | 4.06836   |                                                      |
| `+WSH_SHELL_PRINT_SYS/INFO/WARN/ERR`  | 4.58008   | Not recommended to disable shell messages            |
| `+WSH_SHELL_INTERACTIVE_MODE`         | 4.68164   |                                                      |
| `+WSH_SHELL_HISTORY`                  | 5.63867   |                                                      |
| `+WSH_SHELL_AUTOCOMPLETE`             | 6.19727   |                                                      |
| `+WSH_SHELL_PS1_CUSTOM`               | 6.66992   |                                                      |
| `+WSH_SHELL_PROMPT_WAIT`              | 6.78516   |                                                      |
| `+WSH_SHELL_DEF_COMMAND`              | 8.44922   |                                                      |
| `+WSH_SHELL_PRINT_OPT_HELP`           | 8.44922   | Could be usefull on huge amount of external commands |
| `+WSH_SHELL_CMD_PRINT_OPT_OVERVIEW`   | 8.69922   | Could be usefull on huge amount of external commands |

## ⌨️ Code counting

```markdown
===============================================================================
 Language            Files        Lines         Code     Comments       Blanks
===============================================================================
 C                      13         2102         1610           52          440
 C Header               17         2093          758         1095          240
===============================================================================
 Total                  30         4195         2368         1147          680
===============================================================================
```

## 👨‍💻 Authors

- **<abalyberdin@whoosh.bike>** — initial MVP  
- **<vignatov@whoosh.bike>** — improvements, refactoring
- **<akrestinin@whoosh.bike>** — project separation (for submodule usage), main structure, PC/MCU examples  
- **<sshilin@whoosh.bike>** — UX improvements, extra features, documentation, public release
- **<eshamaev@whoosh.bike>**  — CI/CD, docs deployment, high-level PC command app

## ⚖️ License

This project is licensed under the `MIT License`.

You are free to use, modify, and distribute this software in both commercial and non-commercial projects, provided that the original copyright notice and this permission notice are included.

![Shell](img/shell_wide.png)
