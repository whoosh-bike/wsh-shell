#include "wsh_shell_cmd_def.h"
#include "wsh_shell.h"

#if WSH_SHELL_DEF_COMMAND

#define X_ENTRY(name, value) value,
static const WSH_SHELL_CMD_GROUP_t WshShell_CmdGroups[] = {WSH_SHELL_CMD_GROUP_LIST};
#undef X_ENTRY

#define WSH_SHELL_CMD_GROUP_COUNT (WSH_SHELL_ARR_LEN(WshShell_CmdGroups))

/**
 * @def WSH_SHELL_CMD_DEF_OPT_TABLE
 * @brief Macro defining the default shell command options using X-Macros.
 * 
 * This macro expands into a list of `X_CMD_ENTRY()` calls, each representing 
 * an available option for the default shell command. Used for both option 
 * structure definitions and enumeration.
 */
/*
 * When subcommands are enabled, `wsh user list` supersedes the flat -u flag.
 * When subcommands are disabled, inject -u back so the user table is still
 * reachable. A slot macro keeps the X-macro table structure clean.
 */
#if WSH_SHELL_SUBCOMMANDS
#define WSH_SHELL_CMD_DEF_OPT_USER_SLOT()
#else
#define WSH_SHELL_CMD_DEF_OPT_USER_SLOT()                                                       \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_USER, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-u", \
                                                               "--user", "Get info about users"))
#endif

/* clang-format off */
#define WSH_SHELL_CMD_DEF_OPT_TABLE() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Print basic info about shell instance")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HELP, WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_INTERACT, WSH_SHELL_OPT_INTERACT(WSH_SHELL_OPT_ACCESS_ANY)) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_EXEC, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-x", "--exec", "Get info about accessible commands")) \
    WSH_SHELL_CMD_DEF_OPT_USER_SLOT() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_CLS, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_EXECUTE, "-c", "--cls", "Clear screen")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_CLEAR, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_WRITE, "-r", "--histrst", "Reset history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_PRINT, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-g", "--histprint", "Print history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEAUTH, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-d", "--deauth", "DeAuth and destroy history")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_PING, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-p", "--ping", "Ping shell")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_END, WSH_SHELL_OPT_END())
/* clang-format on */

/**
 * @enum WSH_SHELL_DEF_OPT_t
 * @brief Enumeration of supported options for the default shell command.
 *
 * Each value corresponds to an entry from the WSH_SHELL_CMD_DEF_OPT_TABLE.
 */
typedef enum {
#define X_CMD_ENTRY(en, m) en,
    WSH_SHELL_CMD_DEF_OPT_TABLE() WSH_SHELL_CMD_DEF_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} WSH_SHELL_DEF_OPT_t;

#define X_CMD_ENTRY(enum, opt) {enum, opt},
static const WshShellOption_t WshShell_OptArr[] = {WSH_SHELL_CMD_DEF_OPT_TABLE()};
#undef X_CMD_ENTRY

static void WshShell_CmdDefInteractive(WshShellIO_CommandLine_t* pInter) {
    WshShellInteract_AppendLineBreak(pInter);

    WSH_SHELL_PRINT("Just echo of interactive command: %s", pInter->Buff);
}

/* Shared helpers — used by the -u flat flag (no subcommands) and by wsh user
 * subcommand handlers (subcommands enabled). Defined unconditionally so both
 * paths can call them without duplication. */
static void WshShellDef_PrintUserRow(const WshShellUser_t* pcUser,
                                     const WshShell_Char_t* pRowTemplate) {
    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
    WshShellStr_GroupBitsToStr(pcUser->Groups, WSH_SHELL_CMD_GROUP_COUNT, groupRow);

    WshShell_Char_t rightsRow[5];
    WshShellStr_AccessBitsToStr(pcUser->Rights, rightsRow);

    WSH_SHELL_PRINT(pRowTemplate, pcUser->Login, groupRow, rightsRow);
}

static void WshShellDef_PrintUserHead(WshShell_Char_t* pRowTemplate,
                                      WshShell_Size_t rowTemplateLen) {
    const WshShell_Size_t loginMaxLen  = WSH_SHELL_LOGIN_LEN;
    const WshShell_Size_t groupMaxLen  = WSH_SHELL_CMD_GROUP_COUNT + 3;
    const WshShell_Size_t rightsMaxLen = 6;

    WshShell_Char_t headTemplate[64];
    WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                       WSH_SHELL_COLOR_SYS "  %%-%ds %%-%ds %%-%ds\r\n" WSH_SHELL_ESC_RESET_STYLE,
                       loginMaxLen, groupMaxLen, rightsMaxLen);

    WSH_SHELL_PRINT(headTemplate, "Login", "Groups", "Rights");

    WSH_SHELL_SNPRINTF(pRowTemplate, rowTemplateLen, "  %%-%ds %%-%ds %%-%ds\r\n", loginMaxLen,
                       groupMaxLen, rightsMaxLen);
}

#if WSH_SHELL_SUBCOMMANDS

/* clang-format off */
#define WSH_SHELL_CMD_USER_LIST_OPT_TABLE() \
    X_CMD_ENTRY(USER_LIST_OPT_DEF,   WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_READ, "Print registered users as a table")) \
    X_CMD_ENTRY(USER_LIST_OPT_HELP,  WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(USER_LIST_OPT_SHORT, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-s", "--short", "Print logins only, one per line")) \
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

static WSH_SHELL_RET_STATE_t WshShellCmdDef_UserList(const WshShellCmd_t* pcCmd,
                                                     WshShell_Size_t argc,
                                                     const WshShell_Char_t* pArgv[],
                                                     void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell  = (WshShell_t*)pShellCtx;
    WshShell_Bool_t shortForm = false;
    WshShell_Bool_t helpOnly  = false;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option)
            break;

        switch (optCtx.Option->ID) {
            case USER_LIST_OPT_HELP:
                helpOnly = true;
                break;
            case USER_LIST_OPT_SHORT:
                shortForm = true;
                break;
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
            const WshShellUser_t* pcUser =
                WshShellUser_GetUserByIndex(&(pParentShell->Users), user);
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
static const WshShellOption_t WshShellDefUserWhoamiOptArr[] = {
    WSH_SHELL_CMD_USER_WHOAMI_OPT_TABLE()};
#undef X_CMD_ENTRY

#define USER_WHOAMI_FIELD_NAME   (1u << 0)
#define USER_WHOAMI_FIELD_GROUPS (1u << 1)

static WSH_SHELL_RET_STATE_t WshShellCmdDef_UserWhoami(const WshShellCmd_t* pcCmd,
                                                       WshShell_Size_t argc,
                                                       const WshShell_Char_t* pArgv[],
                                                       void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pParentShell  = (WshShell_t*)pShellCtx;
    WshShell_Size_t fieldMask = 0;
    WshShell_Bool_t helpOnly  = false;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pParentShell->CurrUser->Rights, &tokenPos);
        if (!optCtx.Option)
            break;

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
        WshShellStr_GroupBitsToStr(pParentShell->CurrUser->Groups, WSH_SHELL_CMD_GROUP_COUNT,
                                   groupRow);
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
    X_CMD_ENTRY(USER_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Print user subcommand overview")) \
    X_CMD_ENTRY(USER_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(USER_OPT_COUNT,  WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-c", "--count", "Print total number of registered users")) \
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
        if (!optCtx.Option)
            break;

        switch (optCtx.Option->ID) {
            case USER_OPT_COUNT:
                WSH_SHELL_PRINT("Users: %d\r\n",
                                (int)WshShellUser_GetUsersNum(&(pParentShell->Users)));
                break;
            case USER_OPT_DEF:
            case USER_OPT_HELP:
            default:
                WshShellCmd_PrintOptionsOverview(pcCmd);
                break;
        }
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

static const WshShellCmd_t* const WshShellDefSubCmds[] = {
    &WshShellDefUserCmd,
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
        if (!optCtx.Option)
            break;

        switch (optCtx.Option->ID) {
            case WSH_SHELL_DEF_OPT_DEF: {
                WSH_SHELL_PRINT("Ver: %s\r\n", pParentShell->Version);
                WSH_SHELL_PRINT("Device name: %s\r\n", pParentShell->DeviceName);
                WSH_SHELL_PRINT("User: %s\r\n", pParentShell->CurrUser->Login);
            } break;

            case WSH_SHELL_DEF_OPT_HELP: {
                WshShellCmd_PrintOptionsOverview(pcCmd);
            } break;

            case WSH_SHELL_DEF_OPT_INTERACT: {
                WshShellInteract_Attach(&(pParentShell->Interact), pcCmd->Name,
                                        WshShell_CmdDefInteractive);
            } break;

            case WSH_SHELL_DEF_OPT_EXEC: {
                WSH_SHELL_PRINT("Availible commands:\r\n");

                const WshShell_Size_t cmdMaxLen   = WSH_SHELL_CMD_NAME_LEN;
                const WshShell_Size_t optMaxLen   = 5;
                const WshShell_Size_t groupMaxLen = WSH_SHELL_CMD_GROUP_COUNT + 3;

                WshShell_Char_t headTemplate[64];
                WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                                   WSH_SHELL_COLOR_SYS
                                   "  %%-%ds %%-%ds %%-%ds %%s\r\n" WSH_SHELL_ESC_RESET_STYLE,
                                   cmdMaxLen, optMaxLen, groupMaxLen);

                WSH_SHELL_PRINT(headTemplate, "Command", "Opts", "Groups", "Descr");

                WshShell_Char_t rowTemplate[64];
                WSH_SHELL_SNPRINTF(rowTemplate, sizeof(rowTemplate),
                                   "  %%-%ds %%-%dd %%-%ds %%s\r\n", cmdMaxLen, optMaxLen,
                                   groupMaxLen);

                const WshShellCmd_t* pcDefCmd = WshShellDefCmd_GetPtr();
                if (pcDefCmd != NULL) {
                    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
                    WshShellStr_GroupBitsToStr(pcDefCmd->Groups, WSH_SHELL_CMD_GROUP_COUNT,
                                               groupRow);
                    WSH_SHELL_PRINT(rowTemplate, pcDefCmd->Name, pcDefCmd->OptNum - 1, groupRow,
                                    pcDefCmd->Descr);
                }

                WshShell_Size_t commandsNum = WshShellCmd_GetCmdNum(&(pParentShell->Commands));
                for (WshShell_Size_t cmd = 0; cmd < commandsNum; cmd++) {
                    const WshShellCmd_t* pcTargetCmd =
                        WshShellCmd_GetCmdByIndex(&(pParentShell->Commands), cmd);

                    if (pcTargetCmd == NULL)
                        continue;

                    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
                    WshShellStr_GroupBitsToStr(pcTargetCmd->Groups, WSH_SHELL_CMD_GROUP_COUNT,
                                               groupRow);

                    WSH_SHELL_PRINT(rowTemplate, pcTargetCmd->Name, pcTargetCmd->OptNum - 1,
                                    groupRow, pcTargetCmd->Descr);
                }
            } break;

#if !WSH_SHELL_SUBCOMMANDS
            case WSH_SHELL_DEF_OPT_USER: {
                WSH_SHELL_PRINT("Registered users:\r\n");
                WshShell_Char_t rowTemplate[64];
                WshShellDef_PrintUserHead(rowTemplate, sizeof(rowTemplate));
                const WshShell_Size_t usersNum = WshShellUser_GetUsersNum(&(pParentShell->Users));
                for (WshShell_Size_t user = 0; user < usersNum; user++) {
                    const WshShellUser_t* pcUser =
                        WshShellUser_GetUserByIndex(&(pParentShell->Users), user);
                    if (pcUser)
                        WshShellDef_PrintUserRow(pcUser, rowTemplate);
                }
            } break;
#endif /* !WSH_SHELL_SUBCOMMANDS */

            case WSH_SHELL_DEF_OPT_CLS: {
                WSH_SHELL_PRINT(WSH_SHELL_ECS_CLR_SCREEN);
            } break;

            case WSH_SHELL_DEF_OPT_HIST_CLEAR: {
                WshShellHistory_Flush(&(pParentShell->HistoryIO));
                WSH_SHELL_PRINT_INFO("History flushed\r\n");
            } break;

            case WSH_SHELL_DEF_OPT_HIST_PRINT: {
                WshShell_Char_t cmdBuff[WSH_SHELL_INTR_BUFF_LEN];
                WshShell_Size_t cmdNum = WshShellHistory_GetTokenNum(&(pParentShell->HistoryIO));

                WSH_SHELL_PRINT("History len: %d\r\n", cmdNum);
                for (WshShell_Size_t cmdIdx = cmdNum; cmdIdx > 0; cmdIdx--) {
                    if (WshShellHistory_GetTokenByIndex(&(pParentShell->HistoryIO), cmdBuff,
                                                        sizeof(cmdBuff), cmdIdx - 1)) {
                        WSH_SHELL_PRINT("  hist[%2d]: %s\r\n", cmdNum - cmdIdx + 1, cmdBuff);
                    }
                }
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
