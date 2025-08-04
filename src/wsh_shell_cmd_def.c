#include "wsh_shell_cmd_def.h"
#include "wsh_shell.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_types.h"

WSH_SHELL_RET_STATE_t WshShellCmdDef_Help_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;
    WshShell_Size_t tokenPos       = 0;
    while (tokenPos < argc) {
        WshShellCmd_OptDescr_t optDescr = WshShellCmd_ParseOpt(pCmd, argc, pArgv, &tokenPos);
        if (!WshShell_CheckUserAccess(&optDescr)) {
            retState = WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
        }

        switch (optDescr.pOpt->ID) {
            case WSH_SHELL_HELP_OPT_HELP:
                WshShellCmd_PrintInfo(pCmd);
                break;

            case WSH_SHELL_HELP_OPT_DEF:
                for (WshShell_Size_t i = 0; i < WshShellCmd_GetCmdNum(); i++) {
                    const WshShellCmd_t* pCmd = WshShellCmd_GetCmdByIndex(i);
                    if (pCmd != NULL)
                        WSH_SHELL_PRINT("%s : %s" WSH_SHELL_END_LINE, pCmd->pName, pCmd->pHelp);
                }
                break;

            default:
                retState = WSH_SHELL_RET_STATE_ERROR;
                break;
        }
    }
    return retState;
}

WSH_SHELL_RET_STATE_t WshShellCmdDef_Cls_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                    const char** pArgv) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;
    WshShell_Size_t tokenPos       = 0;
    while (tokenPos < argc) {
        WshShellCmd_OptDescr_t optDescr = WshShellCmd_ParseOpt(pCmd, argc, pArgv, &tokenPos);
        if (!WshShell_CheckUserAccess(&optDescr)) {
            retState = WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
        }

        switch (optDescr.pOpt->ID) {
            case WSH_SHELL_CLS_OPT_HELP:
                WshShellCmd_PrintInfo(pCmd);
                break;

            case WSH_SHELL_CLS_OPT_DEF:
                WSH_SHELL_PRINT(WSH_SHELL_ECS_CLR_SCREEN);
                break;

            default:
                retState = WSH_SHELL_RET_STATE_ERROR;
                break;
        }
    }
    return retState;
}

WSH_SHELL_RET_STATE_t WshShellCmdDef_Echo_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;
    WshShell_Size_t tokenPos       = 0;
    while (tokenPos < argc) {
        WshShellCmd_OptDescr_t optDescr = WshShellCmd_ParseOpt(pCmd, argc, pArgv, &tokenPos);
        if (!WshShell_CheckUserAccess(&optDescr)) {
            retState = WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
        }

        switch (optDescr.pOpt->ID) {
            case WSH_SHELL_ECHO_OPT_HELP:
                WshShellCmd_PrintInfo(pCmd);
                break;

            case WSH_SHELL_ECHO_OPT_DEF:
                for (WshShell_Size_t i = 1; i < argc; i++)
                    WSH_SHELL_PRINT("%s ", pArgv[i]);
                WSH_SHELL_PRINT(WSH_SHELL_END_LINE);
                break;

            default:
                retState = WSH_SHELL_RET_STATE_ERROR;
                break;
        }
    }
    return retState;
}

WSH_SHELL_RET_STATE_t WshShellCmdDef_Exit_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;
    WshShell_Size_t tokenPos       = 0;
    while (tokenPos < argc) {
        WshShellCmd_OptDescr_t optDescr = WshShellCmd_ParseOpt(pCmd, argc, pArgv, &tokenPos);
        if (!WshShell_CheckUserAccess(&optDescr)) {
            retState = WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
        }

        switch (optDescr.pOpt->ID) {
            case WSH_SHELL_EXIT_OPT_HELP:
                WshShellCmd_PrintInfo(pCmd);
                break;

            case WSH_SHELL_EXIT_OPT_DEF:
                WshShell_Exit();
                break;

            default:
                retState = WSH_SHELL_RET_STATE_ERROR;
                break;
        }
    }
    return retState;
}
