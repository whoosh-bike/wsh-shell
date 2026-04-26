

# File wsh\_shell\_misc.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_misc.c**](wsh__shell__misc_8c.md)

[Go to the source code of this file](wsh__shell__misc_8c_source.md)



* `#include "wsh_shell_misc.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  WshShell\_U32\_t | [**WshShellMisc\_CalcJenkinsHash**](#function-wshshellmisc_calcjenkinshash) (const WshShell\_U8\_t \* pcBuff, WshShell\_Size\_t len) <br>_Calculate Jenkins one-at-a-time hash._  |
|  void | [**WshShellMisc\_HexDump**](#function-wshshellmisc_hexdump) (const WshShell\_U8\_t \* pBuff, WshShell\_Size\_t len, WshShell\_Size\_t offset) <br>_Print a hex + ASCII dump of a buffer (hexdump -C style)._  |




























## Public Functions Documentation




### function WshShellMisc\_CalcJenkinsHash 

_Calculate Jenkins one-at-a-time hash._ 
```C++
WshShell_U32_t WshShellMisc_CalcJenkinsHash (
    const WshShell_U8_t * pcBuff,
    WshShell_Size_t len
) 
```



Jenkins hash function [https://en.wikipedia.org/wiki/Jenkins\_hash\_function](https://en.wikipedia.org/wiki/Jenkins_hash_function) 


        

<hr>



### function WshShellMisc\_HexDump 

_Print a hex + ASCII dump of a buffer (hexdump -C style)._ 
```C++
void WshShellMisc_HexDump (
    const WshShell_U8_t * pBuff,
    WshShell_Size_t len,
    WshShell_Size_t offset
) 
```



Each row contains the byte offset, up to 16 bytes in hex (split into two groups of 8), and their printable ASCII representation. Non-printable bytes are shown as '.'.


Example output: 
```C++
00000000  57 73 68 53 68 65 6c 6c  00 01 7e 7f 80 ff       |WshShell..~...|
```





**Parameters:**


* `pBuff` Pointer to the data buffer. May be NULL only when len == 0. 
* `len` Number of bytes to dump. 
* `offset` Base value printed in the offset column (use 0 for relative offsets). 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_misc.c`

