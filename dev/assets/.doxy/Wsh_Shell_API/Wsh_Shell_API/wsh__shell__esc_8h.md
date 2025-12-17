

# File wsh\_shell\_esc.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_esc.h**](wsh__shell__esc_8h.md)

[Go to the source code of this file](wsh__shell__esc_8h_source.md)

_Escape sequence handling logic for WSH shell._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_history.h"`
* `#include "wsh_shell_io.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellEsc\_Action\_t**](structWshShellEsc__Action__t.md) <br>_Escape sequence to handler mapping._  |
| struct | [**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) <br>_Escape sequence input state tracker._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef void(\* | [**WshShellEsc\_Hadler\_t**](#typedef-wshshellesc_hadler_t)  <br>_Escape sequence handler function type._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellEsc\_Handler**](#function-wshshellesc_handler) ([**WshShellHistoryIO\_t**](structWshShellHistoryIO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, [**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage, const WshShell\_Char\_t symbol) <br>_Handles incoming escape sequence character._  |
|  WshShell\_Bool\_t | [**WshShellEsc\_IsSeqStarted**](#function-wshshellesc_isseqstarted) ([**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage) <br>_Checks if escape sequence is in progress._  |
|  void | [**WshShellEsc\_StartSeq**](#function-wshshellesc_startseq) ([**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage) <br>_Starts a new escape sequence._  |




























## Detailed Description


Provides functionality to process ANSI escape sequences (e.g., arrow keys) and interact with shell history and input buffer.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Types Documentation




### typedef WshShellEsc\_Hadler\_t 

_Escape sequence handler function type._ 
```C++
typedef void(* WshShellEsc_Hadler_t) (WshShellHistoryIO_t *pHistIO, WshShellIO_CommandLine_t *pCommandLine);
```




<hr>
## Public Functions Documentation




### function WshShellEsc\_Handler 

_Handles incoming escape sequence character._ 
```C++
void WshShellEsc_Handler (
    WshShellHistoryIO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine,
    WshShellEsc_Storage_t * pEscStorage,
    const WshShell_Char_t symbol
) 
```



Matches current buffer against known ANSI sequences and triggers appropriate handler.




**Parameters:**


* `pHistIO` Pointer to shell history interface 
* `pCommandLine` Pointer to input interaction buffer 
* `pEscStorage` Pointer to escape sequence buffer 
* `symbol` Incoming character to append 




        

<hr>



### function WshShellEsc\_IsSeqStarted 

_Checks if escape sequence is in progress._ 
```C++
WshShell_Bool_t WshShellEsc_IsSeqStarted (
    WshShellEsc_Storage_t * pEscStorage
) 
```





**Parameters:**


* `pEscStorage` Pointer to escape sequence buffer 



**Returns:**

WshShell\_Bool\_t `true` if sequence has started 





        

<hr>



### function WshShellEsc\_StartSeq 

_Starts a new escape sequence._ 
```C++
void WshShellEsc_StartSeq (
    WshShellEsc_Storage_t * pEscStorage
) 
```



Initializes the escape buffer with ESC char.




**Parameters:**


* `pEscStorage` Pointer to escape sequence buffer 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_esc.h`

