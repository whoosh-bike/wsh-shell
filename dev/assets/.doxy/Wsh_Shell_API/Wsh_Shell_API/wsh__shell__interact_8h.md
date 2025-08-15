

# File wsh\_shell\_interact.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_interact.h**](wsh__shell__interact_8h.md)

[Go to the source code of this file](wsh__shell__interact_8h_source.md)

_Interactive command registration and line editing utilities for WshShell._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_io.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShell\_Interact\_t**](structWshShell__Interact__t.md) <br>_Interactive command registration structure._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef void(\* | [**WshShellInteractive\_CmdHandler\_t**](#typedef-wshshellinteractive_cmdhandler_t)  <br>_Function pointer type for an interactive shell command._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellInteract\_AppendLineBreak**](#function-wshshellinteract_appendlinebreak) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Appends a CRLF (_ `\r\n` _) sequence to the interaction buffer._ |
|  void | [**WshShellInteract\_Attach**](#function-wshshellinteract_attach) ([**WshShell\_Interact\_t**](structWshShell__Interact__t.md) \* pInteract, const WshShell\_Char\_t \* pcName, [**WshShellInteractive\_CmdHandler\_t**](wsh__shell__interact_8h.md#typedef-wshshellinteractive_cmdhandler_t) handler) <br>_Registers an interactive command._  |
|  void | [**WshShellInteract\_Flush**](#function-wshshellinteract_flush) ([**WshShell\_Interact\_t**](structWshShell__Interact__t.md) \* pInteract) <br>_Clears the contents of an interactive command slot._  |




























## Detailed Description


This module provides an interface for attaching interactive commands and for modifying the user input buffer, such as appending carriage return and line feed sequences (`\r\n`) at the end of the command line.


It is used internally by the shell to register interactive commands and manage the formatting of user-entered command lines.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### typedef WshShellInteractive\_CmdHandler\_t 

_Function pointer type for an interactive shell command._ 
```C++
typedef void(* WshShellInteractive_CmdHandler_t) (WshShellIO_CommandLine_t *pCommandLine);
```



This type defines the prototype for interactive command callbacks, which are invoked with a pointer to the current command line buffer. 


        

<hr>
## Public Functions Documentation




### function WshShellInteract\_AppendLineBreak 

_Appends a CRLF (_ `\r\n` _) sequence to the interaction buffer._
```C++
void WshShellInteract_AppendLineBreak (
    WshShellIO_CommandLine_t * pCommandLine
) 
```



Adds a carriage return and line feed to the end of the current input buffer, if there is enough space. This is typically used to terminate user input lines.


If the buffer is too full to append both characters, an error is printed.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer (command line). 




        

<hr>



### function WshShellInteract\_Attach 

_Registers an interactive command._ 
```C++
void WshShellInteract_Attach (
    WshShell_Interact_t * pInteract,
    const WshShell_Char_t * pcName,
    WshShellInteractive_CmdHandler_t handler
) 
```



Attaches a named command and its execution function to the given interact structure. The command name is truncated if it exceeds the internal buffer size.




**Parameters:**


* `pInteract` Pointer to the interact object to initialize. 
* `pcName` Null-terminated name of the interactive command. 
* `handler` Function pointer to be called when the command is executed. 




        

<hr>



### function WshShellInteract\_Flush 

_Clears the contents of an interactive command slot._ 
```C++
void WshShellInteract_Flush (
    WshShell_Interact_t * pInteract
) 
```



Resets the execution function pointer to `NULL` and clears the command name buffer. This effectively "removes" the interactive command from the shell registry.




**Parameters:**


* `pInteract` Pointer to the interact structure to flush. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_interact.h`

