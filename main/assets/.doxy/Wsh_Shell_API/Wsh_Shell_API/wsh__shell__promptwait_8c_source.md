

# File wsh\_shell\_promptwait.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_promptwait.c**](wsh__shell__promptwait_8c.md)

[Go to the documentation of this file](wsh__shell__promptwait_8c.md)


```C++
#include "wsh_shell_promptwait.h"

void WshShellPromptWait_Flush(WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);
    if (!pWait)
        return;

    pWait->Handler = NULL;
    pWait->Ctx     = NULL;
}

#if WSH_SHELL_PROMPT_WAIT
void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler,
                               void* pCtx) {
    WSH_SHELL_ASSERT(pWait && handler);
    if (!pWait || !handler)
        return;

    pWait->Handler = handler;
    pWait->Ctx     = pCtx;
}

WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait,
                                                WshShell_Char_t symbol) {
    WSH_SHELL_ASSERT(pWait);
    if (!pWait)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pWait->Handler) {
        WshShell_Bool_t res = pWait->Handler(symbol, pWait);
        if (!res)
            WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);

        return res == true ? WSH_SHELL_RET_STATE_SUCCESS : WSH_SHELL_RET_STATE_ERR_BUSY;
    }

    return WSH_SHELL_RET_STATE_ERR_EMPTY;
}

WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);

    if (symbol == '\r' || symbol == '\n') {
        WshShellPromptWait_Flush(pWait);
        return true;
    } else {
        WSH_SHELL_PRINT_SYS("Press <Enter> to continue...\r\n");
        return false;
    }
}

WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);

    if (symbol == 'Y' || symbol == 'y') {
        WSH_SHELL_PRINT_SYS("Yes selected\r\n");
    } else if (symbol == 'N' || symbol == 'n') {
        WSH_SHELL_PRINT_SYS("No selected\r\n");
    } else {
        WSH_SHELL_PRINT_SYS("Invalid input\r\n");
        return false;
    }

    return true;
}

#else /* WSH_SHELL_PROMPT_WAIT */

void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler,
                               void* pCtx) {
    return;
}

WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait,
                                                WshShell_Char_t symbol) {
    return WSH_SHELL_RET_STATE_SUCCESS;
}

WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    return true;
}

WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    return true;
}

#endif /* WSH_SHELL_PROMPT_WAIT */
```


