

# File wsh\_shell\_ps1\_custom.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_ps1\_custom.h**](wsh__shell__ps1__custom_8h.md)

[Go to the source code of this file](wsh__shell__ps1__custom_8h_source.md)

_Shell prompt (PS1) customization and formatting._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_io.h"`
* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShell\_PS1Data\_t**](structWshShell__PS1Data__t.md) <br>_Runtime data used for PS1 (prompt string) expansion._  |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShell\_GeneratePS1**](#function-wshshell_generateps1) (WshShell\_Char\_t \* pPS1, [**WshShell\_PS1Data\_t**](structWshShell__PS1Data__t.md) \* pPS1Data) <br>_Applies the PS1 template and generates the final PS1 string._  |




























## Detailed Description


Provides functionality for building and customizing the shell prompt (`PS1`). This includes dynamic substitution of runtime values such as user name, device name, and ANSI styling codes (colors, bold, reset).




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Functions Documentation




### function WshShell\_GeneratePS1 

_Applies the PS1 template and generates the final PS1 string._ 
```C++
void WshShell_GeneratePS1 (
    WshShell_Char_t * pPS1,
    WshShell_PS1Data_t * pPS1Data
) 
```



Expands a predefined PS1 template (`WSH_SHELL_PS1_TEMPLATE`) into a complete PS1 string, substituting special format specifiers with runtime values like username, device name, and ANSI escape sequences for styling (colors, bold, reset).


Supported format specifiers in the template:
* `u` — current username (from `pUser`)
* `d` — current device name (from `pDevice`)
* `cN` — ANSI color code from predefined color map (0 ≤ N ≤ 9)
* `b` — ANSI escape sequence for bold text
* `r` — ANSI escape sequence to reset all styles
* Any unknown `%` sequence is copied verbatim as `X`.




The output is truncated if it would exceed `WSH_SHELL_PS1_MAX_LEN - 1`. The result is always null-terminated.




**Parameters:**


* `pPS1` Output buffer for the final PS1 string. Must be at least `WSH_SHELL_PS1_MAX_LEN` in size. 
* `pPS1Data` Pointer to the user name, divice name or other strings. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_ps1_custom.h`

