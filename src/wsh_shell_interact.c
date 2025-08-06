#include "wsh_shell_interact.h"

void WshShellInteract_Attach(WshShell_Interact_t* pInteract, const WshShell_Char_t* pcName,
                             InteractiveCmd_t exec) {
    WSH_SHELL_ASSERT(pInteract && pcName && exec);
    if (!pInteract || !pcName || !exec)
        return;

    pInteract->Exec = exec;

    WshShell_Size_t bufSize = sizeof(pInteract->CmdName);
    WshShell_Size_t len     = WSH_SHELL_STRLEN(pcName);
    if (len >= bufSize)
        len = bufSize - 1;

    WSH_SHELL_MEMCPY(pInteract->CmdName, pcName, len);
    pInteract->CmdName[len] = '\0';
}

void WshShellInteract_AppendLineBreak(WshShellIO_CommandLine_t* pInter) {
    WSH_SHELL_ASSERT(pInter);
    if (!pInter)
        return;

    WshShell_Size_t len    = pInter->Len;
    WshShell_Size_t maxLen = sizeof(pInter->Buff);

    if (len + 2 < maxLen) {
        pInter->Buff[len++] = '\r';
        pInter->Buff[len++] = '\n';
    } else {
        WSH_SHELL_PRINT_ERR("Command too long!");
    }
}

void WshShellInteract_Flush(WshShell_Interact_t* pInteract) {
    WSH_SHELL_ASSERT(pInteract);
    if (!pInteract)
        return;

    pInteract->Exec = NULL;
    WSH_SHELL_MEMSET((void*)pInteract->CmdName, 0, sizeof(pInteract->CmdName));
}
