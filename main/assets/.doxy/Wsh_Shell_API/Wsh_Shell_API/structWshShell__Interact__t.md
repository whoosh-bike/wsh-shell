

# Struct WshShell\_Interact\_t



[**ClassList**](annotated.md) **>** [**WshShell\_Interact\_t**](structWshShell__Interact__t.md)



_Interactive command registration structure._ [More...](#detailed-description)

* `#include <wsh_shell_interact.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Char\_t | [**CmdName**](#variable-cmdname)  <br> |
|  [**WshShellInteractive\_CmdHandler\_t**](wsh__shell__interact_8h.md#typedef-wshshellinteractive_cmdhandler_t) | [**Handler**](#variable-handler)  <br> |












































## Detailed Description


Stores the name of the interactive command and the corresponding execution function. 


    
## Public Attributes Documentation




### variable CmdName 

```C++
WshShell_Char_t WshShell_Interact_t::CmdName[WSH_SHELL_CMD_NAME_LEN];
```



Name of the interactive command. 


        

<hr>



### variable Handler 

```C++
WshShellInteractive_CmdHandler_t WshShell_Interact_t::Handler;
```



Function to be executed in interactive mode. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_interact.h`

