

# File wsh\_shell\_history.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_history.c**](wsh__shell__history_8c.md)

[Go to the source code of this file](wsh__shell__history_8c_source.md)



* `#include "wsh_shell_history.h"`





































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
The documentation for this class was generated from the following file `src/wsh_shell_history.c`

