

# File wsh\_shell.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell.c**](wsh__shell_8c.md)

[Go to the source code of this file](wsh__shell_8c_source.md)



* `#include "wsh_shell.h"`





































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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShell\_AuthHandler**](#function-wshshell_authhandler) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br> |
|  void | [**WshShell\_ExitInteractive**](#function-wshshell_exitinteractive) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br> |
|  void | [**WshShell\_InvitationPrint**](#function-wshshell_invitationprint) (const [**WshShell\_t**](structWshShell__t.md) \* pcShell) <br> |
|  void | [**WshShell\_StringHandler**](#function-wshshell_stringhandler) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br> |
|  void | [**WshShell\_StringInteractHandler**](#function-wshshell_stringinteracthandler) ([**WshShell\_t**](structWshShell__t.md) \* pShell) <br> |
|  void | [**WshShell\_Stub\_ExtClbk**](#function-wshshell_stub_extclbk) (void \* pCtx) <br> |
|  void | [**WshShell\_SymbolHandler**](#function-wshshell_symbolhandler) ([**WshShell\_t**](structWshShell__t.md) \* pShell, const WshShell\_Char\_t symbol) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**SHELL\_SAVE\_PREV\_AND\_RETURN**](wsh__shell_8c.md#define-shell_save_prev_and_return) (pShell, sym) `/* multi line expression */`<br> |
| define  | [**WSH\_SHELL\_INTER\_CMD\_EXISTS**](wsh__shell_8c.md#define-wsh_shell_inter_cmd_exists) (\_sh\_) `((\_sh\_)-&gt;Interact.Handler != NULL)`<br> |
| define  | [**WSH\_SHELL\_TMP\_LOGIN\_IS\_EMPTY**](wsh__shell_8c.md#define-wsh_shell_tmp_login_is_empty) (\_sh\_) `((\_sh\_)-&gt;TmpAuth.Login[0] == 0)`<br> |
| define  | [**WSH\_SHELL\_TMP\_PASS\_IS\_EMPTY**](wsh__shell_8c.md#define-wsh_shell_tmp_pass_is_empty) (\_sh\_) `((\_sh\_)-&gt;TmpAuth.Pass[0] == 0)`<br> |
| define  | [**WSH\_SHELL\_USER\_IS\_AUTH**](wsh__shell_8c.md#define-wsh_shell_user_is_auth) (\_sh\_) `((\_sh\_)-&gt;CurrUser != NULL)`<br> |

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
## Public Static Functions Documentation




### function WshShell\_AuthHandler 

```C++
static void WshShell_AuthHandler (
    WshShell_t * pShell
) 
```




<hr>



### function WshShell\_ExitInteractive 

```C++
static void WshShell_ExitInteractive (
    WshShell_t * pShell
) 
```




<hr>



### function WshShell\_InvitationPrint 

```C++
static void WshShell_InvitationPrint (
    const WshShell_t * pcShell
) 
```




<hr>



### function WshShell\_StringHandler 

```C++
static void WshShell_StringHandler (
    WshShell_t * pShell
) 
```




<hr>



### function WshShell\_StringInteractHandler 

```C++
static void WshShell_StringInteractHandler (
    WshShell_t * pShell
) 
```




<hr>



### function WshShell\_Stub\_ExtClbk 

```C++
static void WshShell_Stub_ExtClbk (
    void * pCtx
) 
```




<hr>



### function WshShell\_SymbolHandler 

```C++
static void WshShell_SymbolHandler (
    WshShell_t * pShell,
    const WshShell_Char_t symbol
) 
```




<hr>
## Macro Definition Documentation





### define SHELL\_SAVE\_PREV\_AND\_RETURN 

```C++
#define SHELL_SAVE_PREV_AND_RETURN (
    pShell,
    sym
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_INTER\_CMD\_EXISTS 

```C++
#define WSH_SHELL_INTER_CMD_EXISTS (
    _sh_
) `((_sh_)->Interact.Handler != NULL)`
```




<hr>



### define WSH\_SHELL\_TMP\_LOGIN\_IS\_EMPTY 

```C++
#define WSH_SHELL_TMP_LOGIN_IS_EMPTY (
    _sh_
) `((_sh_)->TmpAuth.Login[0] == 0)`
```




<hr>



### define WSH\_SHELL\_TMP\_PASS\_IS\_EMPTY 

```C++
#define WSH_SHELL_TMP_PASS_IS_EMPTY (
    _sh_
) `((_sh_)->TmpAuth.Pass[0] == 0)`
```




<hr>



### define WSH\_SHELL\_USER\_IS\_AUTH 

```C++
#define WSH_SHELL_USER_IS_AUTH (
    _sh_
) `((_sh_)->CurrUser != NULL)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.c`

