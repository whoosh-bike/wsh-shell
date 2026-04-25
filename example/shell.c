#include "shell.h"

#include "wsh_shell.h"

static WshShell_t Shell = {0};

static const WshShellUser_t Shell_UserTable[] = {
    {
        .Login  = "root",
        .Salt   = "a0523cb065ee08c1",
        .Hash   = "0632cee0",  //1234
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_OPT_ACCESS_ANY,
    },
    {
        .Login  = "mfg",
        .Salt   = "b1734dc176ff19e2",
        .Hash   = "a3c17f42",  //mfg1
        .Groups = WSH_SHELL_CMD_GROUP_LOW_LEVEL | WSH_SHELL_CMD_GROUP_MANUFACT,
        .Rights = WSH_SHELL_OPT_ACCESS_READ | WSH_SHELL_OPT_ACCESS_EXECUTE,
    },
    {
        .Login  = "user",
        .Salt   = "aa89fee902ab12e4",
        .Hash   = "ff69cdab",  //qwer
        .Groups = WSH_SHELL_CMD_GROUP_USER,
        .Rights = WSH_SHELL_OPT_ACCESS_READ,
    },
};

static WshShellHistory_t Shell_HistoryStorage;

static WshShellHistory_t WshShellHistory_Read(void) {
    return Shell_HistoryStorage;
}

static void WshShellHistory_Write(WshShellHistory_t history) {
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

static WshShellExtCallbacks_t Shell_Callbacks = {
    .Auth     = Shell_AuthClbk,
    .DeAuth   = Shell_DeAuthClbk,
    .SymbolIn = Shell_SymInClbk,
};

bool Shell_Init(const char* pcHostName, const char* pcLogin, const char* pcPass) {
    if (WshShell_Init(&Shell, pcHostName, NULL, &Shell_Callbacks) != WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    if (WshShellUser_Attach(&(Shell.Users), Shell_UserTable, WSH_SHELL_ARR_LEN(Shell_UserTable),
                            NULL) != WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    WshShellHistory_Init(&Shell.HistoryIO, WshShellHistory_Read, WshShellHistory_Write);

    if (pcLogin != NULL && pcPass != NULL) {
        WshShell_Auth(&Shell, pcLogin, pcPass);
    }

    return true;
}

void Shell_SendChar(char ch) {
    WshShell_InsertChar(&Shell, ch);
}
