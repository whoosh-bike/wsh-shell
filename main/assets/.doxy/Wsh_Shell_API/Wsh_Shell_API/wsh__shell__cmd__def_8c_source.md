

# File wsh\_shell\_cmd\_def.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd\_def.c**](wsh__shell__cmd__def_8c.md)

[Go to the documentation of this file](wsh__shell__cmd__def_8c.md)


```C++
#include "wsh_shell_cmd_def.h"
#include "wsh_shell.h"

#if WSH_SHELL_DEF_COMMAND

#define X_ENTRY(name, value) value,
static const WSH_SHELL_CMD_GROUP_t WshShell_CmdGroups[] = {WSH_SHELL_CMD_GROUP_LIST};
#undef X_ENTRY

#define WSH_SHELL_CMD_GROUP_COUNT (WSH_SHELL_ARR_LEN(WshShell_CmdGroups))

/*
 * Flat fallback flags injected when WSH_SHELL_SUBCOMMANDS=0.
 * With subcommands on, these are superseded by the subcommand tree.
 */
#if WSH_SHELL_SUBCOMMANDS
#define WSH_SHELL_CMD_DEF_OPT_USER_SLOT()
#define WSH_SHELL_CMD_DEF_OPT_HIST_SLOT()
#else
#define WSH_SHELL_CMD_DEF_OPT_USER_SLOT() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_USER,   \
                WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-u", "--user", "Get info about users"))
#define WSH_SHELL_CMD_DEF_OPT_HIST_SLOT()                                                                        \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HISTPRINT,                                                                     \
                WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-g", "--histprint", "Print command history")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HISTRST,                                                                       \
                WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_WRITE, "-r", "--histrst", "Clear command history"))
#endif

/* clang-format off */
#define WSH_SHELL_CMD_DEF_OPT_TABLE() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Print shell status (version, device, user)")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_EXEC,   WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ,    "-x", "--exec",   "List accessible commands")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_CLS,    WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_EXECUTE, "-c", "--cls",    "Clear screen")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_PING,   WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY,     "-p", "--ping",   "Ping shell")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEAUTH, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY,     "-d", "--deauth", "Logout and clear history")) \
    WSH_SHELL_CMD_DEF_OPT_USER_SLOT() \
    WSH_SHELL_CMD_DEF_OPT_HIST_SLOT() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_END,    WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_DEF_OPT_TABLE() WSH_SHELL_CMD_DEF_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} WSH_SHELL_DEF_OPT_t;

#define X_CMD_ENTRY(enum, opt) {enum, opt},
static const WshShellOption_t WshShell_OptArr[] = {WSH_SHELL_CMD_DEF_OPT_TABLE()};
#undef X_CMD_ENTRY

/* Shared helpers — used by the -u flat flag (no subcommands) and by wsh user
 * subcommand handlers (subcommands enabled). Defined unconditionally so both
 * paths can call them without duplication. */
static void WshShellDef_PrintUserRow(const WshShellUser_t* pcUser, const WshShell_Char_t* pRowTemplate) {
    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
    WshShellStr_GroupBitsToStr(pcUser->Groups, WSH_SHELL_CMD_GROUP_COUNT, groupRow);

    WshShell_Char_t rightsRow[5];
    WshShellStr_AccessBitsToStr(pcUser->Rights, rightsRow);

    WSH_SHELL_PRINT(pRowTemplate, pcUser->Login, groupRow, rightsRow);
}

static void WshShellDef_PrintUserHead(WshShell_Char_t* pRowTemplate, WshShell_Size_t rowTemplateLen) {
    const WshShell_Size_t loginMaxLen  = WSH_SHELL_LOGIN_LEN;
    const WshShell_Size_t groupMaxLen  = WSH_SHELL_CMD_GROUP_COUNT + 3;
    const WshShell_Size_t rightsMaxLen = 6;

    WshShell_Char_t headTemplate[64];
    WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                       WSH_SHELL_COLOR_SYS "  %%-%ds %%-%ds %%-%ds\r\n" WSH_SHELL_ESC_RESET_STYLE, loginMaxLen,
                       groupMaxLen, rightsMaxLen);

    WSH_SHELL_PRINT(headTemplate, "Login", "Groups", "Rights");

    WSH_SHELL_SNPRINTF(pRowTemplate, rowTemplateLen, "  %%-%ds %%-%ds %%-%ds\r\n", loginMaxLen, groupMaxLen,
                       rightsMaxLen);
}

#if WSH_SHELL_SUBCOMMANDS

static const WshShell_Char_t* const WshShellDefUserListFmtVals[] = {"table", "short"};
static const WshShellOptionEnum_t WshShellDefUserListFmt         = {
    WshShellDefUserListFmtVals,
    WSH_SHELL_ARR_LEN(WshShellDefUserListFmtVals),
};

/* clang-format off */
#define WSH_SHELL_CMD_USER_LIST_OPT_TABLE() \
    X_CMD_ENTRY(USER_LIST_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_READ, "Print registered users")) \
    X_CMD_ENTRY(USER_LIST_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(USER_LIST_OPT_FORMAT, WSH_SHELL_OPT_ENUM(WSH_SHELL_OPT_ACCESS_READ, "-f", "--format", &WshShellDefUserListFmt, "Output format (table, short)")) \
    X_CMD_ENTRY(USER_LIST_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_USER_LIST_OPT_TABLE() USER_LIST_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} USER_LIST_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefUserListOptArr[] = {WSH_SHELL_CMD_USER_LIST_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_UserList(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                     const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell  = (WshShell_t*)pShellCtx;
    WshShell_Bool_t shortForm = false;
    WshShell_Bool_t helpOnly  = false;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }

        switch (optCtx.Option->ID) {
            case USER_LIST_OPT_HELP:
                helpOnly = true;
                break;
            case USER_LIST_OPT_FORMAT: {
                WshShell_Char_t fmtBuf[WSH_SHELL_ENUM_VALUE_MAX_LEN] = {0};
                if (WshShellCmd_GetOptValue(&optCtx, argc, pArgv, sizeof(fmtBuf), fmtBuf) ==
                    WSH_SHELL_RET_STATE_SUCCESS) {
                    if (WSH_SHELL_STRCMP(fmtBuf, "short") == 0)
                        shortForm = true;
                }
            } break;
            case USER_LIST_OPT_DEF:
            default:
                break;
        }
    }

    if (helpOnly) {
        WshShellCmd_PrintOptionsOverview(pcCmd);
        return WSH_SHELL_RET_STATE_SUCCESS;
    }

    const WshShell_Size_t usersNum = WshShellUser_GetUsersNum(&(pParentShell->Users));

    if (shortForm) {
        for (WshShell_Size_t user = 0; user < usersNum; user++) {
            const WshShellUser_t* pcUser = WshShellUser_GetUserByIndex(&(pParentShell->Users), user);
            if (pcUser)
                WSH_SHELL_PRINT("%s\r\n", pcUser->Login);
        }
        return WSH_SHELL_RET_STATE_SUCCESS;
    }

    WSH_SHELL_PRINT("Registered users:\r\n");

    WshShell_Char_t rowTemplate[64];
    WshShellDef_PrintUserHead(rowTemplate, sizeof(rowTemplate));

    for (WshShell_Size_t user = 0; user < usersNum; user++) {
        const WshShellUser_t* pcUser = WshShellUser_GetUserByIndex(&(pParentShell->Users), user);
        if (pcUser)
            WshShellDef_PrintUserRow(pcUser, rowTemplate);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefUserListCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "list",
    .Descr   = "List all registered users",
    .Options = WshShellDefUserListOptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShellDefUserListOptArr),
    .Handler = WshShellCmdDef_UserList,
};

/* clang-format off */
#define WSH_SHELL_CMD_USER_WHOAMI_OPT_TABLE() \
    X_CMD_ENTRY(USER_WHOAMI_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_READ, "Print full current session row")) \
    X_CMD_ENTRY(USER_WHOAMI_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(USER_WHOAMI_OPT_NAME,   WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-n", "--name",   "Print only the session login")) \
    X_CMD_ENTRY(USER_WHOAMI_OPT_GROUPS, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-g", "--groups", "Print only the session groups")) \
    X_CMD_ENTRY(USER_WHOAMI_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_USER_WHOAMI_OPT_TABLE() USER_WHOAMI_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} USER_WHOAMI_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefUserWhoamiOptArr[] = {WSH_SHELL_CMD_USER_WHOAMI_OPT_TABLE()};
#undef X_CMD_ENTRY

#define USER_WHOAMI_FIELD_NAME   (1u << 0)
#define USER_WHOAMI_FIELD_GROUPS (1u << 1)

static WSH_SHELL_RET_STATE_t WshShellCmdDef_UserWhoami(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                       const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell  = (WshShell_t*)pShellCtx;
    WshShell_Size_t fieldMask = 0;
    WshShell_Bool_t helpOnly  = false;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }

        switch (optCtx.Option->ID) {
            case USER_WHOAMI_OPT_HELP:
                helpOnly = true;
                break;
            case USER_WHOAMI_OPT_NAME:
                fieldMask |= USER_WHOAMI_FIELD_NAME;
                break;
            case USER_WHOAMI_OPT_GROUPS:
                fieldMask |= USER_WHOAMI_FIELD_GROUPS;
                break;
            case USER_WHOAMI_OPT_DEF:
            default:
                break;
        }
    }

    if (helpOnly) {
        WshShellCmd_PrintOptionsOverview(pcCmd);
        return WSH_SHELL_RET_STATE_SUCCESS;
    }

    if (pParentShell->CurrUser == NULL) {
        WSH_SHELL_PRINT_ERR("Not authenticated\r\n");
        return WSH_SHELL_RET_STATE_ERROR;
    }

    if (fieldMask == 0) {
        WSH_SHELL_PRINT("Current session:\r\n");
        WshShell_Char_t rowTemplate[64];
        WshShellDef_PrintUserHead(rowTemplate, sizeof(rowTemplate));
        WshShellDef_PrintUserRow(pParentShell->CurrUser, rowTemplate);
        return WSH_SHELL_RET_STATE_SUCCESS;
    }

    if (fieldMask & USER_WHOAMI_FIELD_NAME)
        WSH_SHELL_PRINT("login: %s\r\n", pParentShell->CurrUser->Login);

    if (fieldMask & USER_WHOAMI_FIELD_GROUPS) {
        WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
        WshShellStr_GroupBitsToStr(pParentShell->CurrUser->Groups, WSH_SHELL_CMD_GROUP_COUNT, groupRow);
        WSH_SHELL_PRINT("groups: %s\r\n", groupRow);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefUserWhoamiCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "whoami",
    .Descr   = "Print current session user info",
    .Options = WshShellDefUserWhoamiOptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShellDefUserWhoamiOptArr),
    .Handler = WshShellCmdDef_UserWhoami,
};

static const WshShellCmd_t* const WshShellDefUserSubCmds[] = {
    &WshShellDefUserListCmd,
    &WshShellDefUserWhoamiCmd,
};

/* clang-format off */
#define WSH_SHELL_CMD_USER_OPT_TABLE() \
    X_CMD_ENTRY(USER_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "User management subcommands")) \
    X_CMD_ENTRY(USER_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(USER_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_USER_OPT_TABLE() USER_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} USER_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefUserOptArr[] = {WSH_SHELL_CMD_USER_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_User(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                 const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        WshShellCmd_PrintOptionsOverview(pcCmd);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefUserCmd = {
    .Groups    = WSH_SHELL_CMD_GROUP_ALL,
    .Name      = "user",
    .Descr     = "User table and session queries",
    .Options   = WshShellDefUserOptArr,
    .OptNum    = WSH_SHELL_ARR_LEN(WshShellDefUserOptArr),
    .Handler   = WshShellCmdDef_User,
    .SubCmds   = WshShellDefUserSubCmds,
    .SubCmdNum = WSH_SHELL_ARR_LEN(WshShellDefUserSubCmds),
};

/* clang-format off */
#define WSH_SHELL_CMD_HIST_LIST_OPT_TABLE() \
    X_CMD_ENTRY(HIST_LIST_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_READ, "Print command history")) \
    X_CMD_ENTRY(HIST_LIST_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(HIST_LIST_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_HIST_LIST_OPT_TABLE() HIST_LIST_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} HIST_LIST_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefHistListOptArr[] = {WSH_SHELL_CMD_HIST_LIST_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_HistList(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                     const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        if (optCtx.Option->ID == HIST_LIST_OPT_HELP) {
            WshShellCmd_PrintOptionsOverview(pcCmd);
            return WSH_SHELL_RET_STATE_SUCCESS;
        }
    }

    WshShell_Char_t cmdBuff[WSH_SHELL_INTR_BUFF_LEN];
    WshShell_Size_t cmdNum = WshShellHistory_GetTokenNum(&(pParentShell->HistoryIO));

    WSH_SHELL_PRINT("History (%d):\r\n", cmdNum);
    for (WshShell_Size_t i = cmdNum; i > 0; i--) {
        if (WshShellHistory_GetTokenByIndex(&(pParentShell->HistoryIO), cmdBuff, sizeof(cmdBuff), i - 1))
            WSH_SHELL_PRINT("  [%2d] %s\r\n", cmdNum - i + 1, cmdBuff);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefHistListCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "list",
    .Descr   = "Print command history",
    .Options = WshShellDefHistListOptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShellDefHistListOptArr),
    .Handler = WshShellCmdDef_HistList,
};

/* clang-format off */
#define WSH_SHELL_CMD_HIST_CLEAR_OPT_TABLE() \
    X_CMD_ENTRY(HIST_CLEAR_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_WRITE, "Clear command history")) \
    X_CMD_ENTRY(HIST_CLEAR_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(HIST_CLEAR_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_HIST_CLEAR_OPT_TABLE() HIST_CLEAR_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} HIST_CLEAR_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefHistClearOptArr[] = {WSH_SHELL_CMD_HIST_CLEAR_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_HistClear(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                      const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        if (optCtx.Option->ID == HIST_CLEAR_OPT_HELP) {
            WshShellCmd_PrintOptionsOverview(pcCmd);
            return WSH_SHELL_RET_STATE_SUCCESS;
        }
    }

    WshShellHistory_Flush(&(pParentShell->HistoryIO));
    WSH_SHELL_PRINT_INFO("History cleared\r\n");
    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefHistClearCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "clear",
    .Descr   = "Clear command history",
    .Options = WshShellDefHistClearOptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShellDefHistClearOptArr),
    .Handler = WshShellCmdDef_HistClear,
};

static const WshShellCmd_t* const WshShellDefHistSubCmds[] = {
    &WshShellDefHistListCmd,
    &WshShellDefHistClearCmd,
};

/* clang-format off */
#define WSH_SHELL_CMD_HIST_OPT_TABLE() \
    X_CMD_ENTRY(HIST_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "History subcommands")) \
    X_CMD_ENTRY(HIST_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(HIST_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_HIST_OPT_TABLE() HIST_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} HIST_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefHistOptArr[] = {WSH_SHELL_CMD_HIST_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_History(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                    const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        WshShellCmd_PrintOptionsOverview(pcCmd);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefHistCmd = {
    .Groups    = WSH_SHELL_CMD_GROUP_ALL,
    .Name      = "history",
    .Descr     = "Command history management",
    .Options   = WshShellDefHistOptArr,
    .OptNum    = WSH_SHELL_ARR_LEN(WshShellDefHistOptArr),
    .Handler   = WshShellCmdDef_History,
    .SubCmds   = WshShellDefHistSubCmds,
    .SubCmdNum = WSH_SHELL_ARR_LEN(WshShellDefHistSubCmds),
};

#if WSH_SHELL_INTERACTIVE_MODE

static void WshShellCmdDef_TokenizeCallback(WshShellIO_CommandLine_t* pInter) {
    WshShellInteract_AppendLineBreak(pInter);

    /* ParseToArgcArgv modifies the string in-place — work on a copy.
     * Strip trailing \r\n so they don't leak into the last token. */
    WshShell_Char_t lineCopy[WSH_SHELL_INTR_BUFF_LEN];
    WSH_SHELL_STRNCPY(lineCopy, pInter->Buff, WSH_SHELL_INTR_BUFF_LEN - 1);
    lineCopy[WSH_SHELL_INTR_BUFF_LEN - 1] = '\0';
    WshShell_Size_t copyLen               = WSH_SHELL_STRLEN(lineCopy);
    while (copyLen > 0 && (lineCopy[copyLen - 1] == '\r' || lineCopy[copyLen - 1] == '\n'))
        lineCopy[--copyLen] = '\0';

    const WshShell_Char_t* argv[WSH_SHELL_CMD_ARGS_MAX_NUM];
    WshShell_Size_t argc = 0;
    WshShellStr_ParseToArgcArgv(lineCopy, &argc, argv, WSH_SHELL_CMD_ARGS_MAX_NUM);

    if (argc == 0) {
        WSH_SHELL_PRINT("  (empty)\r\n");
        return;
    }

    for (WshShell_Size_t i = 0; i < argc; i++)
        WSH_SHELL_PRINT("  [%d] \"%s\"\r\n", i, argv[i]);
}

/* clang-format off */
#define WSH_SHELL_CMD_TOKENIZE_OPT_TABLE() \
    X_CMD_ENTRY(TOKENIZE_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Split input into tokens (interactive)")) \
    X_CMD_ENTRY(TOKENIZE_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(TOKENIZE_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_TOKENIZE_OPT_TABLE() TOKENIZE_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} TOKENIZE_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t WshShellDefTokenizeOptArr[] = {WSH_SHELL_CMD_TOKENIZE_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t WshShellCmdDef_Tokenize(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                     const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell = (WshShell_t*)pShellCtx;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }
        if (optCtx.Option->ID == TOKENIZE_OPT_HELP) {
            WshShellCmd_PrintOptionsOverview(pcCmd);
            return WSH_SHELL_RET_STATE_SUCCESS;
        }
        WshShellInteract_Attach(&(pParentShell->Interact), pcCmd->Name, WshShellCmdDef_TokenizeCallback);
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t WshShellDefTokenizeCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "tokenize",
    .Descr   = "Split each input line into tokens (interactive mode)",
    .Options = WshShellDefTokenizeOptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShellDefTokenizeOptArr),
    .Handler = WshShellCmdDef_Tokenize,
};

#endif /* WSH_SHELL_INTERACTIVE_MODE */

static const WshShellCmd_t* const WshShellDefSubCmds[] = {
    &WshShellDefUserCmd,
    &WshShellDefHistCmd,
#if WSH_SHELL_INTERACTIVE_MODE
    &WshShellDefTokenizeCmd,
#endif
};

#endif /* WSH_SHELL_SUBCOMMANDS */

static WSH_SHELL_RET_STATE_t WshShellCmdDef(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                            const WshShell_Char_t* pArgv[], void* pShellCtx) {
    if ((argc > 0 && !pArgv) || !pcCmd)
        return WSH_SHELL_RET_STATE_ERROR;

    WshShell_t* pParentShell       = (WshShell_t*)pShellCtx;
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option) {
            if (optCtx.ParseError)
                retState = WSH_SHELL_RET_STATE_ERR_PARAM;
            break;
        }

        switch (optCtx.Option->ID) {
            case WSH_SHELL_DEF_OPT_DEF: {
                WSH_SHELL_PRINT("Ver: %s\r\n", pParentShell->Version);
                WSH_SHELL_PRINT("Device name: %s\r\n", pParentShell->DeviceName);
                WSH_SHELL_PRINT("User: %s\r\n", pParentShell->CurrUser->Login);
            } break;

            case WSH_SHELL_DEF_OPT_HELP: {
                WshShellCmd_PrintOptionsOverview(pcCmd);
            } break;

            case WSH_SHELL_DEF_OPT_EXEC: {
                WSH_SHELL_PRINT("Availible commands:\r\n");

                const WshShell_Size_t cmdMaxLen   = WSH_SHELL_CMD_NAME_LEN;
                const WshShell_Size_t optMaxLen   = 5;
                const WshShell_Size_t groupMaxLen = WSH_SHELL_CMD_GROUP_COUNT + 3;

                WshShell_Char_t headTemplate[64];
                WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                                   WSH_SHELL_COLOR_SYS "  %%-%ds %%-%ds %%-%ds %%s\r\n" WSH_SHELL_ESC_RESET_STYLE,
                                   cmdMaxLen, optMaxLen, groupMaxLen);

                WSH_SHELL_PRINT(headTemplate, "Command", "Opts", "Groups", "Descr");

                WshShell_Char_t rowTemplate[64];
                WSH_SHELL_SNPRINTF(rowTemplate, sizeof(rowTemplate), "  %%-%ds %%-%dd %%-%ds %%s\r\n", cmdMaxLen,
                                   optMaxLen, groupMaxLen);

                const WshShellCmd_t* pcDefCmd = WshShellDefCmd_GetPtr();
                if (pcDefCmd != NULL) {
                    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
                    WshShellStr_GroupBitsToStr(pcDefCmd->Groups, WSH_SHELL_CMD_GROUP_COUNT, groupRow);
                    WSH_SHELL_PRINT(rowTemplate, pcDefCmd->Name, pcDefCmd->OptNum - 1, groupRow, pcDefCmd->Descr);
                }

                WshShell_Size_t commandsNum = WshShellCmd_GetCmdNum(&(pParentShell->Commands));
                for (WshShell_Size_t cmd = 0; cmd < commandsNum; cmd++) {
                    const WshShellCmd_t* pcTargetCmd = WshShellCmd_GetCmdByIndex(&(pParentShell->Commands), cmd);

                    if (pcTargetCmd == NULL)
                        continue;

                    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
                    WshShellStr_GroupBitsToStr(pcTargetCmd->Groups, WSH_SHELL_CMD_GROUP_COUNT, groupRow);

                    WSH_SHELL_PRINT(rowTemplate, pcTargetCmd->Name, pcTargetCmd->OptNum - 1, groupRow,
                                    pcTargetCmd->Descr);
                }
            } break;

#if !WSH_SHELL_SUBCOMMANDS
            case WSH_SHELL_DEF_OPT_USER: {
                WSH_SHELL_PRINT("Registered users:\r\n");
                WshShell_Char_t rowTemplate[64];
                WshShellDef_PrintUserHead(rowTemplate, sizeof(rowTemplate));
                const WshShell_Size_t usersNum = WshShellUser_GetUsersNum(&(pParentShell->Users));
                for (WshShell_Size_t user = 0; user < usersNum; user++) {
                    const WshShellUser_t* pcUser = WshShellUser_GetUserByIndex(&(pParentShell->Users), user);
                    if (pcUser)
                        WshShellDef_PrintUserRow(pcUser, rowTemplate);
                }
            } break;

            case WSH_SHELL_DEF_OPT_HISTPRINT: {
                WshShell_Size_t cmdNum = WshShellHistory_GetTokenNum(&(pParentShell->HistoryIO));
                WshShell_Char_t cmdBuff[WSH_SHELL_INTR_BUFF_LEN];
                for (WshShell_Size_t i = cmdNum; i > 0; i--) {
                    if (WshShellHistory_GetTokenByIndex(&(pParentShell->HistoryIO), cmdBuff, sizeof(cmdBuff), i - 1))
                        WSH_SHELL_PRINT("  [%d] %s\r\n", cmdNum - i + 1, cmdBuff);
                }
            } break;

            case WSH_SHELL_DEF_OPT_HISTRST: {
                WshShellHistory_Flush(&(pParentShell->HistoryIO));
                WSH_SHELL_PRINT_SYS("History cleared\r\n");
            } break;
#endif /* !WSH_SHELL_SUBCOMMANDS */

            case WSH_SHELL_DEF_OPT_CLS: {
                WSH_SHELL_PRINT(WSH_SHELL_ECS_CLR_SCREEN);
            } break;

            case WSH_SHELL_DEF_OPT_PING: {
                WSH_SHELL_PRINT("pong\r\n");
            } break;

            case WSH_SHELL_DEF_OPT_DEAUTH: {
                WshShell_DeAuth(pParentShell, "command");
            } break;

            default: {
                retState = WSH_SHELL_RET_STATE_ERROR;
            } break;
        }
    }

    return retState;
}

static const WshShellCmd_t WshShellDefCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "wsh",
    .Descr   = "Default command for configuring and querying the shell interface",
    .Options = WshShell_OptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShell_OptArr),
    .Handler = WshShellCmdDef,
#if WSH_SHELL_SUBCOMMANDS
    .SubCmds   = WshShellDefSubCmds,
    .SubCmdNum = WSH_SHELL_ARR_LEN(WshShellDefSubCmds),
#endif
};

#else /* WSH_SHELL_DEF_COMMAND */

WSH_SHELL_RET_STATE_t WshShellCmdDef_Dummy(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                           const WshShell_Char_t* pArgv[], void* pShellCtx) {
    return WSH_SHELL_RET_STATE_ERR_EMPTY;
}

static const WshShellCmd_t WshShellDefCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "wsh",
    .Descr   = "",
    .Options = NULL,
    .OptNum  = 0,
    .Handler = WshShellCmdDef_Dummy,
};

#endif /* WSH_SHELL_DEF_COMMAND */

const WshShellCmd_t* WshShellDefCmd_GetPtr(void) {
    return &WshShellDefCmd;
}
```


