

# File wsh\_shell\_option.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_option.h**](wsh__shell__option_8h.md)

[Go to the source code of this file](wsh__shell__option_8h_source.md)

_Definition of shell command-line option object and creation macros._ [More...](#detailed-description)

* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellOption\_Context\_t**](structWshShellOption__Context__t.md) <br>_Option usage context during parsing._  |
| struct | [**WshShellOption\_t**](structWshShellOption__t.md) <br>_Represents a shell command-line option._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**WSH\_SHELL\_OPTION\_TYPE\_t**](#enum-wsh_shell_option_type_t)  <br>_Enumeration of all option types used by the shell._  |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**WshShell\_OptionTypeNames**](#variable-wshshell_optiontypenames)   = `/* multi line expression */`<br>_Human-readable names of option types._  |
















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**WshShell\_OptTypeStr\_Get**](#function-wshshell_opttypestr_get) ([**WSH\_SHELL\_OPTION\_TYPE\_t**](wsh__shell__option_8h.md#enum-wsh_shell_option_type_t) optType) <br>_Return the string name of an option type._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_OPTION\_TYPES\_TABLE**](wsh__shell__option_8h.md#define-wsh_shell_option_types_table) () `/* multi line expression */`<br>_Internal macro: full list of option types._  |
| define  | [**WSH\_SHELL\_OPT\_ACCESS\_ANY**](wsh__shell__option_8h.md#define-wsh_shell_opt_access_any)  `((WshShell\_Size\_t)(~0U))`<br> |
| define  | [**WSH\_SHELL\_OPT\_ACCESS\_EXECUTE**](wsh__shell__option_8h.md#define-wsh_shell_opt_access_execute)  `0x04`<br> |
| define  | [**WSH\_SHELL\_OPT\_ACCESS\_NONE**](wsh__shell__option_8h.md#define-wsh_shell_opt_access_none)  `0x00`<br> |
| define  | [**WSH\_SHELL\_OPT\_ACCESS\_READ**](wsh__shell__option_8h.md#define-wsh_shell_opt_access_read)  `0x01`<br> |
| define  | [**WSH\_SHELL\_OPT\_ACCESS\_WRITE**](wsh__shell__option_8h.md#define-wsh_shell_opt_access_write)  `0x02`<br> |
| define  | [**WSH\_SHELL\_OPT\_DESCR**](wsh__shell__option_8h.md#define-wsh_shell_opt_descr) (descr) `""`<br> |
| define  | [**WSH\_SHELL\_OPT\_END**](wsh__shell__option_8h.md#define-wsh_shell_opt_end) () `WSH\_SHELL\_OPTION\_END, WSH\_SHELL\_OPT\_ACCESS\_ANY, 0, NULL, NULL, NULL`<br>_Marks the end of an option array._  |
| define  | [**WSH\_SHELL\_OPT\_FLOAT**](wsh__shell__option_8h.md#define-wsh_shell_opt_float) (acc, short, long, descr) `WSH\_SHELL\_OPTION\_FLOAT, (acc), 1, (short), (long), WSH\_SHELL\_OPT\_DESCR(descr)`<br>_Define a float argument option._  |
| define  | [**WSH\_SHELL\_OPT\_HELP**](wsh__shell__option_8h.md#define-wsh_shell_opt_help) () `/* multi line expression */`<br>_Define a built-in help option (e.g._ `"--help"` _or_`"-h"` _)._ |
| define  | [**WSH\_SHELL\_OPT\_INT**](wsh__shell__option_8h.md#define-wsh_shell_opt_int) (acc, short, long, descr) `WSH\_SHELL\_OPTION\_INT, (acc), 1, (short), (long), WSH\_SHELL\_OPT\_DESCR(descr)`<br>_Define an integer argument option._  |
| define  | [**WSH\_SHELL\_OPT\_INTERACT**](wsh__shell__option_8h.md#define-wsh_shell_opt_interact) () `/* multi line expression */`<br>_Define an option for entering interactive mode._  |
| define  | [**WSH\_SHELL\_OPT\_MULTI\_ARG**](wsh__shell__option_8h.md#define-wsh_shell_opt_multi_arg) (acc, argnum, short, long, descr) `WSH\_SHELL\_OPTION\_MULTI\_ARG, (acc), (argnum), (short), (long), WSH\_SHELL\_OPT\_DESCR(descr)`<br>_Define an option that accepts multiple arguments._  |
| define  | [**WSH\_SHELL\_OPT\_NO**](wsh__shell__option_8h.md#define-wsh_shell_opt_no) (acc) `WSH\_SHELL\_OPTION\_NO, (acc), 0, NULL, NULL, NULL`<br>_Define a special option that matches the command name only (no flags)._  |
| define  | [**WSH\_SHELL\_OPT\_STR**](wsh__shell__option_8h.md#define-wsh_shell_opt_str) (acc, short, long, descr) `WSH\_SHELL\_OPTION\_STR, (acc), 1, (short), (long), WSH\_SHELL\_OPT\_DESCR(descr)`<br>_Define a string argument option._  |
| define  | [**WSH\_SHELL\_OPT\_WAITS\_INPUT**](wsh__shell__option_8h.md#define-wsh_shell_opt_waits_input) (acc) `WSH\_SHELL\_OPTION\_WAITS\_INPUT, (acc), 0, NULL, NULL, NULL`<br>_Define an option that triggers when input is provided with no flags._  |
| define  | [**WSH\_SHELL\_OPT\_WO\_PARAM**](wsh__shell__option_8h.md#define-wsh_shell_opt_wo_param) (acc, short, long, descr) `WSH\_SHELL\_OPTION\_WO\_PARAM, (acc), 0, (short), (long), WSH\_SHELL\_OPT\_DESCR(descr)`<br>_Define an option that requires no arguments._  |
| define  | [**X\_ENTRY**](wsh__shell__option_8h.md#define-x_entry) (id, str) `id,`<br> |
| define  | [**X\_ENTRY**](wsh__shell__option_8h.md#define-x_entry) (id, str) `id,`<br> |

## Detailed Description


This file provides structures and macros for defining and managing command-line options in the WSH shell framework.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### enum WSH\_SHELL\_OPTION\_TYPE\_t 

_Enumeration of all option types used by the shell._ 
```C++
enum WSH_SHELL_OPTION_TYPE_t {
    WSH_SHELL_OPTION_TYPES_TABLE =() WSH_SHELL_OPTION_ENUM_SIZE
};
```




<hr>
## Public Static Attributes Documentation




### variable WshShell\_OptionTypeNames 

_Human-readable names of option types._ 
```C++
const WshShell_Char_t* WshShell_OptionTypeNames[];
```




<hr>
## Public Static Functions Documentation




### function WshShell\_OptTypeStr\_Get 

_Return the string name of an option type._ 
```C++
static inline const WshShell_Char_t * WshShell_OptTypeStr_Get (
    WSH_SHELL_OPTION_TYPE_t optType
) 
```





**Parameters:**


* `optType` The option type. 



**Returns:**

Option type name as string. 





        

<hr>
## Macro Definition Documentation





### define WSH\_SHELL\_OPTION\_TYPES\_TABLE 

_Internal macro: full list of option types._ 
```C++
#define WSH_SHELL_OPTION_TYPES_TABLE (
    
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_OPT\_ACCESS\_ANY 

```C++
#define WSH_SHELL_OPT_ACCESS_ANY `((WshShell_Size_t)(~0U))`
```




<hr>



### define WSH\_SHELL\_OPT\_ACCESS\_EXECUTE 

```C++
#define WSH_SHELL_OPT_ACCESS_EXECUTE `0x04`
```




<hr>



### define WSH\_SHELL\_OPT\_ACCESS\_NONE 

```C++
#define WSH_SHELL_OPT_ACCESS_NONE `0x00`
```




<hr>



### define WSH\_SHELL\_OPT\_ACCESS\_READ 

```C++
#define WSH_SHELL_OPT_ACCESS_READ `0x01`
```




<hr>



### define WSH\_SHELL\_OPT\_ACCESS\_WRITE 

```C++
#define WSH_SHELL_OPT_ACCESS_WRITE `0x02`
```




<hr>



### define WSH\_SHELL\_OPT\_DESCR 

```C++
#define WSH_SHELL_OPT_DESCR (
    descr
) `""`
```




<hr>



### define WSH\_SHELL\_OPT\_END 

_Marks the end of an option array._ 
```C++
#define WSH_SHELL_OPT_END (
    
) `WSH_SHELL_OPTION_END, WSH_SHELL_OPT_ACCESS_ANY, 0, NULL, NULL, NULL`
```




<hr>



### define WSH\_SHELL\_OPT\_FLOAT 

_Define a float argument option._ 
```C++
#define WSH_SHELL_OPT_FLOAT (
    acc,
    short,
    long,
    descr
) `WSH_SHELL_OPTION_FLOAT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)`
```





**Parameters:**


* `acc` Access rights mask. 
* `short` Short flag. 
* `long` Long flag. 
* `descr` Help description. 




        

<hr>



### define WSH\_SHELL\_OPT\_HELP 

_Define a built-in help option (e.g._ `"--help"` _or_`"-h"` _)._
```C++
#define WSH_SHELL_OPT_HELP (
    
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_OPT\_INT 

_Define an integer argument option._ 
```C++
#define WSH_SHELL_OPT_INT (
    acc,
    short,
    long,
    descr
) `WSH_SHELL_OPTION_INT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)`
```





**Parameters:**


* `acc` Access rights mask. 
* `short` Short flag. 
* `long` Long flag. 
* `descr` Help description. 




        

<hr>



### define WSH\_SHELL\_OPT\_INTERACT 

_Define an option for entering interactive mode._ 
```C++
#define WSH_SHELL_OPT_INTERACT (
    
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_OPT\_MULTI\_ARG 

_Define an option that accepts multiple arguments._ 
```C++
#define WSH_SHELL_OPT_MULTI_ARG (
    acc,
    argnum,
    short,
    long,
    descr
) `WSH_SHELL_OPTION_MULTI_ARG, (acc), (argnum), (short), (long), WSH_SHELL_OPT_DESCR(descr)`
```





**Parameters:**


* `acc` Access rights mask. 
* `argnum` Number of arguments. 
* `short` Short flag. 
* `long` Long flag. 
* `descr` Help description. 




        

<hr>



### define WSH\_SHELL\_OPT\_NO 

_Define a special option that matches the command name only (no flags)._ 
```C++
#define WSH_SHELL_OPT_NO (
    acc
) `WSH_SHELL_OPTION_NO, (acc), 0, NULL, NULL, NULL`
```





**Parameters:**


* `acc` Access rights mask. 




        

<hr>



### define WSH\_SHELL\_OPT\_STR 

_Define a string argument option._ 
```C++
#define WSH_SHELL_OPT_STR (
    acc,
    short,
    long,
    descr
) `WSH_SHELL_OPTION_STR, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)`
```





**Parameters:**


* `acc` Access rights mask. 
* `short` Short flag. 
* `long` Long flag. 
* `descr` Help description. 




        

<hr>



### define WSH\_SHELL\_OPT\_WAITS\_INPUT 

_Define an option that triggers when input is provided with no flags._ 
```C++
#define WSH_SHELL_OPT_WAITS_INPUT (
    acc
) `WSH_SHELL_OPTION_WAITS_INPUT, (acc), 0, NULL, NULL, NULL`
```





**Parameters:**


* `acc` Access rights mask. 




        

<hr>



### define WSH\_SHELL\_OPT\_WO\_PARAM 

_Define an option that requires no arguments._ 
```C++
#define WSH_SHELL_OPT_WO_PARAM (
    acc,
    short,
    long,
    descr
) `WSH_SHELL_OPTION_WO_PARAM, (acc), 0, (short), (long), WSH_SHELL_OPT_DESCR(descr)`
```





**Parameters:**


* `acc` Access rights mask. 
* `short` Short flag (e.g. `"-a"`). 
* `long` Long flag (e.g. `"--all"`). 
* `descr` Help description. 




        

<hr>



### define X\_ENTRY 

```C++
#define X_ENTRY (
    id,
    str
) `id,`
```




<hr>



### define X\_ENTRY 

```C++
#define X_ENTRY (
    id,
    str
) `id,`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_option.h`

