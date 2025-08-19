

# File wsh\_shell\_cmd\_def.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd\_def.c**](wsh__shell__cmd__def_8c.md)

[Go to the source code of this file](wsh__shell__cmd__def_8c_source.md)



* `#include "wsh_shell_cmd_def.h"`
* `#include "wsh_shell.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) | [**WshShellDefCmd**](#variable-wshshelldefcmd)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellCmdDef\_Dummy**](#function-wshshellcmddef_dummy) (const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmd, WshShell\_Size\_t argc, const WshShell\_Char\_t \* pArgv, void \* pCtx) <br> |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellDefCmd\_GetPtr**](#function-wshshelldefcmd_getptr) (void) <br>_Get a pointer to the default shell command structure._  |




























## Public Static Attributes Documentation




### variable WshShellDefCmd 

```C++
const WshShellCmd_t WshShellDefCmd;
```




<hr>
## Public Functions Documentation




### function WshShellCmdDef\_Dummy 

```C++
WSH_SHELL_RET_STATE_t WshShellCmdDef_Dummy (
    const WshShellCmd_t * pcCmd,
    WshShell_Size_t argc,
    const WshShell_Char_t * pArgv,
    void * pCtx
) 
```




<hr>



### function WshShellDefCmd\_GetPtr 

_Get a pointer to the default shell command structure._ 
```C++
const WshShellCmd_t * WshShellDefCmd_GetPtr (
    void
) 
```



This function returns a constant pointer to the static command structure WshShellDefCmd, which describes the default command used for configuring and querying the shell interface.




**Returns:**

const WshShellCmd\_t\* Pointer to the default command structure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd_def.c`

