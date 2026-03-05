

# Struct WshShellHistoryIO\_t



[**ClassList**](annotated.md) **>** [**WshShellHistoryIO\_t**](structWshShellHistoryIO__t.md)



_Structure holding function pointers for history persistence I/O._ [More...](#detailed-description)

* `#include <wsh_shell_history.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellHistory\_ReadHandler\_t**](wsh__shell__history_8h.md#typedef-wshshellhistory_readhandler_t) | [**Read**](#variable-read)  <br> |
|  [**WshShellHistory\_WriteHandler\_t**](wsh__shell__history_8h.md#typedef-wshshellhistory_writehandler_t) | [**Write**](#variable-write)  <br> |












































## Detailed Description


This allows the shell history system to remain storage-agnostic by delegating read/write responsibilities to external code (e.g., flash drivers, NVM emulation). 


    
## Public Attributes Documentation




### variable Read 

```C++
WshShellHistory_ReadHandler_t WshShellHistoryIO_t::Read;
```



Callback for loading saved history. 


        

<hr>



### variable Write 

```C++
WshShellHistory_WriteHandler_t WshShellHistoryIO_t::Write;
```



Callback for saving current history. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_history.h`

