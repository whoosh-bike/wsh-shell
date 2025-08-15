

# File wsh\_shell\_autocomplete.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_autocomplete.c**](wsh__shell__autocomplete_8c.md)

[Go to the source code of this file](wsh__shell__autocomplete_8c_source.md)



* `#include "wsh_shell_autocomplete.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  WshShell\_Bool\_t | [**WshShellAutocomplete\_Try**](#function-wshshellautocomplete_try) (WshShell\_Char\_t \* pInBuff, WshShell\_Size\_t inBuffLen, [**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands) <br>_Attempts to autocomplete the command in the input buffer._  |




























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
The documentation for this class was generated from the following file `src/wsh_shell_autocomplete.c`

