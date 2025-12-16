

# Struct WshShellExtCallbacks\_t



[**ClassList**](annotated.md) **>** [**WshShellExtCallbacks\_t**](structWshShellExtCallbacks__t.md)



_Optional external callbacks invoked on user session changes._ 

* `#include <wsh_shell.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShell\_ExtClbk\_t**](wsh__shell_8h.md#typedef-wshshell_extclbk_t) | [**Auth**](#variable-auth)  <br> |
|  [**WshShell\_ExtClbk\_t**](wsh__shell_8h.md#typedef-wshshell_extclbk_t) | [**DeAuth**](#variable-deauth)  <br> |
|  [**WshShell\_ExtClbk\_t**](wsh__shell_8h.md#typedef-wshshell_extclbk_t) | [**SymbolIn**](#variable-symbolin)  <br> |












































## Public Attributes Documentation




### variable Auth 

```C++
WshShell_ExtClbk_t WshShellExtCallbacks_t::Auth;
```



Called when a user successfully authenticates. 


        

<hr>



### variable DeAuth 

```C++
WshShell_ExtClbk_t WshShellExtCallbacks_t::DeAuth;
```



Called when a user is de-authenticated or logs out. 


        

<hr>



### variable SymbolIn 

```C++
WshShell_ExtClbk_t WshShellExtCallbacks_t::SymbolIn;
```



Called when a sybol was received and inserted 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.h`

