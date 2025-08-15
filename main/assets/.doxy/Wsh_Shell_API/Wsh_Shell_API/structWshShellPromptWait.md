

# Struct WshShellPromptWait



[**ClassList**](annotated.md) **>** [**WshShellPromptWait**](structWshShellPromptWait.md)



_Prompt-wait control structure._ [More...](#detailed-description)

* `#include <wsh_shell_promptwait.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**Ctx**](#variable-ctx)  <br> |
|  [**WshShellPromptWait\_Handler\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_handler_t) | [**Handler**](#variable-handler)  <br> |












































## Detailed Description


Holds the active handler and optional context pointer. If Handler is NULL, no prompt-wait is active. 


    
## Public Attributes Documentation




### variable Ctx 

```C++
void* WshShellPromptWait::Ctx;
```




<hr>



### variable Handler 

```C++
WshShellPromptWait_Handler_t WshShellPromptWait::Handler;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_promptwait.h`

