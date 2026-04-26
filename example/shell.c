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

/* ── hex-dump command ──────────────────────────────────────────────────── */

/* clang-format off */
#define DUMP_OPT_TABLE() \
    X_CMD_ENTRY(DUMP_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Hex dump of a sample buffer")) \
    X_CMD_ENTRY(DUMP_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(DUMP_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    DUMP_OPT_TABLE() DUMP_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} DUMP_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t Dump_OptArr[] = {DUMP_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t Shell_DumpHandler(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                               const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx = WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        if (optCtx.Option->ID == DUMP_OPT_HELP) {
            WshShellCmd_PrintOptionsOverview(pcCmd);
            return WSH_SHELL_RET_STATE_SUCCESS;
        }
    }

    /* Dump the live PS1 buffer — shows ANSI escape codes alongside text */
    WshShellMisc_HexDump((const WshShell_U8_t*)pShell->PS1,
                         WSH_SHELL_STRNLEN(pShell->PS1, WSH_SHELL_PS1_MAX_LEN), 0);
    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t Shell_DumpCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "dump",
    .Descr   = "Hex dump of a sample buffer (WshShellMisc_HexDump usage example)",
    .Options = Dump_OptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(Dump_OptArr),
    .Handler = Shell_DumpHandler,
};

static const WshShellCmd_t* Shell_CmdTable[] = {&Shell_DumpCmd};

bool Shell_Init(const char* pcHostName, const char* pcLogin, const char* pcPass) {
    if (WshShell_Init(&Shell, pcHostName, NULL, &Shell_Callbacks) != WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    if (WshShellUser_Attach(&(Shell.Users), Shell_UserTable, WSH_SHELL_ARR_LEN(Shell_UserTable), NULL) !=
        WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    WshShellHistory_Init(&Shell.HistoryIO, WshShellHistory_Read, WshShellHistory_Write);

    WshShellCmd_Attach(&Shell.Commands, Shell_CmdTable, WSH_SHELL_ARR_LEN(Shell_CmdTable));

    if (pcLogin != NULL && pcPass != NULL) {
        WshShell_Auth(&Shell, pcLogin, pcPass);
    }

    return true;
}

void Shell_SendChar(char ch) {
    WshShell_InsertChar(&Shell, ch);
}
