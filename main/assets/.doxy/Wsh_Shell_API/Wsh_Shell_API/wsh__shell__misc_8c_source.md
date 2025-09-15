

# File wsh\_shell\_misc.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_misc.c**](wsh__shell__misc_8c.md)

[Go to the documentation of this file](wsh__shell__misc_8c.md)


```C++
#include "wsh_shell_misc.h"

WshShell_U32_t WshShellMisc_CalcJenkinsHash(const WshShell_U8_t* pcBuff, WshShell_Size_t len) {
    WSH_SHELL_ASSERT(pcBuff);

    WshShell_U32_t hash = 0;
    for (WshShell_Size_t idx = 0; idx < len; idx++) {
        hash += pcBuff[idx];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}
```


