

# File wsh\_shell\_autocomplete.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_autocomplete.h**](wsh__shell__autocomplete_8h.md)

[Go to the source code of this file](wsh__shell__autocomplete_8h_source.md)

_Command-line autocompletion for shell commands._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_cmd.h"`
* `#include "wsh_shell_cmd_def.h"`
* `#include "wsh_shell_io.h"`
* `#include "wsh_shell_str.h"`
* `#include "wsh_shell_types.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  WshShell\_Bool\_t | [**WshShellAutocomplete\_Try**](#function-wshshellautocomplete_try) (WshShell\_Char\_t \* pInBuff, WshShell\_Size\_t inBuffLen, [**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands) <br>_Attempts to autocomplete the command in the input buffer._  |




























## Detailed Description


This module provides autocompletion support for the shell interface. It enables the user to complete partially typed commands based on the currently registered command set. The algorithm performs prefix matching and modifies the input buffer accordingly:



* If **no match** is found, the input remains unchanged.
* If **exactly one match** is found, the input is completed with that command.
* If **multiple matches** exist with a **common prefix**, the input is extended to the longest common prefix.




Autocompletion improves user experience and reduces typing effort in shell environments.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Functions Documentation




### function WshShellAutocomplete\_Try 

_Attempts to autocomplete the command in the input buffer._ 
```C++
WshShell_Bool_t WshShellAutocomplete_Try (
    WshShell_Char_t * pInBuff,
    WshShell_Size_t inBuffLen,
    WshShellCmd_Table_t * pShellCommands
) 
```



Scans the registered command table and tries to match the current input as a prefix. If possible, it autocompletes the input buffer:



* If no matches are found, the buffer is not changed.
* If one match is found, the buffer is completed with the full command name.
* If multiple matches are found with a shared prefix, the buffer is extended to the longest matchable prefix.




This function is typically triggered by the user pressing the Tab key.




**Parameters:**


* `pInBuff` Pointer to the input buffer to modify. 
* `inBuffLen` Total size of the input buffer. 
* `pShellCommands` Pointer to the command table to search.



**Return value:**


* `true` The buffer was modified (i.e., autocomplete was applied). 
* `false` No autocomplete was possible (zero matches or ambiguous match). 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_autocomplete.h`

