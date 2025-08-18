

# File wsh\_shell.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell.h**](wsh__shell_8h.md)

[Go to the source code of this file](wsh__shell_8h_source.md)

_Core shell interface for command parsing, user authentication, and terminal interaction._ [More...](#detailed-description)

* `#include "wsh_shell_autocomplete.h"`
* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_cmd.h"`
* `#include "wsh_shell_cmd_def.h"`
* `#include "wsh_shell_esc.h"`
* `#include "wsh_shell_history.h"`
* `#include "wsh_shell_interact.h"`
* `#include "wsh_shell_io.h"`
* `#include "wsh_shell_promptwait.h"`
* `#include "wsh_shell_ps1_custom.h"`
* `#include "wsh_shell_str.h"`
* `#include "wsh_shell_types.h"`
* `#include "wsh_shell_user.h"`
* `#include "wsh_shell_version.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShell\_AuthContext\_t**](structWshShell__AuthContext__t.md) <br>_Temporary authentication context for login input handling._  |
| struct | [**WshShell\_ExtCallbacks\_t**](structWshShell__ExtCallbacks__t.md) <br>_Optional external callbacks invoked on user session changes._  |
| struct | [**WshShell\_t**](structWshShell__t.md) <br>_Main shell structure containing state, configuration, user context, and subsystems._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef void(\* | [**WshShell\_ExtClbk\_t**](#typedef-wshshell_extclbk_t)  <br>_Type of external callback used in shell extension hooks._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  WshShell\_Bool\_t | [**WshShell\_Auth**](#function-wshshell_auth) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t \* pcLogin, const WshShell\_Char\_t \* pcPass) <br>_Attempt to authenticate a user with given login credentials._  |
|  void | [**WshShell\_DeAuth**](#function-wshshell_deauth) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t \* pcReason) <br>_De-authenticate the currently logged-in user._  |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShell\_Init**](#function-wshshell_init) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t \* pcDevName, const WshShell\_Char\_t \* pcCustomHeader, [**WshShell\_ExtCallbacks\_t**](structWshShell__ExtCallbacks__t.md) \* pExtClbks) <br>_Initialize a shell instance._  |
|  void | [**WshShell\_InsertChar**](#function-wshshell_insertchar) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t symbol) <br>_Process a new character entered by the user._  |
|  WshShell\_Bool\_t | [**WshShell\_IsAuth**](#function-wshshell_isauth) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br>_Check if a user is currently authenticated._  |




























## Detailed Description


This header provides the main shell object (`WshShell_t`), core APIs for initialization, input processing, user session management, and support for optional external callbacks.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### typedef WshShell\_ExtClbk\_t 

_Type of external callback used in shell extension hooks._ 
```C++
typedef void(* WshShell_ExtClbk_t) (void *pCtx);
```




<hr>
## Public Functions Documentation




### function WshShell\_Auth 

_Attempt to authenticate a user with given login credentials._ 
```C++
WshShell_Bool_t WshShell_Auth (
    WshShell_t * pShell,
    const WshShell_Char_t * pcLogin,
    const WshShell_Char_t * pcPass
) 
```



Updates the current user context on success, or leaves it unchanged on failure.




**Parameters:**


* `pShell` Shell instance. 
* `pcLogin` User name. 
* `pcPass` Password. 



**Returns:**

Is auth OK? 





        

<hr>



### function WshShell\_DeAuth 

_De-authenticate the currently logged-in user._ 
```C++
void WshShell_DeAuth (
    WshShell_t * pShell,
    const WshShell_Char_t * pcReason
) 
```



Resets the user context and optionally triggers the DeAuth callback.




**Parameters:**


* `pShell` Shell instance. 
* `pcReason` Reason or source of deauth. 




        

<hr>



### function WshShell\_Init 

_Initialize a shell instance._ 
```C++
WSH_SHELL_RET_STATE_t WshShell_Init (
    WshShell_t * pShell,
    const WshShell_Char_t * pcDevName,
    const WshShell_Char_t * pcCustomHeader,
    WshShell_ExtCallbacks_t * pExtClbks
) 
```



Initializes internal subsystems, assigns device name and optional header, and installs optional external callbacks.




**Parameters:**


* `pShell` Pointer to the shell instance. 
* `pcDevName` Device name (e.g., "ttyS0" or "shell0"). 
* `pcCustomHeader` Optional header string (can be NULL). 
* `pExtClbks` Pointer to external callback structure (can be NULL). 



**Returns:**

Initialization status code. 





        

<hr>



### function WshShell\_InsertChar 

_Process a new character entered by the user._ 
```C++
void WshShell_InsertChar (
    WshShell_t * pShell,
    const WshShell_Char_t symbol
) 
```



Handles interactive editing, history navigation, or command execution if input is complete.




**Parameters:**


* `pShell` Shell instance. 
* `symbol` Character to insert. 




        

<hr>



### function WshShell\_IsAuth 

_Check if a user is currently authenticated._ 
```C++
WshShell_Bool_t WshShell_IsAuth (
    WshShell_t * pShell
) 
```





**Parameters:**


* `pShell` Shell instance. 



**Returns:**

`WSH_SHELL_TRUE` if a user is authenticated, `WSH_SHELL_FALSE` otherwise. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.h`

