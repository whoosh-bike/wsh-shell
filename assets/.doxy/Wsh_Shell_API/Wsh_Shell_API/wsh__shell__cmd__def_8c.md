

# File wsh\_shell\_cmd\_def.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd\_def.c**](wsh__shell__cmd__def_8c.md)

[Go to the source code of this file](wsh__shell__cmd__def_8c_source.md)



* `#include "wsh_shell_cmd_def.h"`
* `#include "wsh_shell.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**WSH\_SHELL\_DEF\_OPT\_t**](#enum-wsh_shell_def_opt_t)  <br>_Enumeration of supported options for the default shell command._  |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellOption\_t**](structWshShellOption__t.md) | [**OptArr**](#variable-optarr)   = `{WSH\_SHELL\_CMD\_DEF\_OPT\_TABLE()}`<br> |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) | [**WshShellDefCmd**](#variable-wshshelldefcmd)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellCmdDef**](#function-wshshellcmddef) (const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* pcCmd, WshShell\_Size\_t argc, const WshShell\_Char\_t \* pArgv, void \* pCtx) <br> |
|  const [**WshShellCmd\_t**](wsh__shell__cmd_8h.md#typedef-wshshellcmd_t) \* | [**WshShellDefCmd\_GetPtr**](#function-wshshelldefcmd_getptr) (void) <br>_Get a pointer to the default shell command structure._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**shell\_cmd\_def\_\_interactive**](#function-shell_cmd_def__interactive) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pInter) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_CMD\_DEF\_OPT\_TABLE**](wsh__shell__cmd__def_8c.md#define-wsh_shell_cmd_def_opt_table) () `/* multi line expression */`<br>_Macro defining the default shell command options using X-Macros._  |
| define  | [**X\_CMD\_ENTRY**](wsh__shell__cmd__def_8c.md#define-x_cmd_entry) (en, m) `en,`<br> |
| define  | [**X\_CMD\_ENTRY**](wsh__shell__cmd__def_8c.md#define-x_cmd_entry) (enum, opt) `{enum, opt},`<br> |

## Public Types Documentation




### enum WSH\_SHELL\_DEF\_OPT\_t 

_Enumeration of supported options for the default shell command._ 
```C++
enum WSH_SHELL_DEF_OPT_t {
    WSH_SHELL_CMD_DEF_OPT_TABLE =() WSH_SHELL_CMD_DEF_OPT_ENUM_SIZE
};
```



Each value corresponds to an entry from the WSH\_SHELL\_CMD\_DEF\_OPT\_TABLE. 


        

<hr>
## Public Static Attributes Documentation




### variable OptArr 

```C++
WshShellOption_t OptArr[];
```




<hr>



### variable WshShellDefCmd 

```C++
const WshShellCmd_t WshShellDefCmd;
```




<hr>
## Public Functions Documentation




### function WshShellCmdDef 

```C++
WSH_SHELL_RET_STATE_t WshShellCmdDef (
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
## Public Static Functions Documentation




### function shell\_cmd\_def\_\_interactive 

```C++
static void shell_cmd_def__interactive (
    WshShellIO_CommandLine_t * pInter
) 
```




<hr>
## Macro Definition Documentation





### define WSH\_SHELL\_CMD\_DEF\_OPT\_TABLE 

_Macro defining the default shell command options using X-Macros._ 
```C++
#define WSH_SHELL_CMD_DEF_OPT_TABLE (
    
) `/* multi line expression */`
```



This macro expands into a list of `X_CMD_ENTRY()` calls, each representing an available option for the default shell command. Used for both option structure definitions and enumeration. 


        

<hr>



### define X\_CMD\_ENTRY 

```C++
#define X_CMD_ENTRY (
    en,
    m
) `en,`
```




<hr>



### define X\_CMD\_ENTRY 

```C++
#define X_CMD_ENTRY (
    enum,
    opt
) `{enum, opt},`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cmd_def.c`

