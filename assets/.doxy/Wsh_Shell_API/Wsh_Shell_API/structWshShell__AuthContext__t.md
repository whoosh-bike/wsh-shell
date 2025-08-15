

# Struct WshShell\_AuthContext\_t



[**ClassList**](annotated.md) **>** [**WshShell\_AuthContext\_t**](structWshShell__AuthContext__t.md)



_Temporary authentication context for login input handling._ 

* `#include <wsh_shell.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Char\_t | [**Login**](#variable-login)  <br> |
|  WshShell\_Char\_t | [**Pass**](#variable-pass)  <br> |












































## Public Attributes Documentation




### variable Login 

```C++
WshShell_Char_t WshShell_AuthContext_t::Login[WSH_SHELL_LOGIN_LEN];
```



Input buffer for username. 


        

<hr>



### variable Pass 

```C++
WshShell_Char_t WshShell_AuthContext_t::Pass[WSH_SHELL_PASS_LEN];
```



Input buffer for password. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.h`

