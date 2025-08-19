

# Struct WshShell\_PS1Data\_t



[**ClassList**](annotated.md) **>** [**WshShell\_PS1Data\_t**](structWshShell__PS1Data__t.md)



_Runtime data used for PS1 (prompt string) expansion._ [More...](#detailed-description)

* `#include <wsh_shell_ps1_custom.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**DevName**](#variable-devname)  <br> |
|  WshShell\_Char\_t \* | [**InterCmdName**](#variable-intercmdname)  <br> |
|  const WshShell\_Char\_t \* | [**UserName**](#variable-username)  <br> |












































## Detailed Description


This structure provides the dynamic values that can be substituted into the PS1 template string (`WSH_SHELL_PS1_TEMPLATE`).


Members:
* `UserName` — Current user name string (used by `u`).
* `DevName` — Device name string (used by `d`).
* `InterCmdName` — Optional intermediate command or context-specific string that can be injected into the prompt. Can be NULL if not used. 




    
## Public Attributes Documentation




### variable DevName 

```C++
const WshShell_Char_t* WshShell_PS1Data_t::DevName;
```



Device name string, used in `d`. 


        

<hr>



### variable InterCmdName 

```C++
WshShell_Char_t* WshShell_PS1Data_t::InterCmdName;
```



Intermediate/temporary command name or context string. 


        

<hr>



### variable UserName 

```C++
const WshShell_Char_t* WshShell_PS1Data_t::UserName;
```



User name string, used in `u`. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_ps1_custom.h`

