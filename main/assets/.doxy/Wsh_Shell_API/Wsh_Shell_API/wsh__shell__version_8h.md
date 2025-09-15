

# File wsh\_shell\_version.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_version.h**](wsh__shell__version_8h.md)

[Go to the source code of this file](wsh__shell__version_8h_source.md)



































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_VERSION\_MAJOR**](wsh__shell__version_8h.md#define-wsh_shell_version_major)  `2`<br> |
| define  | [**WSH\_SHELL\_VERSION\_MINOR**](wsh__shell__version_8h.md#define-wsh_shell_version_minor)  `2`<br> |
| define  | [**WSH\_SHELL\_VERSION\_STR**](wsh__shell__version_8h.md#define-wsh_shell_version_str)  `\_WSH\_SHELL\_STR(WSH\_SHELL\_VERSION\_MAJOR) "." \_WSH\_SHELL\_STR(WSH\_SHELL\_VERSION\_MINOR)`<br> |
| define  | [**\_WSH\_SHELL\_STR**](wsh__shell__version_8h.md#define-_wsh_shell_str) (x) `\_WSH\_SHELL\_STR\_HELPER(x)`<br> |
| define  | [**\_WSH\_SHELL\_STR\_HELPER**](wsh__shell__version_8h.md#define-_wsh_shell_str_helper) (x) `#x`<br> |

## Macro Definition Documentation





### define WSH\_SHELL\_VERSION\_MAJOR 

```C++
#define WSH_SHELL_VERSION_MAJOR `2`
```




<hr>



### define WSH\_SHELL\_VERSION\_MINOR 

```C++
#define WSH_SHELL_VERSION_MINOR `2`
```




<hr>



### define WSH\_SHELL\_VERSION\_STR 

```C++
#define WSH_SHELL_VERSION_STR `_WSH_SHELL_STR(WSH_SHELL_VERSION_MAJOR) "." _WSH_SHELL_STR(WSH_SHELL_VERSION_MINOR)`
```




<hr>



### define \_WSH\_SHELL\_STR 

```C++
#define _WSH_SHELL_STR (
    x
) `_WSH_SHELL_STR_HELPER(x)`
```




<hr>



### define \_WSH\_SHELL\_STR\_HELPER 

```C++
#define _WSH_SHELL_STR_HELPER (
    x
) `#x`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_version.h`

