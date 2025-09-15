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

## 👾 Demo

![Demo](img/demo.gif)

## 💾 Memory footprint

- Build options: cortex-m7, `-O1` optimization
- sizeof(WshShell_t) = 336 bytes

| Config                                | FLASH, KB | Comment                                              |
| --------------------------------------| --------- | ---------------------------------------------------- |
| All features disabled                 | 3.50781   |                                                      |
| `+WSH_SHELL_PRINT_SYS/INFO/WARN/ERR`  | 3.8457    | Not recommended to disable shell messages            |
| `+WSH_SHELL_INTERACTIVE_MODE`         | 3.94336   |                                                      |
| `+WSH_SHELL_HISTORY`                  | 4.88672   |                                                      |
| `+WSH_SHELL_AUTOCOMPLETE`             | 5.44531   |                                                      |
| `+WSH_SHELL_PS1_CUSTOM`               | 5.91797   |                                                      |
| `+WSH_SHELL_PROMPT_WAIT`              | 6.0332    |                                                      |
| `+WSH_SHELL_DEF_COMMAND`              | 7.50586   |                                                      |
| `+WSH_SHELL_PRINT_OPT_HELP`           | 7.50586   | Could be usefull on huge amount of external commands |
| `+WSH_SHELL_CMD_PRINT_OPT_OVERVIEW`   | 7.75977   | Could be usefull on huge amount of external commands |

## ⌨️ Code counting

```markdown
===============================================================================
 Language            Files        Lines         Code     Comments       Blanks
===============================================================================
 C                      13         1971         1501           52          418
 C Header               17         1977          700         1042          235
===============================================================================
 Total                  30         3948         2201         1094          653
===============================================================================
```

## 👨‍💻 Authors

- **<abalyberdin@whoosh.bike>** — initial MVP  
- **<vignatov@whoosh.bike>** — improvements, refactoring
- **<akrestinin@whoosh.bike>** — project separation (for submodule usage), main structure, PC/MCU examples  
- **<sh@whoosh.bike>** — UX improvements, extra features, documentation
- **<eshamaev@whoosh.bike>**  — CI/CD, docs deployment, high-level PC command app

## ⚖️ License

This project is licensed under the `MIT License`.

You are free to use, modify, and distribute this software in both commercial and non-commercial projects, provided that the original copyright notice and this permission notice are included.

![Shell](img/shell_wide.png)
