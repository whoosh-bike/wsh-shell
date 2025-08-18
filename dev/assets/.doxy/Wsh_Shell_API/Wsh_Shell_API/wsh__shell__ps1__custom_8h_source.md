

# File wsh\_shell\_ps1\_custom.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_ps1\_custom.h**](wsh__shell__ps1__custom_8h.md)

[Go to the documentation of this file](wsh__shell__ps1__custom_8h.md)


```C++

#ifndef __WSH_SHELL_PS1_CUSTOM_H
#define __WSH_SHELL_PS1_CUSTOM_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const WshShell_Char_t* UserName; 
    const WshShell_Char_t* DevName;  
    WshShell_Char_t* InterCmdName;   
} WshShell_PS1Data_t;

void WshShell_GeneratePS1(WshShell_Char_t* pPS1, WshShell_PS1Data_t* pPS1Data);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_PS1_CUSTOM_H */
```


