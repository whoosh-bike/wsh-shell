#include "shell.h"
#include "wsh_shell.h"

static WshShell_t Shell = {0};

static const WshShellUser_t UserTable[] = {
    {
        .Login  = "root",
        .Pass   = "1234",
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_OPT_ACCESS_ANY,
    },
};

static WshShellHistory_t Shell_HistoryStorage;

WshShellHistory_t WshShellHistory_Read(void) {
    return Shell_HistoryStorage;
}

void WshShellHistory_Write(WshShellHistory_t history) {
    memcpy((void*)&Shell_HistoryStorage, (void*)&history, sizeof(WshShellHistory_t));
}

static void Shell_AuthClbk(void* pCtx) {
    (void)(pCtx);
}

static void Shell_DeAuthClbk(void* pCtx) {
    (void)(pCtx);
}

static void Shell_SymInClbk(void* pCtx) {
    (void)(pCtx);
}

static WshShell_ExtCallbacks_t Shell_Callbacks = {
    .Auth     = Shell_AuthClbk,
    .DeAuth   = Shell_DeAuthClbk,
    .SymbolIn = Shell_SymInClbk,
};

void Shell_Init(const char* pcHostName) {
    if (WshShell_Init(&Shell, pcHostName, NULL, &Shell_Callbacks) != WSH_SHELL_RET_STATE_SUCCESS) {
        return;
    }

    if (WshShellUser_Attach(&(Shell.Users), UserTable, WSH_SHELL_ARR_LEN(UserTable)) !=
        WSH_SHELL_RET_STATE_SUCCESS) {
        return;
    }

    WshShellHistory_Init(&Shell.HistoryIO, WshShellHistory_Read, WshShellHistory_Write);

    // WshShell_Auth(&Shell, "root", "1234"); For quick auth
}

void Shell_SendChar(char ch) {
    WshShell_InsertChar(&Shell, ch);
}
