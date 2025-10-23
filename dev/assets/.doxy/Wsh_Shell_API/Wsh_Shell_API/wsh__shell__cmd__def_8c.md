

# File wsh\_shell\_cmd\_def.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd\_def.c**](wsh__shell__cmd__def_8c.md)

[Go to the source code of this file](wsh__shell__cmd__def_8c_source.md)



* `#include "wsh_shell_cmd_def.h"`
* `#include "wsh_shell.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) | [**WshShellDefCmd**](#variable-wshshelldefcmd)   = `/* multi line expression */`<br> |
|  const WSH\_SHELL\_CMD\_GROUP\_t | [**WshShell\_CmdGroups**](#variable-wshshell_cmdgroups)   = `{WSH\_SHELL\_CMD\_GROUP\_LIST}`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellCmdDef\_Dummy**](#function-wshshellcmddef_dummy) (const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmd, WshShell\_Size\_t argc, const WshShell\_Char\_t \* pArgv, void \* pShellCtx) <br> |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellDefCmd\_GetPtr**](#function-wshshelldefcmd_getptr) (void) <br>_Get a pointer to the default shell command structure._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_COUNT**](wsh__shell__cmd__def_8c.md#define-wsh_shell_cmd_group_count)  `(WSH\_SHELL\_ARR\_LEN(WshShell\_CmdGroups))`<br> |
| define  | [**X\_ENTRY**](wsh__shell__cmd__def_8c.md#define-x_entry) (name, value) `value,`<br> |

## Public Static Attributes Documentation




### variable WshShellDefCmd 

```C++
const WshShellCmd_t WshShellDefCmd;
```




<hr>



### variable WshShell\_CmdGroups 

```C++
const WSH_SHELL_CMD_GROUP_t WshShell_CmdGroups[];
```




<hr>
## Public Functions Documentation




### function WshShellCmdDef\_Dummy 

```C++
WSH_SHELL_RET_STATE_t WshShellCmdDef_Dummy (
    const WshShellCmd_t * pcCmd,
    WshShell_Size_t argc,
    const WshShell_Char_t * pArgv,
    void * pShellCtx
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
## Macro Definition Documentation





### define WSH\_SHELL\_CMD\_GROUP\_COUNT 

```C++
#define WSH_SHELL_CMD_GROUP_COUNT `(WSH_SHELL_ARR_LEN(WshShell_CmdGroups))`
```




<hr>



### define X\_ENTRY 

```C++
#define X_ENTRY (
    name,
    value
) `value,`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd_def.c`

