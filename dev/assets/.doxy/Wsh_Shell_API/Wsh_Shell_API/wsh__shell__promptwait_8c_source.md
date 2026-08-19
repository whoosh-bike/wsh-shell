

# File wsh\_shell\_promptwait.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_promptwait.c**](wsh__shell__promptwait_8c.md)

[Go to the documentation of this file](wsh__shell__promptwait_8c.md)


```C++
#include "wsh_shell_promptwait.h"

void WshShellPromptWait_Flush(WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);
    if (!pWait)
        return;

    pWait->Handler     = NULL;
    pWait->Ctx         = NULL;
    pWait->RejectedNum = 0;
}

#if WSH_SHELL_PROMPT_WAIT
void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler, void* pCtx) {
    WSH_SHELL_ASSERT(pWait && handler);
    if (!pWait || !handler)
        return;

    pWait->Handler     = handler;
    pWait->Ctx         = pCtx;
    /* Every wait starts with a fresh hint budget. */
    pWait->RejectedNum = 0;
}

WshShell_Bool_t WshShellPromptWait_HintIsNeeded(const WshShellPromptWait_t* pcWait) {
    if (!pcWait)
        return false;

    return pcWait->RejectedNum < WSH_SHELL_PROMPT_WAIT_HINT_RETRIES;
}

WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait, WshShell_Char_t symbol) {
    WSH_SHELL_ASSERT(pWait);
    if (!pWait)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pWait->Handler) {
        /* The handler sees every symbol, Ctrl+C included, so it can undo
         * whatever the wait was guarding. */
        WshShell_Bool_t res = pWait->Handler(symbol, pWait);

        /* Ctrl+C then always escapes the wait, whatever the handler returned:
         * a handler can never trap the shell. The cancel handler in the main
         * symbol dispatcher runs afterwards as usual. */
        if (symbol == WSH_SHELL_SYM_CANCEL) {
            WshShellPromptWait_Flush(pWait);
            return WSH_SHELL_RET_STATE_ERR_EMPTY;
        }

        /* Counted after the handler ran, so the handler still sees the budget
         * for the keystroke it is refusing right now. Saturates at the limit:
         * nothing above it changes behaviour, and it cannot overflow. */
        if (!res) {
            if (pWait->RejectedNum < WSH_SHELL_PROMPT_WAIT_HINT_RETRIES)
                pWait->RejectedNum++;

            WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
        }

        return res == true ? WSH_SHELL_RET_STATE_SUCCESS : WSH_SHELL_RET_STATE_ERR_BUSY;
    }

    return WSH_SHELL_RET_STATE_ERR_EMPTY;
}

WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);

    /* Ctrl+C is force-flushed by the caller; nothing to undo, stay quiet. */
    if (symbol == WSH_SHELL_SYM_CANCEL)
        return true;

    if (symbol == '\r' || symbol == '\n') {
        WshShellPromptWait_Flush(pWait);
        return true;
    } else {
        if (WshShellPromptWait_HintIsNeeded(pWait))
            WSH_SHELL_PRINT_SYS("Press <Enter> to continue...\r\n");

        return false;
    }
}

/* The signature is pinned by WshShellPromptWait_Handler_t: handlers are stored in
 * that type, and a const parameter would no longer match it. */
// cppcheck-suppress constParameterPointer
WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    WSH_SHELL_ASSERT(pWait);

    /* Ctrl+C is force-flushed by the caller; nothing to undo, stay quiet. */
    if (symbol == WSH_SHELL_SYM_CANCEL)
        return true;

    if (symbol == 'Y' || symbol == 'y') {
        WSH_SHELL_PRINT_SYS("Yes selected\r\n");
    } else if (symbol == 'N' || symbol == 'n') {
        WSH_SHELL_PRINT_SYS("No selected\r\n");
    } else {
        if (WshShellPromptWait_HintIsNeeded(pWait))
            WSH_SHELL_PRINT_SYS("Invalid input, press Y or N\r\n");

        return false;
    }

    return true;
}

#else /* WSH_SHELL_PROMPT_WAIT */

void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler, void* pCtx) {
    return;
}

WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait, WshShell_Char_t symbol) {
    return WSH_SHELL_RET_STATE_SUCCESS;
}

WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    return true;
}

WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait) {
    return true;
}

WshShell_Bool_t WshShellPromptWait_HintIsNeeded(const WshShellPromptWait_t* pcWait) {
    return false;
}

#endif /* WSH_SHELL_PROMPT_WAIT */
```


