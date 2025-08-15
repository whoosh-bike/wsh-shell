

# File wsh\_shell\_cmd\_def.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd\_def.h**](wsh__shell__cmd__def_8h.md)

[Go to the source code of this file](wsh__shell__cmd__def_8h_source.md)

_Declaration of default shell commands and their option tables._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_cmd.h"`
* `#include "wsh_shell_history.h"`
* `#include "wsh_shell_types.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellDefCmd\_GetPtr**](#function-wshshelldefcmd_getptr) (void) <br>_Get a pointer to the default shell command structure._  |




























## Detailed Description


This header defines macros and functions for registering and executing a set of built-in shell commands. Each command is associated with a predefined option table and an executable handler.


These default commands provide core shell functionality such as help, user info, clearing the screen, and managing command history.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Functions Documentation




### function WshShellDefCmd\_GetPtr 

_Get a pointer to the default shell command structure._ 
```C++
const WshShellCmd_t * WshShellDefCmd_GetPtr (
    void
) 
```



This function returns a constant pointer to the static command structure WshShellDefCmd, which describes the default command used for configuring and querying the shell interface.




**Returns:**

const WshShellCmd\_t\* Pointer to the default command structure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd_def.h`

