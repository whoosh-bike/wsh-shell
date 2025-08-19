# Wsh-Shell

Wsh-Shell is a lightweight, portable, and fully static shell interpreter written in C, designed for embedded systems. It requires no dynamic memory allocation and is built to run in constrained environments like microcontrollers, either bare-metal or under RTOS (e.g., FreeRTOS).

## 🚀 Features

- **Cross-platform, highly portable** — only one header file to include  
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

## 💾 Memory footprint

- Build options: cortex-m7, `-O1` optimization
- sizeof(WshShell_t) = 336 bytes

| Config                              | FLASH, KB | RAM, KB | Comment                                              |
| ----------------------------------- | --------- | ------- | ---------------------------------------------------- |
| All features disabled               | 3.29102   | 0.0     |                                                      |
| +WSH_SHELL_PRINT_SYS/INFO/WARN/ERR  | 3.58984   | 0.0     |                                                      |
| +WSH_SHELL_DEF_COMMAND              | 4.61914   | 0.27343 |                                                      |
| +WSH_SHELL_PRINT_HELP               | 4.65234   | 0.27343 | Could be usefull on huge amount of external commands |
| +WSH_SHELL_CMD_PRINT_OPT_OVERVIEW   | 4.90625   | 0.27343 |                                                      |
| +WSH_SHELL_INTERACTIVE_MODE         | 5.00391   | 0.27343 |                                                      |
| +WSH_SHELL_HISTORY                  | 6.16016   | 0.27343 |                                                      |
| +WSH_SHELL_AUTOCOMPLETE             | 6.71094   | 0.27343 |                                                      |
| +WSH_SHELL_PS1_CUSTOM               | 7.18359   | 0.27343 |                                                      |
| +WSH_SHELL_PROMPT_WAIT              | 7.29883   | 0.27343 |                                                      |

## ⌨️ Code counting

```txt
===============================================================================
 Language            Files        Lines         Code     Comments       Blanks
===============================================================================
 C                      12         1928         1470           52          406
 C Header               16         1936          679         1028          229
===============================================================================
 Total                  28         3864         2149         1080          635
===============================================================================
```

## Authors

- **<abalyberdin@whoosh.bike>** — initial MVP  
- **<vignatov@whoosh.bike>** — improvements, refactoring
- **<akrestinin@whoosh.bike>** — project separation (for submodule usage), main structure, PC/MCU examples  
- **<sh@whoosh.bike>** — UX improvements, extra features, documentation
- **<eshamaev@whoosh.bike>**  — CI/CD, docs deployment, high-level PC command app
