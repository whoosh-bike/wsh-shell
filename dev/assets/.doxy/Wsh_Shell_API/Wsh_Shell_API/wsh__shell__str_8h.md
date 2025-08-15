

# File wsh\_shell\_str.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_str.h**](wsh__shell__str_8h.md)

[Go to the source code of this file](wsh__shell__str_8h_source.md)

_Common string operations for user input processing._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_io.h"`
* `#include "wsh_shell_option.h"`
* `#include "wsh_shell_types.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellStr\_PS1Data\_t**](structWshShellStr__PS1Data__t.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellStr\_AccessBitsToStr**](#function-wshshellstr_accessbitstostr) (WshShell\_Size\_t access, WshShell\_Char\_t \* pOutStr) <br>_Converts access permission bits to a human-readable string like "rwx"._  |
|  void | [**WshShellStr\_DecrInterCnt**](#function-wshshellstr_decrintercnt) (WshShell\_Size\_t \* pInterCnt) <br>_Decrease buffer counter._  |
|  void | [**WshShellStr\_GeneratePS1**](#function-wshshellstr_generateps1) (WshShell\_Char\_t \* pPS1, [**WshShellStr\_PS1Data\_t**](structWshShellStr__PS1Data__t.md) \* pPS1Data) <br>_Applies the PS1 template and generates the final PS1 string._  |
|  void | [**WshShellStr\_GroupBitsToStr**](#function-wshshellstr_groupbitstostr) (WshShell\_Size\_t group, WshShell\_Char\_t \* pOutStr) <br>_Converts group bitmask into symbolic group string (e.g. "\*--\*", "---\*", etc)._  |
|  void | [**WshShellStr\_IncrInterCnt**](#function-wshshellstr_incrintercnt) (WshShell\_Size\_t \* pInterCnt, WshShell\_Size\_t buffSize) <br>_Increase buffer counter._  |
|  WshShell\_Bool\_t | [**WshShellStr\_IsPrintableAscii**](#function-wshshellstr_isprintableascii) (WshShell\_Char\_t ch) <br>_Check whether the given character is a printable ASCII symbol._  |
|  void | [**WshShellStr\_ParseToArgcArgv**](#function-wshshellstr_parsetoargcargv) (WshShell\_Char\_t \* pStr, WshShell\_Size\_t \* pArgNum, const WshShell\_Char\_t \* pArgBuff, WshShell\_Size\_t maxArgNum) <br>_Parse a string into space-separated tokens, handling quoted substrings as single tokens._  |
|  WshShell\_Char\_t \* | [**WshShellStr\_TrimString**](#function-wshshellstr_trimstring) (WshShell\_Char\_t \* pString, WshShell\_Size\_t len) <br>_Trim tabulation and spaces from beginning and end of a string._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_PRESS\_ENTER\_TO\_LOG\_IN\_STR**](wsh__shell__str_8h.md#define-wsh_shell_press_enter_to_log_in_str)  `"Press &lt;Enter&gt; to log in..."`<br> |

## Detailed Description




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Functions Documentation




### function WshShellStr\_AccessBitsToStr 

_Converts access permission bits to a human-readable string like "rwx"._ 
```C++
void WshShellStr_AccessBitsToStr (
    WshShell_Size_t access,
    WshShell_Char_t * pOutStr
) 
```



This function translates access bit flags into a POSIX-style permission string. The result is written into the provided buffer.


The output format is:
* 'r`if`WSH\_SHELL\_OPT\_ACCESS\_READ`is set, otherwise`'-'` -`'w'`if`WSH\_SHELL\_OPT\_ACCESS\_WRITE`is set, otherwise`'-'` -`'x'`if`WSH\_SHELL\_OPT\_ACCESS\_EXECUTE`is set, otherwise`'-'` -`'A'`if`access == WSH\_SHELL\_OPT\_ACCESS\_ANY` (optional fourth character)




Examples:
* `0x00` → `"---"`
* `0x03` → `"rw-"`
* `0x07` → `"rwx"`
* `0xFFFFFFFF`→ `"rwxA"`






**Parameters:**


* `access` Bitmask of access flags (e.g. WSH\_SHELL\_OPT\_ACCESS\_READ). 
* `pOutStr` Buffer to write result to. Must be at least 5 bytes long. 




        

<hr>



### function WshShellStr\_DecrInterCnt 

_Decrease buffer counter._ 
```C++
void WshShellStr_DecrInterCnt (
    WshShell_Size_t * pInterCnt
) 
```





**Parameters:**


* `pInterCnt` Pointer to a counter. 




        

<hr>



### function WshShellStr\_GeneratePS1 

_Applies the PS1 template and generates the final PS1 string._ 
```C++
void WshShellStr_GeneratePS1 (
    WshShell_Char_t * pPS1,
    WshShellStr_PS1Data_t * pPS1Data
) 
```



Expands a predefined PS1 template (`WSH_SHELL_PS1_TEMPLATE`) into a complete PS1 string, substituting special format specifiers with runtime values like username, device name, and ANSI escape sequences for styling (colors, bold, reset).


Supported format specifiers in the template:
* `u` — current username (from `pUser`)
* `d` — current device name (from `pDevice`)
* `cN` — ANSI color code from predefined color map (0 ≤ N ≤ 9)
* `b` — ANSI escape sequence for bold text
* `r` — ANSI escape sequence to reset all styles
* Any unknown `%` sequence is copied verbatim as `X`.




The output is truncated if it would exceed `WSH_SHELL_PS1_MAX_LEN - 1`. The result is always null-terminated.




**Parameters:**


* `pPS1` Output buffer for the final PS1 string. Must be at least `WSH_SHELL_PS1_MAX_LEN` in size. 
* `pPS1Data` Pointer to the user name, divice name or other strings. 




        

<hr>



### function WshShellStr\_GroupBitsToStr 

_Converts group bitmask into symbolic group string (e.g. "\*--\*", "---\*", etc)._ 
```C++
void WshShellStr_GroupBitsToStr (
    WshShell_Size_t group,
    WshShell_Char_t * pOutStr
) 
```



Produces a fixed-length string of '\*' and '-' characters from highest to lowest group index.


Example for 4 groups:
* WSH\_SHELL\_CMD\_GROUP\_ADMIN = bit 0 → rightmost char
* WSH\_SHELL\_CMD\_GROUP\_MANUF = bit 3 → leftmost char






**Parameters:**


* `group` Bitmask of groups. 
* `pOutStr` Output buffer (must be at least WSH\_SHELL\_GROUP\_STR\_LEN). 




        

<hr>



### function WshShellStr\_IncrInterCnt 

_Increase buffer counter._ 
```C++
void WshShellStr_IncrInterCnt (
    WshShell_Size_t * pInterCnt,
    WshShell_Size_t buffSize
) 
```





**Parameters:**


* `pInterCnt` Pointer to a counter. 
* `buffSize` Size of a buffer. 




        

<hr>



### function WshShellStr\_IsPrintableAscii 

_Check whether the given character is a printable ASCII symbol._ 
```C++
WshShell_Bool_t WshShellStr_IsPrintableAscii (
    WshShell_Char_t ch
) 
```



This function checks if the character belongs to the range of printable ASCII characters (from 0x20 ' ' to 0x7E '~').




**Parameters:**


* `ch` Character to check.



**Returns:**

true if the character is printable ASCII; false otherwise. 





        

<hr>



### function WshShellStr\_ParseToArgcArgv 

_Parse a string into space-separated tokens, handling quoted substrings as single tokens._ 
```C++
void WshShellStr_ParseToArgcArgv (
    WshShell_Char_t * pStr,
    WshShell_Size_t * pArgNum,
    const WshShell_Char_t * pArgBuff,
    WshShell_Size_t maxArgNum
) 
```



This function splits the input string `pStr` into tokens by replacing spaces with null terminators (`\0`). Tokens are written as pointers to the beginning of each token into the `pArgBuff` array. Quoted substrings (enclosed in double quotes `"`), if any, are treated as a single token. Quotes themselves are removed during parsing. ч 

**Note:**

This function modifies the input string in-place. Spaces and quotes are replaced with `\0`. Escaped or nested quotes are not supported.


Example: Input: `"arg1 arg2 \"quoted string" arg3"</tt>
 Output: <tt>pArgBuff = { "arg1", "arg2", "quoted string", "arg3" }`, `*pArgNum = 4`




**Parameters:**


* `pStr` Pointer to the null-terminated string to be parsed. Will be modified in-place. 
* `pArgNum` Pointer to a counter that will be set to the number of parsed tokens. 
* `pArgBuff` Array of string pointers (tokens). Must be large enough to hold all tokens. 
* `maxArgNum` Maximum number of tokens that can be stored in `pArgBuff`. Excess tokens are discarded. 




        

<hr>



### function WshShellStr\_TrimString 

_Trim tabulation and spaces from beginning and end of a string._ 
```C++
WshShell_Char_t * WshShellStr_TrimString (
    WshShell_Char_t * pString,
    WshShell_Size_t len
) 
```





**Parameters:**


* `pString` Pointer to a string to be trimmed. 
* `len` Length of an input string.



**Returns:**

WshShell\_Char\_t\*: Pointer to a trimmed string. 





        

<hr>
## Macro Definition Documentation





### define WSH\_SHELL\_PRESS\_ENTER\_TO\_LOG\_IN\_STR 

```C++
#define WSH_SHELL_PRESS_ENTER_TO_LOG_IN_STR `"Press <Enter> to log in..."`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_str.h`

