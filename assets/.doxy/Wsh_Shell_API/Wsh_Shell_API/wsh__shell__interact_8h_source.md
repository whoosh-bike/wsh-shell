

# File wsh\_shell\_interact.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_interact.h**](wsh__shell__interact_8h.md)

[Go to the documentation of this file](wsh__shell__interact_8h.md)


```C++

#ifndef __WSH_SHELL_INTERACT_H
#define __WSH_SHELL_INTERACT_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WshShellInteractive_CmdHandler_t)(WshShellIO_CommandLine_t* pCommandLine);

typedef struct {
    WshShellInteractive_CmdHandler_t Handler; 
    WshShell_Char_t CmdName[WSH_SHELL_CMD_NAME_LEN]; 
} WshShell_Interact_t;

void WshShellInteract_Attach(WshShell_Interact_t* pInteract, const WshShell_Char_t* pcName,
                             WshShellInteractive_CmdHandler_t handler);

void WshShellInteract_AppendLineBreak(WshShellIO_CommandLine_t* pCommandLine);

void WshShellInteract_Flush(WshShell_Interact_t* pInteract);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_INTERACT_H */
```


