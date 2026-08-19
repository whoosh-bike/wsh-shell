

# Struct WshShellSessionIO\_t



[**ClassList**](annotated.md) **>** [**WshShellSessionIO\_t**](structWshShellSessionIO__t.md)



_Function pointers for session persistence I/O._ [More...](#detailed-description)

* `#include <wsh_shell_session.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellSession\_ReadHandler\_t**](wsh__shell__session_8h.md#typedef-wshshellsession_readhandler_t) | [**Read**](#variable-read)  <br> |
|  [**WshShellSession\_WriteHandler\_t**](wsh__shell__session_8h.md#typedef-wshshellsession_writehandler_t) | [**Write**](#variable-write)  <br> |












































## Detailed Description


Keeps the session system storage-agnostic by delegating read/write to the integrator (typically no-init RAM). 


    
## Public Attributes Documentation




### variable Read 

```C++
WshShellSession_ReadHandler_t WshShellSessionIO_t::Read;
```



Callback for loading the saved session. 


        

<hr>



### variable Write 

```C++
WshShellSession_WriteHandler_t WshShellSessionIO_t::Write;
```



Callback for saving the session. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_session.h`

