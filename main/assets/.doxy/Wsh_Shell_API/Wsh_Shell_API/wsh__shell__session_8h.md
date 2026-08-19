

# File wsh\_shell\_session.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_session.h**](wsh__shell__session_8h.md)

[Go to the source code of this file](wsh__shell__session_8h_source.md)

_Persistent login session across reboots._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_misc.h"`
* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellSessionIO\_t**](structWshShellSessionIO__t.md) <br>_Function pointers for session persistence I/O._  |
| struct | [**WshShellSession\_t**](structWshShellSession__t.md) <br>_Persisted session descriptor with an integrity hash._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**WshShellSession\_t**](structWshShellSession__t.md)(\* | [**WshShellSession\_ReadHandler\_t**](#typedef-wshshellsession_readhandler_t)  <br>_Read the persisted session descriptor._  |
| typedef void(\* | [**WshShellSession\_WriteHandler\_t**](#typedef-wshshellsession_writehandler_t)  <br>_Persist the session descriptor._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellSession\_Clear**](#function-wshshellsession_clear) ([**WshShellSessionIO\_t**](structWshShellSessionIO__t.md) \* pSessIO) <br>_Drop any persisted session._  |
|  void | [**WshShellSession\_Init**](#function-wshshellsession_init) ([**WshShellSessionIO\_t**](structWshShellSessionIO__t.md) \* pSessIO, [**WshShellSession\_ReadHandler\_t**](wsh__shell__session_8h.md#typedef-wshshellsession_readhandler_t) readFn, [**WshShellSession\_WriteHandler\_t**](wsh__shell__session_8h.md#typedef-wshshellsession_writehandler_t) writeFn) <br>_Initialise the session system with custom I/O handlers._  |
|  WshShell\_Bool\_t | [**WshShellSession\_IsValid**](#function-wshshellsession_isvalid) (const [**WshShellSession\_t**](structWshShellSession__t.md) \* pcSession) <br>_Check whether a descriptor is a valid, active keep request._  |
|  [**WshShellSession\_t**](structWshShellSession__t.md) | [**WshShellSession\_Read**](#function-wshshellsession_read) ([**WshShellSessionIO\_t**](structWshShellSessionIO__t.md) \* pSessIO) <br>_Read the persisted descriptor._  |
|  void | [**WshShellSession\_Store**](#function-wshshellsession_store) ([**WshShellSessionIO\_t**](structWshShellSessionIO__t.md) \* pSessIO, [**WshShellSession\_t**](structWshShellSession__t.md) session) <br>_Persist a descriptor, computing its integrity hash first._  |




























## Detailed Description


Lets the host keep the current login valid across a bounded number of reboots, so a device that reboots mid-session (firmware bug, watchdog) does not force a re-login. The budget is a plain reboot counter — no wall-clock/RTC is needed: each restore consumes one reboot from the budget.


Storage is delegated to the integrator through read/write handlers (same pattern as the command history), so the shell core stays storage-agnostic. A no-init RAM region is the natural backing store: it survives a warm reboot but is lost on real power removal.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Types Documentation




### typedef WshShellSession\_ReadHandler\_t 

_Read the persisted session descriptor._ 
```C++
typedef WshShellSession_t(* WshShellSession_ReadHandler_t) (void);
```




<hr>



### typedef WshShellSession\_WriteHandler\_t 

_Persist the session descriptor._ 
```C++
typedef void(* WshShellSession_WriteHandler_t) (WshShellSession_t);
```




<hr>
## Public Functions Documentation




### function WshShellSession\_Clear 

_Drop any persisted session._ 
```C++
void WshShellSession_Clear (
    WshShellSessionIO_t * pSessIO
) 
```





**Parameters:**


* `pSessIO` Pointer to the I/O structure. 




        

<hr>



### function WshShellSession\_Init 

_Initialise the session system with custom I/O handlers._ 
```C++
void WshShellSession_Init (
    WshShellSessionIO_t * pSessIO,
    WshShellSession_ReadHandler_t readFn,
    WshShellSession_WriteHandler_t writeFn
) 
```



Installs the handlers and normalises the backing store: any invalid descriptor (e.g. random RAM after a cold boot) is cleared so it will not be restored.




**Parameters:**


* `pSessIO` Pointer to the I/O structure. 
* `readFn` Callback to read the saved descriptor. 
* `writeFn` Callback to persist the descriptor. 




        

<hr>



### function WshShellSession\_IsValid 

_Check whether a descriptor is a valid, active keep request._ 
```C++
WshShell_Bool_t WshShellSession_IsValid (
    const WshShellSession_t * pcSession
) 
```





**Parameters:**


* `pcSession` Descriptor to validate. 



**Return value:**


* `true` Hash matches and RebootsLeft &gt; 0. 
* `false` Corrupt/uninitialised, or budget exhausted. 




        

<hr>



### function WshShellSession\_Read 

_Read the persisted descriptor._ 
```C++
WshShellSession_t WshShellSession_Read (
    WshShellSessionIO_t * pSessIO
) 
```





**Parameters:**


* `pSessIO` Pointer to the I/O structure. 



**Returns:**

The stored descriptor (may be invalid — check with [**WshShellSession\_IsValid()**](wsh__shell__session_8h.md#function-wshshellsession_isvalid)). 





        

<hr>



### function WshShellSession\_Store 

_Persist a descriptor, computing its integrity hash first._ 
```C++
void WshShellSession_Store (
    WshShellSessionIO_t * pSessIO,
    WshShellSession_t session
) 
```





**Parameters:**


* `pSessIO` Pointer to the I/O structure. 
* `session` Descriptor to store (its Hash field is filled in). 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_session.h`

