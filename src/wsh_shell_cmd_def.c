#include "wsh_shell_cmd_def.h"
#include "wsh_shell.h"

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
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HELP, WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_INTERACT, WSH_SHELL_OPT_INTERACT()) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY)) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_EXEC, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-x", "--exec", "Get info about accessible commands")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_USER, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-u", "--user", "Get info about users")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_CLS, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-c", "--cls", "Clear screen")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_CLEAR, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-r", "--histrst", "Reset history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_HIST_PRINT, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-p", "--histprint", "Print history storage")) \
    X_CMD_ENTRY(WSH_SHELL_DEF_OPT_DEAUTH, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-d", "--deauth", "DeAuth and destroy history")) \
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

static void shell_cmd_def__interactive(WshShellIO_CommandLine_t* pInter) {
    WshShellInteract_AppendLineBreak(pInter);

    WSH_SHELL_PRINT("Just echo of interactive command: %s", pInter->Buff);
}

WSH_SHELL_RET_STATE_t WshShellCmdDef_Executable(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                                const WshShell_Char_t* pArgv[], void* pCtx) {
    if ((argc > 0 && pArgv == NULL) || pcCmd == NULL)
        return WSH_SHELL_RET_STATE_ERROR;

    WshShell_t* pParentShell       = (WshShell_t*)pCtx;
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;

    WshShell_Size_t tokenPos = 0;
    while (tokenPos < argc) {
        WshShellOption_Context_t optCtx = WshShellCmd_ParseOpt(pcCmd, argc, pArgv, &tokenPos);
        if (optCtx.Option == NULL) {
            retState = WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
        }

        switch (optCtx.Option->ID) {
            case WSH_SHELL_DEF_OPT_HELP:
                WshShellCmd_PrintInfo(pcCmd);
                break;

            case WSH_SHELL_DEF_OPT_DEF:
                WSH_SHELL_PRINT("Ver: %s\r\n", pParentShell->Version);
                WSH_SHELL_PRINT("Device name: %s\r\n", pParentShell->DeviceName);
                WSH_SHELL_PRINT("User: %s\r\n", pParentShell->CurrUser->Login);
                break;

            case WSH_SHELL_DEF_OPT_INTERACT:
                WshShellInteract_Attach(&(pParentShell->Interact), pcCmd->Name,
                                        shell_cmd_def__interactive);
                break;

            case WSH_SHELL_DEF_OPT_EXEC: {
                WSH_SHELL_PRINT("Availible commans:\r\n");

                const WshShell_Size_t cmdMaxLen   = WSH_SHELL_CMD_NAME_LEN;
                const WshShell_Size_t optMaxLen   = 5;
                const WshShell_Size_t groupMaxLen = WSH_SHELL_CMD_GROUP_MAX_COUNT + 3;

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
                    WshShell_Char_t groupRow[16];
                    WshShellStr_GroupBitsToStr(pcDefCmd->Groups, groupRow);
                    WSH_SHELL_PRINT(rowTemplate, pcDefCmd->Name, pcDefCmd->OptNum - 1, groupRow,
                                    pcDefCmd->Descr);
                }

                WshShell_Size_t commandsNum = WshShellCmd_GetCmdNum(&(pParentShell->Commands));
                for (WshShell_Size_t cmd = 0; cmd < commandsNum; cmd++) {
                    const WshShellCmd_t* pcTargetCmd =
                        WshShellCmd_GetCmdByIndex(&(pParentShell->Commands), cmd);

                    if (pcTargetCmd == NULL)
                        continue;

                    WshShell_Char_t groupRow[16];
                    WshShellStr_GroupBitsToStr(pcTargetCmd->Groups, groupRow);

                    WSH_SHELL_PRINT(rowTemplate, pcTargetCmd->Name, pcTargetCmd->OptNum - 1,
                                    groupRow, pcTargetCmd->Descr);
                }
            } break;

            case WSH_SHELL_DEF_OPT_USER:
                WSH_SHELL_PRINT("Availible users:\r\n");

                const WshShell_Size_t loginMaxLen  = WSH_SHELL_LOGIN_LEN;
                const WshShell_Size_t groupMaxLen  = WSH_SHELL_CMD_GROUP_MAX_COUNT + 3;
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

                    WshShell_Char_t groupRow[16];
                    WshShellStr_GroupBitsToStr(pcTargetUser->Groups, groupRow);

                    WshShell_Char_t rightsRow[16];
                    WshShellStr_AccessBitsToStr(pcTargetUser->Rights, rightsRow);

                    WSH_SHELL_PRINT(rowTemplate, pcTargetUser->Login, groupRow, rightsRow);
                }
                break;

            case WSH_SHELL_DEF_OPT_CLS:
                WSH_SHELL_PRINT(WSH_SHELL_ECS_CLR_SCREEN);
                break;

            case WSH_SHELL_DEF_OPT_HIST_CLEAR:
                WshShellHistory_Flush(&(pParentShell->HistoryIO));
                break;

            case WSH_SHELL_DEF_OPT_HIST_PRINT: {
                WshShell_Char_t cmdBuff[64];
                WshShell_Size_t cmdNum = WshShellHistory_GetTokenNum(&(pParentShell->HistoryIO));

                WSH_SHELL_PRINT("History len: %d\r\n", cmdNum - 1);
                for (WshShell_Size_t cmdIdx = 0; cmdIdx < cmdNum; cmdIdx++) {
                    if (WshShellHistory_GetTokenByIndex(&(pParentShell->HistoryIO), cmdBuff,
                                                        sizeof(cmdBuff), cmdNum - cmdIdx)) {
                        WSH_SHELL_PRINT("  hist[%2d]: %s\r\n", cmdIdx, cmdBuff);
                    }
                }
            } break;

            case WSH_SHELL_DEF_OPT_DEAUTH:
                WshShell_DeAuth(pParentShell);
                break;

            default:
                retState = WSH_SHELL_RET_STATE_ERROR;
                break;
        }
    }

    return retState;
}

#define X_CMD_ENTRY(enum, opt) {enum, opt},
static WshShellOption_t OptArr[] = {WSH_SHELL_CMD_DEF_OPT_TABLE()};
#undef X_CMD_ENTRY

static const WshShellCmd_t WshShellDefCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "shell",
    .Descr   = "Configuring and querying the shell interface",
    .Options = OptArr,
    .OptNum  = WSH_SHELL_ARR_LEN(OptArr),
    .Exec    = WshShellCmdDef_Executable,
};

const WshShellCmd_t* WshShellDefCmd_GetPtr(void) {
    return &WshShellDefCmd;
}
