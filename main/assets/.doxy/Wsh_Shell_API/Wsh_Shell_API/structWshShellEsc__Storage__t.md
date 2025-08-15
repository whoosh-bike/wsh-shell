

# Struct WshShellEsc\_Storage\_t



[**ClassList**](annotated.md) **>** [**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md)



_Escape sequence input state tracker._ 

* `#include <wsh_shell_esc.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Char\_t | [**Buff**](#variable-buff)  <br> |
|  WshShell\_Size\_t | [**Cnt**](#variable-cnt)  <br> |












































## Public Attributes Documentation




### variable Buff 

```C++
WshShell_Char_t WshShellEsc_Storage_t::Buff[WSH_SHELL_ESC_BUFF_LEN];
```



Escape sequence buffer 


        

<hr>



### variable Cnt 

```C++
WshShell_Size_t WshShellEsc_Storage_t::Cnt;
```



Current number of buffered characters 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_esc.h`

