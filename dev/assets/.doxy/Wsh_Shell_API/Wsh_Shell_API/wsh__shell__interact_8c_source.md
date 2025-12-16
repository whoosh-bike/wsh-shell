

# File wsh\_shell\_interact.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_interact.c**](wsh__shell__interact_8c.md)

[Go to the documentation of this file](wsh__shell__interact_8c.md)


```C++
#include "wsh_shell_interact.h"

#if WSH_SHELL_INTERACTIVE_MODE

void WshShellInteract_Attach(WshShellInteract_t* pInteract, const WshShell_Char_t* pcName,
                             WshShellInteractive_CmdHandler_t handler) {
    WSH_SHELL_ASSERT(pInteract && pcName && handler);
    if (!pInteract || !pcName || !handler)
        return;

    pInteract->Handler = handler;

    WshShell_Size_t bufSize = sizeof(pInteract->CmdName);
    WshShell_Size_t len     = WSH_SHELL_STRLEN(pcName);
    if (len >= bufSize)
        len = bufSize - 1;

    WSH_SHELL_MEMCPY(pInteract->CmdName, pcName, len);
    pInteract->CmdName[len] = '\0';
}

void WshShellInteract_AppendLineBreak(WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    WshShell_Size_t len    = pCommandLine->Len;
    WshShell_Size_t maxLen = sizeof(pCommandLine->Buff);

    if (len + 3 < maxLen) {
        pCommandLine->Buff[len++] = '\r';
        pCommandLine->Buff[len++] = '\n';
        pCommandLine->Buff[len++] = '\0';
    } else {
        WSH_SHELL_PRINT_ERR("Command too long!");
    }
}

void WshShellInteract_Flush(WshShellInteract_t* pInteract) {
    WSH_SHELL_ASSERT(pInteract);
    if (!pInteract)
        return;

    pInteract->Handler = NULL;
    WSH_SHELL_MEMSET((void*)pInteract->CmdName, 0, sizeof(pInteract->CmdName));
}

#else /* WSH_SHELL_INTERACTIVE_MODE */

void WshShellInteract_Attach(WshShellInteract_t* pInteract, const WshShell_Char_t* pcName,
                             WshShellInteractive_CmdHandler_t handler) {
    (void)(pInteract);
    (void)(pcName);
    (void)(handler);

    WSH_SHELL_PRINT_WARN("Interactive mode disabled\r\n");
}

void WshShellInteract_AppendLineBreak(WshShellIO_CommandLine_t* pCommandLine) {
    (void)(pCommandLine);
}

void WshShellInteract_Flush(WshShellInteract_t* pInteract) {
    (void)(pInteract);
}

#endif /* WSH_SHELL_INTERACTIVE_MODE */
```


