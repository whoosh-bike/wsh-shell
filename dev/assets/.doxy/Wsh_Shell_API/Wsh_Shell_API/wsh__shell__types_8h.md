

# File wsh\_shell\_types.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_types.h**](wsh__shell__types_8h.md)

[Go to the source code of this file](wsh__shell__types_8h_source.md)

_Basic type definitions and return state enumeration for WshShell._ [More...](#detailed-description)

* `#include <stdarg.h>`
* `#include <stdbool.h>`
* `#include <stddef.h>`
* `#include <stdint.h>`
* `#include <stdio.h>`
* `#include <stdlib.h>`
* `#include <string.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**WSH\_SHELL\_RET\_STATE\_t**](#enum-wsh_shell_ret_state_t)  <br> |
| typedef bool | [**WshShell\_Bool\_t**](#typedef-wshshell_bool_t)  <br> |
| typedef char | [**WshShell\_Char\_t**](#typedef-wshshell_char_t)  <br> |
| typedef int16\_t | [**WshShell\_S16\_t**](#typedef-wshshell_s16_t)  <br> |
| typedef int32\_t | [**WshShell\_S32\_t**](#typedef-wshshell_s32_t)  <br> |
| typedef int8\_t | [**WshShell\_S8\_t**](#typedef-wshshell_s8_t)  <br> |
| typedef size\_t | [**WshShell\_Size\_t**](#typedef-wshshell_size_t)  <br> |
| typedef uint16\_t | [**WshShell\_U16\_t**](#typedef-wshshell_u16_t)  <br> |
| typedef uint32\_t | [**WshShell\_U32\_t**](#typedef-wshshell_u32_t)  <br> |
| typedef uint8\_t | [**WshShell\_U8\_t**](#typedef-wshshell_u8_t)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**RetStateBuff**](#variable-retstatebuff)   = `/* multi line expression */`<br> |
















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**WshShell\_GetRetStateStr**](#function-wshshell_getretstatestr) (WSH\_SHELL\_RET\_STATE\_t retState) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_ARR\_LEN**](wsh__shell__types_8h.md#define-wsh_shell_arr_len) (a) `(sizeof((a)) / sizeof((a[0])))`<br> |
| define  | [**WSH\_SHELL\_RET\_STATE\_TABLE**](wsh__shell__types_8h.md#define-wsh_shell_ret_state_table) () `/* multi line expression */`<br> |
| define  | [**X\_ENTRY**](wsh__shell__types_8h.md#define-x_entry) (en, str) `en,`<br> |
| define  | [**X\_ENTRY**](wsh__shell__types_8h.md#define-x_entry) (en, str) `en,`<br> |

## Detailed Description


This header defines common typedefs for basic data types used throughout the WshShell project, improving code readability and portability. It also defines the enumeration for return states used by shell functions, along with utility functions to convert return codes to human-readable strings.


The file includes:
* Boolean, character, and integer typedefs with explicit sizes.
* Macro for calculating array length.
* Enumeration of return states for consistent error and status reporting.
* Inline helper to get string representation of return states.






**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### enum WSH\_SHELL\_RET\_STATE\_t 

```C++
enum WSH_SHELL_RET_STATE_t {
    WSH_SHELL_RET_STATE_TABLE =() WSH_SHELL_RET_STATE_ENUM_SIZE
};
```




<hr>



### typedef WshShell\_Bool\_t 

```C++
typedef bool WshShell_Bool_t;
```




<hr>



### typedef WshShell\_Char\_t 

```C++
typedef char WshShell_Char_t;
```




<hr>



### typedef WshShell\_S16\_t 

```C++
typedef int16_t WshShell_S16_t;
```




<hr>



### typedef WshShell\_S32\_t 

```C++
typedef int32_t WshShell_S32_t;
```




<hr>



### typedef WshShell\_S8\_t 

```C++
typedef int8_t WshShell_S8_t;
```




<hr>



### typedef WshShell\_Size\_t 

```C++
typedef size_t WshShell_Size_t;
```




<hr>



### typedef WshShell\_U16\_t 

```C++
typedef uint16_t WshShell_U16_t;
```




<hr>



### typedef WshShell\_U32\_t 

```C++
typedef uint32_t WshShell_U32_t;
```




<hr>



### typedef WshShell\_U8\_t 

```C++
typedef uint8_t WshShell_U8_t;
```




<hr>
## Public Static Attributes Documentation




### variable RetStateBuff 

```C++
const WshShell_Char_t* RetStateBuff[];
```




<hr>
## Public Static Functions Documentation




### function WshShell\_GetRetStateStr 

```C++
static inline const WshShell_Char_t * WshShell_GetRetStateStr (
    WSH_SHELL_RET_STATE_t retState
) 
```




<hr>
## Macro Definition Documentation





### define WSH\_SHELL\_ARR\_LEN 

```C++
#define WSH_SHELL_ARR_LEN (
    a
) `(sizeof((a)) / sizeof((a[0])))`
```




<hr>



### define WSH\_SHELL\_RET\_STATE\_TABLE 

```C++
#define WSH_SHELL_RET_STATE_TABLE (
    
) `/* multi line expression */`
```




<hr>



### define X\_ENTRY 

```C++
#define X_ENTRY (
    en,
    str
) `en,`
```




<hr>



### define X\_ENTRY 

```C++
#define X_ENTRY (
    en,
    str
) `en,`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_types.h`

