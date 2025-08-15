

# Struct WshShellUser\_t



[**ClassList**](annotated.md) **>** [**WshShellUser\_t**](structWshShellUser__t.md)



_User object definition._ [More...](#detailed-description)

* `#include <wsh_shell_user.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Size\_t | [**Groups**](#variable-groups)  <br> |
|  const WshShell\_Char\_t \* | [**Login**](#variable-login)  <br> |
|  const WshShell\_Char\_t \* | [**Pass**](#variable-pass)  <br> |
|  WshShell\_Size\_t | [**Rights**](#variable-rights)  <br> |












































## Detailed Description


This structure represents a shell user with login, password, and associated access groups and rights. 


    
## Public Attributes Documentation




### variable Groups 

```C++
WshShell_Size_t WshShellUser_t::Groups;
```



Bitmask of accessible command groups. 


        

<hr>



### variable Login 

```C++
const WshShell_Char_t* WshShellUser_t::Login;
```



Pointer to login string. 


        

<hr>



### variable Pass 

```C++
const WshShell_Char_t* WshShellUser_t::Pass;
```



Pointer to password string. 


        

<hr>



### variable Rights 

```C++
WshShell_Size_t WshShellUser_t::Rights;
```



Bitmask of execution rights. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_user.h`

