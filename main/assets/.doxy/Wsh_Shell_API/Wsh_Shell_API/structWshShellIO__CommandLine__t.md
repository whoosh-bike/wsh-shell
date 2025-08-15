

# Struct WshShellIO\_CommandLine\_t



[**ClassList**](annotated.md) **>** [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md)



_Structure representing the current user input interaction state._ 

* `#include <wsh_shell_io.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Char\_t | [**Buff**](#variable-buff)  <br> |
|  WshShell\_Size\_t | [**CursorPos**](#variable-cursorpos)  <br> |
|  WshShell\_Size\_t | [**Len**](#variable-len)  <br> |












































## Public Attributes Documentation




### variable Buff 

```C++
WshShell_Char_t WshShellIO_CommandLine_t::Buff[WSH_SHELL_INTR_BUFF_LEN];
```



Input buffer for interactive line. 


        

<hr>



### variable CursorPos 

```C++
WshShell_Size_t WshShellIO_CommandLine_t::CursorPos;
```



Current cursor position in the buffer. 


        

<hr>



### variable Len 

```C++
WshShell_Size_t WshShellIO_CommandLine_t::Len;
```



Current length of the buffer. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_io.h`

