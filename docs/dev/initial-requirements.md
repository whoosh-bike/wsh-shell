# Initial requirements

The **Wsh-Shell** module must satisfy two main categories of requirements:  
**User Requirements** (focused on usability) and **Development Requirements** (focused on integration and maintainability).

---

## User Requirements

1. **Command Flags Support**  
    - Support both **long** (`--long-option`) and **short** (`-s`) flag names.  
    - Commands follow the standard syntax
    - Long flags may exist without a corresponding short form.

2. **Command Auto-Completion**  
    - Pressing **Tab** should auto-complete the current command or suggest possible completions.

3. **Command History**  
    - Store history of executed commands.  
    - Navigation: **↑** to recall the previous command, **↓** to recall the next one.

4. **Line Editing & Cursor Movement**  
    - Move the cursor within the input line using **←** and **→** keys.  
    - Support in-place character insertion and deletion using **Backspace**.

5. **Hotkey Combinations**  
    - Recognize and handle standard key combinations (e.g., **Ctrl + C**, **Ctrl + D**).

6. **User Authorization**  
    - Require user login with a password before executing commands.

7. **Hardware Interfaces Switching**  
    - Allow switching between hardware interfaces (e.g., USB, UART) **without reflashing** the firmware.

8. **Sub-Shell Support**  
    - Enable running a separate shell instance in a dedicated thread for interacting with external devices.

9. **Cross-Platform Operation**  
    - Run on various platforms without modification.

---

## Development Requirements

1. **Single Include Entry Point**  
    - Only one header file is required to integrate Wsh-Shell into a project.

2. **Modular Command Registration**  
    - Developers can define and register module-specific commands directly within their module.

3. **Configurable Portability**  
    - Cross-platform compatibility is achieved via a dedicated configuration file.

4. **Git Submodule Integration**  
    - Wsh-Shell should be easy to integrate into any project as a Git submodule.

5. **Dedicated Sub-Shell Task**  
    - In RTOS-based systems, run sub-Shell routines in a separate FreeRTOS task.

6. **Hot-Swappable Interfaces**  
    - Switching between hardware interfaces must be possible at runtime.

7. **Feature Modularity**  
    - Features such as history, autocomplete, and authorization must be implemented as **independent submodules** that can be disabled without breaking the core functionality.

8. **Mandatory Authorization**  
    - Authorization is always required except in debug builds.

9. **Clear Input Parsing Model**  
    - Input parsing should follow a clearly defined state machine.

10. **Sub-Shell I/O Redirection**  
     - When launching a sub-Shell, redirect input/output processing to the child shell instance.

11. **Hotkey & Combination Parsing**  
     - Ability to detect and handle hotkey sequences and special symbol combinations.

---

## Why not microrl / tinysh / linenoise?

```markdown
¯\_(ツ)_/¯
```

---

## Notes

- The above requirements define both **what the user sees** and **how the developer integrates** the Wsh-Shell.
- Future improvements might include scripting support, persistent command history storage, and remote shell capability.
