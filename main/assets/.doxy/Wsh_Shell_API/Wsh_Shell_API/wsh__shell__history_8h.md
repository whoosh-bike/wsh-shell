

# File wsh\_shell\_history.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_history.h**](wsh__shell__history_8h.md)

[Go to the source code of this file](wsh__shell__history_8h_source.md)

_Shell history management API._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_misc.h"`
* `#include "wsh_shell_str.h"`
* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellHistory\_Data\_t**](structWshShellHistory__Data__t.md) <br>_Internal structure for storing command history buffer._  |
| struct | [**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) <br>_Structure holding function pointers for history persistence I/O._  |
| struct | [**WshShellHistory\_t**](structWshShellHistory__t.md) <br>_Shell history structure with integrity check._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**WSH\_SHELL\_HIST\_CMD\_DIR\_t**](#enum-wsh_shell_hist_cmd_dir_t)  <br>_Direction for history navigation._  |
| typedef [**WshShellHistory\_t**](structWshShellHistory__t.md)(\* | [**WshShellHistory\_ReadHandler\_t**](#typedef-wshshellhistory_readhandler_t)  <br>_Function pointer type for reading shell history from persistent storage._  |
| typedef void(\* | [**WshShellHistory\_WriteHandler\_t**](#typedef-wshshellhistory_writehandler_t)  <br>_Function pointer type for writing shell history to persistent storage._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellHistory\_Flush**](#function-wshshellhistory_flush) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO) <br>_Clear the command history buffer._  |
|  WshShell\_Size\_t | [**WshShellHistory\_GetNextCmd**](#function-wshshellhistory_getnextcmd) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, WshShell\_Char\_t \* pOutBuff, WshShell\_Size\_t outBuffSize) <br>_Retrieve the next command from history._  |
|  WshShell\_Size\_t | [**WshShellHistory\_GetPrevCmd**](#function-wshshellhistory_getprevcmd) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, WshShell\_Char\_t \* pOutBuff, WshShell\_Size\_t outBuffSize) <br>_Retrieve the previous command from history._  |
|  WshShell\_Size\_t | [**WshShellHistory\_GetTokenByIndex**](#function-wshshellhistory_gettokenbyindex) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, WshShell\_Char\_t \* pOutBuff, WshShell\_Size\_t outBuffSize, WshShell\_Size\_t index) <br>_Retrieve a command from history by its index._  |
|  WshShell\_Size\_t | [**WshShellHistory\_GetTokenNum**](#function-wshshellhistory_gettokennum) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO) <br>_Get the total number of commands stored in history._  |
|  void | [**WshShellHistory\_Init**](#function-wshshellhistory_init) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellHistory\_ReadHandler\_t**](wsh__shell__history_8h.md#typedef-wshshellhistory_readhandler_t) readFn, [**WshShellHistory\_WriteHandler\_t**](wsh__shell__history_8h.md#typedef-wshshellhistory_writehandler_t) writeFn) <br>_Initialize the shell history system with custom I/O functions._  |
|  void | [**WshShellHistory\_SaveCmd**](#function-wshshellhistory_savecmd) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, const WshShell\_Char\_t \* pcCmdStr, WshShell\_Size\_t cmdStrLen) <br>_Save a new command to the history buffer._  |




























## Detailed Description


This module provides functionality for storing, navigating, and persisting command history in an embedded shell environment.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Types Documentation




### enum WSH\_SHELL\_HIST\_CMD\_DIR\_t 

_Direction for history navigation._ 
```C++
enum WSH_SHELL_HIST_CMD_DIR_t {
    WSH_SHELL_HIST_CMD_PREV = 0,
    WSH_SHELL_HIST_CMD_NEXT
};
```




<hr>



### typedef WshShellHistory\_ReadHandler\_t 

_Function pointer type for reading shell history from persistent storage._ 
```C++
typedef WshShellHistory_t(* WshShellHistory_ReadHandler_t) (void);
```




<hr>



### typedef WshShellHistory\_WriteHandler\_t 

_Function pointer type for writing shell history to persistent storage._ 
```C++
typedef void(* WshShellHistory_WriteHandler_t) (WshShellHistory_t);
```




<hr>
## Public Functions Documentation




### function WshShellHistory\_Flush 

_Clear the command history buffer._ 
```C++
void WshShellHistory_Flush (
    WshShellHistory_IO_t * pHistIO
) 
```





**Parameters:**


* `pHistIO` Pointer to the I/O structure. 




        

<hr>



### function WshShellHistory\_GetNextCmd 

_Retrieve the next command from history._ 
```C++
WshShell_Size_t WshShellHistory_GetNextCmd (
    WshShellHistory_IO_t * pHistIO,
    WshShell_Char_t * pOutBuff,
    WshShell_Size_t outBuffSize
) 
```





**Parameters:**


* `pHistIO` Pointer to the I/O structure. 
* `pOutBuff` Buffer to store the command string. 
* `outBuffSize` Size of the output buffer.



**Returns:**

Length of the retrieved command. 





        

<hr>



### function WshShellHistory\_GetPrevCmd 

_Retrieve the previous command from history._ 
```C++
WshShell_Size_t WshShellHistory_GetPrevCmd (
    WshShellHistory_IO_t * pHistIO,
    WshShell_Char_t * pOutBuff,
    WshShell_Size_t outBuffSize
) 
```





**Parameters:**


* `pHistIO` Pointer to the I/O structure. 
* `pOutBuff` Buffer to store the command string. 
* `outBuffSize` Size of the output buffer.



**Returns:**

Length of the retrieved command. 





        

<hr>



### function WshShellHistory\_GetTokenByIndex 

_Retrieve a command from history by its index._ 
```C++
WshShell_Size_t WshShellHistory_GetTokenByIndex (
    WshShellHistory_IO_t * pHistIO,
    WshShell_Char_t * pOutBuff,
    WshShell_Size_t outBuffSize,
    WshShell_Size_t index
) 
```



Index 0 corresponds to the most recently saved command, index 1 to the previous one, and so on.




**Parameters:**


* `pHistIO` Pointer to the I/O structure. 
* `pOutBuff` Buffer to store the retrieved command. 
* `outBuffSize` Size of the output buffer. 
* `index` Index of the command to retrieve (0 = latest).



**Returns:**

Length of the retrieved command, or 0 if not found or buffer too small. 





        

<hr>



### function WshShellHistory\_GetTokenNum 

_Get the total number of commands stored in history._ 
```C++
WshShell_Size_t WshShellHistory_GetTokenNum (
    WshShellHistory_IO_t * pHistIO
) 
```



Starts from the most recently saved command and iterates backwards through the history buffer to count all stored commands.




**Parameters:**


* `pHistIO` Pointer to the I/O structure.



**Returns:**

Number of stored commands in history. 





        

<hr>



### function WshShellHistory\_Init 

_Initialize the shell history system with custom I/O functions._ 
```C++
void WshShellHistory_Init (
    WshShellHistory_IO_t * pHistIO,
    WshShellHistory_ReadHandler_t readFn,
    WshShellHistory_WriteHandler_t writeFn
) 
```



This function sets up the internal history system and loads existing history using the provided read/write callbacks.




**Parameters:**


* `pHistIO` Pointer to the I/O structure used for read/write access. 
* `readFn` Callback to read saved history data. 
* `writeFn` Callback to persist current history data. 




        

<hr>



### function WshShellHistory\_SaveCmd 

_Save a new command to the history buffer._ 
```C++
void WshShellHistory_SaveCmd (
    WshShellHistory_IO_t * pHistIO,
    const WshShell_Char_t * pcCmdStr,
    WshShell_Size_t cmdStrLen
) 
```





**Parameters:**


* `pHistIO` Pointer to the I/O structure. 
* `pcCmdStr` Pointer to the command string (without EOL). 
* `cmdStrLen` Length of the command string. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_history.h`

