

# Struct WshShellCmd



[**ClassList**](annotated.md) **>** [**WshShellCmd**](structWshShellCmd.md)



_Descriptor for a shell command._ 

* `#include <wsh_shell_cmd.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**Descr**](#variable-descr)  <br> |
|  WshShell\_Size\_t | [**Groups**](#variable-groups)  <br> |
|  [**WshShellCmdHandler\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmdhandler_t) | [**Handler**](#variable-handler)  <br> |
|  const WshShell\_Char\_t \* | [**Name**](#variable-name)  <br> |
|  WshShell\_Size\_t | [**OptNum**](#variable-optnum)  <br> |
|  const [**WshShellOption\_t**](structWshShellOption__t.md) \* | [**Options**](#variable-options)  <br> |












































## Public Attributes Documentation




### variable Descr 

```C++
const WshShell_Char_t* WshShellCmd::Descr;
```



Human-readable description of the command. 


        

<hr>



### variable Groups 

```C++
WshShell_Size_t WshShellCmd::Groups;
```



Command group bitmask for access control or categorization. 


        

<hr>



### variable Handler 

```C++
WshShellCmdHandler_t WshShellCmd::Handler;
```



Execution callback function. 


        

<hr>



### variable Name 

```C++
const WshShell_Char_t* WshShellCmd::Name;
```



Command name (e.g., "set", "info"). 


        

<hr>



### variable OptNum 

```C++
WshShell_Size_t WshShellCmd::OptNum;
```



Number of defined options. 


        

<hr>



### variable Options 

```C++
const WshShellOption_t* WshShellCmd::Options;
```



Pointer to the command's options array. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd.h`

