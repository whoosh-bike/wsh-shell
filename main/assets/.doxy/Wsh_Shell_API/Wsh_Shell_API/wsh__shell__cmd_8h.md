

# File wsh\_shell\_cmd.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd.h**](wsh__shell__cmd_8h.md)

[Go to the source code of this file](wsh__shell__cmd_8h_source.md)

_Shell command object definition and public API._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_io.h"`
* `#include "wsh_shell_option.h"`
* `#include "wsh_shell_str.h"`
* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellCmd**](structWshShellCmd.md) <br>_Descriptor for a shell command._  |
| struct | [**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) <br>_Table of shell commands._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef WSH\_SHELL\_RET\_STATE\_t(\* | [**WshShellCmdHandler\_t**](#typedef-wshshellcmdhandler_t)  <br>_Function pointer type for shell command execution._  |
| typedef struct [**WshShellCmd**](structWshShellCmd.md) | [**WshShellCmd\_t**](#typedef-wshshellcmd_t)  <br>_Descriptor for a shell command._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellCmd\_Attach**](#function-wshshellcmd_attach) ([**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands, const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmdTable, WshShell\_Size\_t cmdNum) <br>_Initializes the command table._  |
|  void | [**WshShellCmd\_DeAttach**](#function-wshshellcmd_deattach) ([**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands) <br>_Frees or resets the command table._  |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellCmd\_GetCmdByIndex**](#function-wshshellcmd_getcmdbyindex) ([**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands, WshShell\_Size\_t idx) <br>_Retrieves a command by index._  |
|  WshShell\_Size\_t | [**WshShellCmd\_GetCmdNum**](#function-wshshellcmd_getcmdnum) ([**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands) <br>_Returns the number of registered commands._  |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellCmd\_GetOptValue**](#function-wshshellcmd_getoptvalue) ([**WshShellOption\_Context\_t**](structWshShellOption__Context__t.md) \* pOptCtx, WshShell\_Size\_t argc, const WshShell\_Char\_t \* pArgv, WshShell\_Size\_t valueSize, void \* pValue) <br>_Retrieves the value associated with a parsed command option._  |
|  [**WshShellOption\_Context\_t**](structWshShellOption__Context__t.md) | [**WshShellCmd\_ParseOpt**](#function-wshshellcmd_parseopt) (const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmd, WshShell\_Size\_t argc, const WshShell\_Char\_t \* pArgv, WshShell\_Size\_t \* pTokenPos) <br>_Parses a command-line option for a given shell command._  |
|  void | [**WshShellCmd\_PrintInfo**](#function-wshshellcmd_printinfo) (const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmd) <br>_Prints detailed information about a shell command and its options._  |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellCmd\_SearchCmd**](#function-wshshellcmd_searchcmd) ([**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) \* pShellCommands, const WshShell\_Char\_t \* pcCmdName) <br>_Finds a command by its name._  |




























## Detailed Description


Provides data structures and APIs for defining and handling shell commands, including command execution, option parsing, and help information printing.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### typedef WshShellCmdHandler\_t 

_Function pointer type for shell command execution._ 
```C++
typedef WSH_SHELL_RET_STATE_t(* WshShellCmdHandler_t) (const struct WshShellCmd *pcCmd, WshShell_Size_t argc, const WshShell_Char_t *pArgv[], void *pCtx);
```





**Parameters:**


* `pcCmd` Pointer to the command descriptor. 
* `argc` Number of command-line arguments. 
* `pArgv` Array of argument strings.



**Returns:**

Command execution result (success, error, etc.). 





        

<hr>



### typedef WshShellCmd\_t 

_Descriptor for a shell command._ 
```C++
typedef struct WshShellCmd WshShellCmd_t;
```




<hr>
## Public Functions Documentation




### function WshShellCmd\_Attach 

_Initializes the command table._ 
```C++
WSH_SHELL_RET_STATE_t WshShellCmd_Attach (
    WshShellCmd_Table_t * pShellCommands,
    const WshShellCmd_t * pcCmdTable,
    WshShell_Size_t cmdNum
) 
```





**Parameters:**


* `pShellCommands` Pointer to the shell command table. 
* `pcCmdTable` Pointer to the static list of commands. 
* `cmdNum` Number of commands in the list.



**Returns:**

WSH\_SHELL\_RET\_STATE\_SUCCESS on success. 





        

<hr>



### function WshShellCmd\_DeAttach 

_Frees or resets the command table._ 
```C++
void WshShellCmd_DeAttach (
    WshShellCmd_Table_t * pShellCommands
) 
```





**Parameters:**


* `pShellCommands` Pointer to the command table. 




        

<hr>



### function WshShellCmd\_GetCmdByIndex 

_Retrieves a command by index._ 
```C++
const WshShellCmd_t * WshShellCmd_GetCmdByIndex (
    WshShellCmd_Table_t * pShellCommands,
    WshShell_Size_t idx
) 
```





**Parameters:**


* `pShellCommands` Pointer to the command table. 
* `idx` Command index.



**Returns:**

Pointer to the command descriptor or NULL if out-of-bounds. 





        

<hr>



### function WshShellCmd\_GetCmdNum 

_Returns the number of registered commands._ 
```C++
WshShell_Size_t WshShellCmd_GetCmdNum (
    WshShellCmd_Table_t * pShellCommands
) 
```





**Parameters:**


* `pShellCommands` Pointer to the command table.



**Returns:**

Number of commands. 





        

<hr>



### function WshShellCmd\_GetOptValue 

_Retrieves the value associated with a parsed command option._ 
```C++
WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue (
    WshShellOption_Context_t * pOptCtx,
    WshShell_Size_t argc,
    const WshShell_Char_t * pArgv,
    WshShell_Size_t valueSize,
    void * pValue
) 
```



This function extracts the value of a command-line option parsed by `WshShellCmd_ParseOpt()`, based on its type. Supported types include:
* `WSH_SHELL_OPTION_STR` → copied to `pValue` as a string
* `WSH_SHELL_OPTION_INT` → parsed as integer and written to `pValue`
* `WSH_SHELL_OPTION_FLOAT`→ parsed as float and written to `pValue`




The function expects the option to be followed by its value in `pArgv`. All other option types return `WSH_SHELL_RET_STATE_ERR_EMPTY`.




**Parameters:**


* `pOptCtx` Pointer to parsed option descriptor. 
* `argc` Argument count. 
* `pArgv` Argument vector (array of strings). 
* `valueSize` Maximum size of the output buffer (used for strings). 
* `pValue` Output buffer for value (string/int/float based on type).



**Returns:**

WSH\_SHELL\_RET\_STATE\_SUCCESS if value was successfully retrieved, 




**Returns:**

WSH\_SHELL\_RET\_STATE\_ERR\_PARAM if input parameters are invalid, 




**Returns:**

WSH\_SHELL\_RET\_STATE\_ERR\_EMPTY if the option doesn't accept a value, 




**Returns:**

WSH\_SHELL\_RET\_STATE\_ERR\_OVERFLOW if argument list is too short. 





        

<hr>



### function WshShellCmd\_ParseOpt 

_Parses a command-line option for a given shell command._ 
```C++
WshShellOption_Context_t WshShellCmd_ParseOpt (
    const WshShellCmd_t * pcCmd,
    WshShell_Size_t argc,
    const WshShell_Char_t * pArgv,
    WshShell_Size_t * pTokenPos
) 
```



This function analyzes the current token in the argument list and attempts to identify a valid option defined in the given command's option table. If a valid option is found, its descriptor is returned. The token position (`*pTokenPos`) is advanced accordingly depending on the number of arguments that the option consumes.


If only the command name is present (i.e., `argc == 1`), the function checks for an option of type `WSH_SHELL_OPTION_NO` (indicating the command may be executed without any parameters).




**Parameters:**


* `pcCmd` Pointer to the shell command definition. 
* `argc` Number of arguments in the input array. 
* `pArgv` Array of argument strings. 
* `pTokenPos` Pointer to the current token position in `pArgv`. Will be updated to point past the parsed option.



**Returns:**

A filled option descriptor if a matching option is found, or an empty descriptor if not. 





        

<hr>



### function WshShellCmd\_PrintInfo 

_Prints detailed information about a shell command and its options._ 
```C++
void WshShellCmd_PrintInfo (
    const WshShellCmd_t * pcCmd
) 
```



This function outputs the help text and a formatted table describing each available option for the specified command. For each option, the following fields are printed:
* Short name
* Long name
* Option type (e.g., INT, STR, FLOAT)
* Access flags (read/write/etc.)
* Description/help string




Options of type `WSH_SHELL_OPTION_NO` and `WSH_SHELL_OPTION_WAITS_INPUT` are skipped.


This is typically used for displaying help information when a user requests `--help` for a specific command.




**Parameters:**


* `pcCmd` Pointer to the command descriptor whose options should be printed. 




        

<hr>



### function WshShellCmd\_SearchCmd 

_Finds a command by its name._ 
```C++
const WshShellCmd_t * WshShellCmd_SearchCmd (
    WshShellCmd_Table_t * pShellCommands,
    const WshShell_Char_t * pcCmdName
) 
```





**Parameters:**


* `pShellCommands` Pointer to the command table. 
* `pcCmdName` Command name string.



**Returns:**

Pointer to the matching command descriptor or NULL if not found. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd.h`

