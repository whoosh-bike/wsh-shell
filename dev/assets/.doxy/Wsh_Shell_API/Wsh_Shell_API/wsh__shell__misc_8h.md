

# File wsh\_shell\_misc.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_misc.h**](wsh__shell__misc_8h.md)

[Go to the source code of this file](wsh__shell__misc_8h_source.md)

_Miscellaneous helper utilities for the shell._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_types.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  WshShell\_U32\_t | [**WshShellMisc\_CalcJenkinsHash**](#function-wshshellmisc_calcjenkinshash) (const WshShell\_U8\_t \* pcBuff, WshShell\_Size\_t len) <br>_Calculate Jenkins one-at-a-time hash._  |




























## Detailed Description


This module provides various helper functions that do not belong to specific subsystems, but are commonly used across the shell. Currently includes hashing utilities and may be extended in the future.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2025 





    
## Public Functions Documentation




### function WshShellMisc\_CalcJenkinsHash 

_Calculate Jenkins one-at-a-time hash._ 
```C++
WshShell_U32_t WshShellMisc_CalcJenkinsHash (
    const WshShell_U8_t * pcBuff,
    WshShell_Size_t len
) 
```



This function computes the 32-bit Jenkins one-at-a-time hash for the given buffer. It is a simple and fast non-cryptographic (!) hash function, commonly used for hash tables and quick data checksums.




**Parameters:**


* `pcBuff` Pointer to the input buffer. 
* `len` Number of bytes in the input buffer.



**Returns:**

32-bit hash value computed over the input buffer.




**Note:**

This hash is not suitable for cryptographic purposes !!! It is designed for speed and good distribution in hash tables.


Jenkins hash function [https://en.wikipedia.org/wiki/Jenkins\_hash\_function](https://en.wikipedia.org/wiki/Jenkins_hash_function) 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_misc.h`

