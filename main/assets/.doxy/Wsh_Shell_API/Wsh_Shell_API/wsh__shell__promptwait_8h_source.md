

# File wsh\_shell\_promptwait.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_promptwait.h**](wsh__shell__promptwait_8h.md)

[Go to the documentation of this file](wsh__shell__promptwait_8h.md)


```C++

#ifndef __WSH_SHELL_PROMPTWAIT_H
#define __WSH_SHELL_PROMPTWAIT_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of command structure
struct WshShellPromptWait;

typedef WshShell_Bool_t (*WshShellPromptWait_Handler_t)(WshShell_Char_t symbol,
                                                        struct WshShellPromptWait* pWait);

typedef struct WshShellPromptWait {
    WshShellPromptWait_Handler_t Handler;
    void* Ctx;
} WshShellPromptWait_t;

void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler,
                               void* pCtx);

void WshShellPromptWait_Flush(WshShellPromptWait_t* pWait);

WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait,
                                                WshShell_Char_t symbol);

WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait);
WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_PROMPTWAIT_H */
```


