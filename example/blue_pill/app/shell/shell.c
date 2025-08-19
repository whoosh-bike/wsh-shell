#include "shell.h"
#include "commands.h"
#include "wsh_shell.h"

static WshShell_t Shell = {0};

static const WshShellUser_t Shell_UserTable[] = {
    {
        .Login  = "root",
        .Pass   = "1234",
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_OPT_ACCESS_ANY,
    },
};

static WshShellHistory_t Shell_HistoryStorage;

static WshShellHistory_t Shell_HistoryRead(void) {
    return Shell_HistoryStorage;
}

static void Shell_HistoryWrite(WshShellHistory_t history) {
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

bool Shell_Init(const char* pcHostName) {
    if (WshShell_Init(&Shell, pcHostName, NULL, &Shell_Callbacks) != WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    if (WshShellUser_Attach(&(Shell.Users), Shell_UserTable, WSH_SHELL_ARR_LEN(Shell_UserTable)) !=
        WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    WshShellHistory_Init(&Shell.HistoryIO, Shell_HistoryRead, Shell_HistoryWrite);

    if (!Shell_Commands_Init(&Shell))
        return false;

    // WshShell_Auth(&Shell, Shell_UserTable[0].Login, Shell_UserTable[0].Pass);  //For quick auth

    return true;
}

void Shell_SendChar(char ch) {
    WshShell_InsertChar(&Shell, ch);
}
