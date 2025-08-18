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

## 💾 Memory footprint

Used flags: `-O1`, `-mcpu=cortex-m3`, `-mfloat-abi=soft`

| Config                            | FLASH | RAM    |
| --------------------------------- | ----- | ------ |
| All features enabled              | 12 KB | 1.5 KB |
| + WSH_SHELL_HISTORY               | 6 KB  | 0.5 KB |
| + WSH_SHELL_PS1_CUSTOM            | 6 KB  | 0.5 KB |
| + WSH_SHELL_AUTOCOMPLETE          | 6 KB  | 0.5 KB |
| + WSH_SHELL_PRINT_SYS_ENABLE      | 6 KB  | 0.5 KB |
| + WSH_SHELL_PRINT_INFO_ENABLE     | 6 KB  | 0.5 KB |
| + WSH_SHELL_PRINT_WARN_ENABLE     | 6 KB  | 0.5 KB |
| + WSH_SHELL_PRINT_ERR_ENABLE      | 6 KB  | 0.5 KB |
| + WSH_SHELL_PRINT_OPT_HELP_ENABLE | 6 KB  | 0.5 KB |

## ⌨️ Code counting

```txt
===============================================================================
 Language            Files        Lines         Code     Comments       Blanks
===============================================================================
 C                      11         1806         1385           52          369
 C Header               15         1856          660          970          226
===============================================================================
 Total                  26         3662         2045         1022          595
===============================================================================
```

## Authors

- **<abalyberdin@whoosh.bike>** — initial MVP  
- **<vignatov@whoosh.bike>** — improvements, refactoring
- **<akrestinin@whoosh.bike>** — project separation (for submodule usage), main structure, PC/MCU examples  
- **<sh@whoosh.bike>** — UX improvements, extra features, documentation
- **<eshamaev@whoosh.bike>**  — CI/CD, docs deployment, high-level PC command app
