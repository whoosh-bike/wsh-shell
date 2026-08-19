

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
* `#include "wsh_shell_misc.h"`
* `#include "wsh_shell_promptwait.h"`
* `#include "wsh_shell_ps1_custom.h"`
* `#include "wsh_shell_session.h"`
* `#include "wsh_shell_str.h"`
* `#include "wsh_shell_types.h"`
* `#include "wsh_shell_user.h"`
* `#include "wsh_shell_version.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellAuthCtx\_t**](structWshShellAuthCtx__t.md) <br>_Temporary authentication context for login input handling._  |
| struct | [**WshShellExtCallbacks\_t**](structWshShellExtCallbacks__t.md) <br>_Optional external callbacks invoked on user session changes._  |
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
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShell\_Init**](#function-wshshell_init) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t \* pcDevName, const WshShell\_Char\_t \* pcCustomHeader, [**WshShellExtCallbacks\_t**](structWshShellExtCallbacks__t.md) \* pExtClbks) <br>_Initialize a shell instance._  |
|  void | [**WshShell\_InsertChar**](#function-wshshell_insertchar) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t symbol) <br>_Process a new character entered by the user._  |
|  WshShell\_Bool\_t | [**WshShell\_IsAuth**](#function-wshshell_isauth) (const [**WshShell\_t**](structWshShell__t.md) \* pcShell) <br>_Check if a user is currently authenticated._  |
|  WshShell\_Bool\_t | [**WshShell\_SessionArm**](#function-wshshell_sessionarm) ([**WshShell\_t**](structWshShell__t.md) \* pShell, WshShell\_U32\_t reboots) <br>_Arm cross-reboot login persistence for the current user._  |
|  WshShell\_Bool\_t | [**WshShell\_SessionIsKeepActive**](#function-wshshell_sessioniskeepactive) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br>_Whether an armed keep-session is currently active (budget remaining)._  |
|  WshShell\_U32\_t | [**WshShell\_SessionRebootsLeft**](#function-wshshell_sessionrebootsleft) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br>_Remaining reboot budget of the armed session, or 0 if none._  |
|  WshShell\_Bool\_t | [**WshShell\_SessionRestore**](#function-wshshell_sessionrestore) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br>_Restore a previously armed login without a password prompt._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**COMPILER**](wsh__shell_8h.md#define-compiler)  `"Unknown Compiler"`<br> |
| define  | [**OS\_NAME**](wsh__shell_8h.md#define-os_name)  `WSH\_SHELL\_TARGET\_OS`<br> |
| define  | [**WSH\_SHELL\_HEADER**](wsh__shell_8h.md#define-wsh_shell_header)  `"\                \_\_               \_\_         \_\_\_\_  \r\n\ \_      \_\_\_\_\_\_\_/ /\_        \_\_\_\_\_/ /\_  \_\_\_  / / /  \r\n\\| \| /\| / / \_\_\_/ \_\_ \\\_\_\_\_\_\_/ \_\_\_/ \_\_ \\/ \_ \\/ / /\r\n\\| \|/ \|/ (\_\_  ) / / /\_\_\_\_\_(\_\_  ) / / /  \_\_/ / /    \r\n\\|\_\_/\|\_\_/\_\_\_\_/\_/ /\_/     /\_\_\_\_/\_/ /\_/\\\_\_\_/\_/\_/    \r\n\\r\n"`<br> |

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
    WshShellExtCallbacks_t * pExtClbks
) 
```



Initializes internal subsystems, assigns device name and optional header, and installs optional external callbacks.




**Parameters:**


* `pShell` Pointer to the shell instance. 
* `pcDevName` Device name (e.g., "ttyS0" or "shell0"). 
* `pcCustomHeader` Optional welcome banner for this instance. When NULL, WSH\_SHELL\_HEADER is used: either the one defined in wsh\_shell\_cfg.h or the built-in wsh-shell logo. 
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
    const WshShell_t * pcShell
) 
```





**Parameters:**


* `pShell` Shell instance. 



**Returns:**

`WSH_SHELL_TRUE` if a user is authenticated, `WSH_SHELL_FALSE` otherwise. 





        

<hr>



### function WshShell\_SessionArm 

_Arm cross-reboot login persistence for the current user._ 
```C++
WshShell_Bool_t WshShell_SessionArm (
    WshShell_t * pShell,
    WshShell_U32_t reboots
) 
```



Records the current login so it can be restored without a password after up to `reboots` reboots. While a session is armed the integrator can also suppress the inactivity auto-logout (see [**WshShell\_SessionIsKeepActive()**](wsh__shell_8h.md#function-wshshell_sessioniskeepactive)). Passing 0 clears any armed session. Requires the shell to have session I/O installed ([**WshShellSession\_Init()**](wsh__shell__session_8h.md#function-wshshellsession_init)) and a user currently logged in.




**Parameters:**


* `pShell` Shell instance. 
* `reboots` Number of reboots the login may survive (0 clears). 



**Returns:**

`true` if the request was applied. 





        

<hr>



### function WshShell\_SessionIsKeepActive 

_Whether an armed keep-session is currently active (budget remaining)._ 
```C++
WshShell_Bool_t WshShell_SessionIsKeepActive (
    WshShell_t * pShell
) 
```



The integrator can use this to block the inactivity auto-logout while the host asked to stay logged in.




**Parameters:**


* `pShell` Shell instance. 



**Returns:**

`true` if a valid session with remaining reboot budget is stored. 





        

<hr>



### function WshShell\_SessionRebootsLeft 

_Remaining reboot budget of the armed session, or 0 if none._ 
```C++
WshShell_U32_t WshShell_SessionRebootsLeft (
    WshShell_t * pShell
) 
```





**Parameters:**


* `pShell` Shell instance. 



**Returns:**

Reboots left, or 0 when no valid session is armed. 





        

<hr>



### function WshShell\_SessionRestore 

_Restore a previously armed login without a password prompt._ 
```C++
WshShell_Bool_t WshShell_SessionRestore (
    WshShell_t * pShell
) 
```



Intended to be called once at start-up, after the user table is attached. On success the current user and PS1 are set and the Auth callback is invoked, exactly as a normal login would. Consumes one reboot from the budget.




**Parameters:**


* `pShell` Shell instance. 



**Returns:**

`true` if a valid session was restored. 





        

<hr>
## Macro Definition Documentation





### define COMPILER 

```C++
#define COMPILER `"Unknown Compiler"`
```




<hr>



### define OS\_NAME 

```C++
#define OS_NAME `WSH_SHELL_TARGET_OS`
```




<hr>



### define WSH\_SHELL\_HEADER 

```C++
#define WSH_SHELL_HEADER `"\                __               __         ____  \r\n\ _      _______/ /_        _____/ /_  ___  / / /  \r\n\| | /| / / ___/ __ \\______/ ___/ __ \\/ _ \\/ / /\r\n\| |/ |/ (__  ) / / /_____(__  ) / / /  __/ / /    \r\n\|__/|__/____/_/ /_/     /____/_/ /_/\\___/_/_/    \r\n\\r\n"`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.h`

