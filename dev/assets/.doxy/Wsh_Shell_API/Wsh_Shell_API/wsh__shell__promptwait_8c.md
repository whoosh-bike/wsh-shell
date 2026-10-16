

# File wsh\_shell\_promptwait.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_promptwait.c**](wsh__shell__promptwait_8c.md)

[Go to the source code of this file](wsh__shell__promptwait_8c_source.md)



* `#include "wsh_shell_promptwait.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellPromptWait\_Attach**](#function-wshshellpromptwait_attach) ([**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait, [**WshShellPromptWait\_Handler\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_handler_t) handler, void \* pCtx) <br>_Attach a one-time input handler._  |
|  void | [**WshShellPromptWait\_AttachTagged**](#function-wshshellpromptwait_attachtagged) ([**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait, [**WshShellPromptWait\_Handler\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_handler_t) handler, void \* pCtx, const WshShell\_Char\_t \* pcTag) <br>_Attach a wait and say what it is for. A host reads_ `pcTag` _from the shell state line ("busy":"&lt;tag&gt;") to tell e.g. a running sniffer from a yes/no question. The string must outlive the wait._ |
|  WshShell\_Bool\_t | [**WshShellPromptWait\_Enter**](#function-wshshellpromptwait_enter) (WshShell\_Char\_t symbol, [**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait) <br>_User input handler for prompt-wait mode._  |
|  void | [**WshShellPromptWait\_Flush**](#function-wshshellpromptwait_flush) ([**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait) <br>_Flush the current prompt-wait mode._  |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellPromptWait\_Handle**](#function-wshshellpromptwait_handle) ([**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait, WshShell\_Char\_t symbol) <br>_Handle a symbol when in prompt-wait mode._  |
|  WshShell\_Bool\_t | [**WshShellPromptWait\_HintIsNeeded**](#function-wshshellpromptwait_hintisneeded) (const [**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pcWait) <br>_Whether a handler should still print its hint on a rejected keystroke._  |
|  void | [**WshShellPromptWait\_SetNotify**](#function-wshshellpromptwait_setnotify) ([**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait, [**WshShellPromptWait\_Notify\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_notify_t) notify, void \* pOwner) <br>_Set by the shell itself; applications don't call it._  |
|  WshShell\_Bool\_t | [**WshShellPromptWait\_YesNo**](#function-wshshellpromptwait_yesno) (WshShell\_Char\_t symbol, [**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pWait) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellPromptWait\_Notify**](#function-wshshellpromptwait_notify) (const [**WshShellPromptWait\_t**](structWshShellPromptWait.md) \* pcWait) <br> |


























## Public Functions Documentation




### function WshShellPromptWait\_Attach 

_Attach a one-time input handler._ 
```C++
void WshShellPromptWait_Attach (
    WshShellPromptWait_t * pWait,
    WshShellPromptWait_Handler_t handler,
    void * pCtx
) 
```



Replaces normal shell input handling with a temporary callback. The callback will receive the first character entered by the user, after which it should typically disable itself by calling `WshShellPromptWait_Flush()`.




**Parameters:**


* `pWait` Pointer to prompt-wait control object. 
* `handler` Callback function to invoke on user input. 
* `pCtx` Optional pointer to user context. 




        

<hr>



### function WshShellPromptWait\_AttachTagged 

_Attach a wait and say what it is for. A host reads_ `pcTag` _from the shell state line ("busy":"&lt;tag&gt;") to tell e.g. a running sniffer from a yes/no question. The string must outlive the wait._
```C++
void WshShellPromptWait_AttachTagged (
    WshShellPromptWait_t * pWait,
    WshShellPromptWait_Handler_t handler,
    void * pCtx,
    const WshShell_Char_t * pcTag
) 
```




<hr>



### function WshShellPromptWait\_Enter 

_User input handler for prompt-wait mode._ 
```C++
WshShell_Bool_t WshShellPromptWait_Enter (
    WshShell_Char_t symbol,
    WshShellPromptWait_t * pWait
) 
```



Processes a character entered by the user while waiting for specific input (e.g., Enter key, Y/N confirmation, etc.).




**Parameters:**


* `symbol` The character entered by the user. 
* `pWait` Pointer to the prompt-wait control structure. 



**Returns:**

WshShell\_Bool\_t
* true if the input was handled and waiting can end;
* false if waiting should continue. 







        

<hr>



### function WshShellPromptWait\_Flush 

_Flush the current prompt-wait mode._ 
```C++
void WshShellPromptWait_Flush (
    WshShellPromptWait_t * pWait
) 
```



Resets the handler to NULL so that normal shell input is processed.




**Parameters:**


* `pWait` Pointer to prompt-wait control object. 




        

<hr>



### function WshShellPromptWait\_Handle 

_Handle a symbol when in prompt-wait mode._ 
```C++
WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle (
    WshShellPromptWait_t * pWait,
    WshShell_Char_t symbol
) 
```



If a handler is set, it is called with the given symbol and context.




**Parameters:**


* `pWait` Pointer to prompt-wait control object. 
* `symbol` Character entered by the user. 



**Returns:**

WSH\_SHELL\_RET\_STATE\_ERR\_PARAM if some problems with input params WSH\_SHELL\_RET\_STATE\_ERR\_EMPTY if there are no handler attached WSH\_SHELL\_RET\_STATE\_ERR\_BUSY if normal shell processing should continue WSH\_SHELL\_RET\_STATE\_SUCCESS if the symbol was handled by the prompt-wait mechanism 





        

<hr>



### function WshShellPromptWait\_HintIsNeeded 

_Whether a handler should still print its hint on a rejected keystroke._ 
```C++
WshShell_Bool_t WshShellPromptWait_HintIsNeeded (
    const WshShellPromptWait_t * pcWait
) 
```



Call it from a prompt-wait handler before printing "press ..." so the hint stops after [**WSH\_SHELL\_PROMPT\_WAIT\_HINT\_RETRIES**](wsh__shell__promptwait_8h.md#define-wsh_shell_prompt_wait_hint_retries) rejections instead of repeating for every key.




**Parameters:**


* `pcWait` Pointer to prompt-wait control object. 



**Returns:**

true while the hint is still worth printing. 





        

<hr>



### function WshShellPromptWait\_SetNotify 

_Set by the shell itself; applications don't call it._ 
```C++
void WshShellPromptWait_SetNotify (
    WshShellPromptWait_t * pWait,
    WshShellPromptWait_Notify_t notify,
    void * pOwner
) 
```




<hr>



### function WshShellPromptWait\_YesNo 

```C++
WshShell_Bool_t WshShellPromptWait_YesNo (
    WshShell_Char_t symbol,
    WshShellPromptWait_t * pWait
) 
```




<hr>
## Public Static Functions Documentation




### function WshShellPromptWait\_Notify 

```C++
static void WshShellPromptWait_Notify (
    const WshShellPromptWait_t * pcWait
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_promptwait.c`

