#include "wsh_shell_cmd_def.h"
#include "wsh_shell.h"

#define X_ENTRY(name, value) value,
static const WSH_SHELL_CMD_GROUP_t WshShell_CmdGroups[] = {WSH_SHELL_CMD_GROUP_LIST};
#undef X_ENTRY

#define WSH_SHELL_CMD_GROUP_COUNT (WSH_SHELL_ARR_LEN(WshShell_CmdGroups))

#if WSH_SHELL_DEF_COMMAND

/**
 * @def WSH_SHELL_CMD_DEF_OPT_TABLE
 * @brief Macro defining the default shell command options using X-Macros.
 * 
 * This macro expands into a list of `X_CMD_ENTRY()` calls, each representing 
 * an available option for the default shell command. Used for both option 
 * structure definitions and enumeration.
 */
/* clang-format off */
#define WSH_SHELL_CMD_DEF_OPT_TABLE() \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Print basic info about shell instance")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HELP, WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_INTERACT, WSH_SHELL_OPT_INTERACT(WSH_SHELL_OPT_ACCESS_ANY)) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_EXEC, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-x", "--exec", "Get info about accessible commands")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_USER, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-u", "--user", "Get info about users")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_CLS, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_EXECUTE, "-c", "--cls", "Clear screen")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_CLEAR, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_WRITE, "-r", "--histrst", "Reset history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_PRINT, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-p", "--histprint", "Print history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEAUTH, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-d", "--deauth", "DeAuth and destroy history")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_STR, WSH_SHELL_OPT_STR(WSH_SHELL_OPT_ACCESS_EXECUTE, "-s", "--str", "Set string")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_INT, WSH_SHELL_OPT_INT(WSH_SHELL_OPT_ACCESS_EXECUTE, "-n", "--int", "Set int")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_FLT, WSH_SHELL_OPT_FLOAT(WSH_SHELL_OPT_ACCESS_ADMIN, "-f", "--flt", "Set float")) \
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
            return WSH_SHELL_RET_STATE_ERR_EMPTY;

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
                WSH_SHELL_PRINT("Availible commans:\r\n");

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

            case WSH_SHELL_DEF_OPT_USER: {
                WSH_SHELL_PRINT("Availible users:\r\n");

                const WshShell_Size_t loginMaxLen  = WSH_SHELL_LOGIN_LEN;
                const WshShell_Size_t groupMaxLen  = WSH_SHELL_CMD_GROUP_COUNT + 3;
                const WshShell_Size_t rightsMaxLen = 6;

                WshShell_Char_t headTemplate[64];
                WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                                   WSH_SHELL_COLOR_SYS
                                   "  %%-%ds %%-%ds %%-%ds\r\n" WSH_SHELL_ESC_RESET_STYLE,
                                   loginMaxLen, groupMaxLen, rightsMaxLen);

                WSH_SHELL_PRINT(headTemplate, "Login", "Groups", "Rights");

                WshShell_Char_t rowTemplate[64];
                WSH_SHELL_SNPRINTF(rowTemplate, sizeof(rowTemplate), "  %%-%ds %%-%ds %%-%ds\r\n",
                                   loginMaxLen, groupMaxLen, rightsMaxLen);

                const WshShell_Size_t usersNum = WshShellUser_GetUsersNum(&(pParentShell->Users));
                for (WshShell_Size_t user = 0; user < usersNum; user++) {
                    const WshShellUser_t* pcTargetUser =
                        WshShellUser_GetUserByIndex(&(pParentShell->Users), user);

                    if (pcTargetUser == NULL)
                        continue;

                    WshShell_Char_t groupRow[WSH_SHELL_CMD_GROUP_COUNT + 1];
                    WshShellStr_GroupBitsToStr(pcTargetUser->Groups, WSH_SHELL_CMD_GROUP_COUNT,
                                               groupRow);

                    WshShell_Char_t rightsRow[5];
                    WshShellStr_AccessBitsToStr(pcTargetUser->Rights, rightsRow);

                    WSH_SHELL_PRINT(rowTemplate, pcTargetUser->Login, groupRow, rightsRow);
                }
            } break;

            case WSH_SHELL_DEF_OPT_CLS: {
                WSH_SHELL_PRINT(WSH_SHELL_ECS_CLR_SCREEN);
            } break;

            case WSH_SHELL_DEF_OPT_HIST_CLEAR: {
                WshShellHistory_Flush(&(pParentShell->HistoryIO));
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

            case WSH_SHELL_DEF_OPT_DEAUTH: {
                WshShell_DeAuth(pParentShell, "command");
            } break;

            case WSH_SHELL_DEF_OPT_STR: {
                WshShell_Char_t optStr[WSH_SHELL_INTR_BUFF_LEN];
                WshShellCmd_GetOptValue(&optCtx, argc, pArgv, sizeof(optStr),
                                        (WshShell_Size_t*)optStr);
                WSH_SHELL_PRINT("Option (str): %s\r\n", optStr);
                break;
            }

            case WSH_SHELL_DEF_OPT_INT: {
                WshShell_Size_t optInt = 0;
                WshShellCmd_GetOptValue(&optCtx, argc, pArgv, sizeof(optInt),
                                        (WshShell_Size_t*)&optInt);
                WSH_SHELL_PRINT("Option (int): %d\r\n", optInt);
                break;
            }

            case WSH_SHELL_DEF_OPT_FLT: {
                WshShell_Float_t optFlt = 0.0f;
                WshShellCmd_GetOptValue(&optCtx, argc, pArgv, sizeof(optFlt),
                                        (WshShell_Size_t*)&optFlt);
                WSH_SHELL_PRINT("Option (float): %f\r\n", optFlt);
                break;
            }

            default: {
                retState = WSH_SHELL_RET_STATE_ERROR;
            } break;
        }
    }

    return retState;
}

static const WshShellCmd_t WshShellDefCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "def",
    .Descr   = "Default command for configuring and querying the shell interface",
    .Options = WshShell_OptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(WshShell_OptArr),
    .Handler = WshShellCmdDef,
};

#else /* WSH_SHELL_DEF_COMMAND */

WSH_SHELL_RET_STATE_t WshShellCmdDef_Dummy(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                           const WshShell_Char_t* pArgv[], void* pShellCtx) {
    return WSH_SHELL_RET_STATE_ERR_EMPTY;
}

static const WshShellCmd_t WshShellDefCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "def",
    .Descr   = "",
    .Options = NULL,
    .OptNum  = 0,
    .Handler = WshShellCmdDef_Dummy,
};

#endif /* WSH_SHELL_DEF_COMMAND */

const WshShellCmd_t* WshShellDefCmd_GetPtr(void) {
    return &WshShellDefCmd;
}
