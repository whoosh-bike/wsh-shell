#include "wsh_shell_cmd.h"
#include "wsh_shell.h"
#include "wsh_shell_option.h"
#include "wsh_shell_types.h"

typedef struct {
    const WshShellCmd_t* pCmd;
    WshShell_Size_t Num;
} WshShellCmd_Table_t;

static WshShellCmd_Table_t CmdTable = {0};

WSH_SHELL_RET_STATE_t WshShellCmd_Init(const WshShellCmd_t* pCmdTable, WshShell_Size_t cmdNum) {
    WSH_SHELL_ASSERT(pCmdTable != NULL);
    WSH_SHELL_ASSERT(cmdNum != 0);
    WSH_SHELL_ASSERT(CmdTable.pCmd == NULL);

    if (pCmdTable == NULL || cmdNum == 0 || CmdTable.pCmd != NULL)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    CmdTable.pCmd = pCmdTable;
    CmdTable.Num  = cmdNum;
    return WSH_SHELL_RET_STATE_SUCCESS;
}

void WshShellCmd_Destroy(void) {
    CmdTable = (WshShellCmd_Table_t){0};
}

WshShell_Size_t WshShellCmd_GetCmdNum(void) {
    return CmdTable.Num;
}

const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShell_Size_t id) {
    WSH_SHELL_ASSERT(id < CmdTable.Num);
    return id < CmdTable.Num ? &CmdTable.pCmd[id] : NULL;
}

void WshShellCmd_PrintInfo(const WshShellCmd_t* pCmd) {
    WSH_SHELL_ASSERT(pCmd != NULL);
    if (pCmd == NULL)
        return;

    WSH_SHELL_PRINT("%s : %s\r\n", pCmd->pName, pCmd->pHelp);
    WSH_SHELL_PRINT("Options : \r\n");
    for (const WshShellOption_t* pOpt = pCmd->pOptions; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
        if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
            continue;
        WSH_SHELL_PRINT(WSH_SHELL_SPACE_TAB "[%s], [%s]: %s\r\n", pOpt->pLongName, pOpt->pShortName, pOpt->pHelp);
    }
}

/**
 * @brief Find option in command instance by its name.
 *
 * @param[in] pCmd: Handle of a command instance.
 * @param[in] strLen: Length of a option name string.
 * @param[in] pStr: Pointer to a string with option name.
 *
 * @retval NULL: Option not found.
 * @retval WshShellOption_Handle_t: Handle of a founded option instance.
 */
static const WshShellOption_t* WshShellCmd_FindOpt(const WshShellCmd_t* pCmd, const char* pStr,
                                                   WshShell_Size_t strLen) {
    const WshShellOption_t* waitsInputHandle = NULL;
    for (const WshShellOption_t* pOpt = pCmd->pOptions; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
        WSH_SHELL_OPTION_TYPE_t optType = pOpt->Type;

        // If we are here -> there are options -> we ignore NO_OPTIONS case
        if (optType == WSH_SHELL_OPTION_NO)
            continue;

        // command is waiting for multiple NON_OPTION input
        if (optType == WSH_SHELL_OPTION_WAITS_INPUT) {
            waitsInputHandle = pOpt;
            continue;
        }

        int result = -1;
        if (strLen == WSH_SHELL_OPTION_SHORT_NAME_LEN) {
            result = WSH_SHELL_STRNCMP(pOpt->pShortName, pStr, WSH_SHELL_OPTION_SHORT_NAME_LEN);
        } else {
            result = WSH_SHELL_STRNCMP(pOpt->pLongName, pStr, WSH_SHELL_OPTION_LONG_NAME_LEN);
        }

        if (result == 0)
            return pOpt;
    }

    return waitsInputHandle;
}

WshShellCmd_OptDescr_t WshShellCmd_ParseOpt(const WshShellCmd_t* pCmd, WshShell_Size_t argc, const char* pArgv[],
                                            WshShell_Size_t* pTokenPos) {
    WSH_SHELL_ASSERT(pCmd != NULL);
    WSH_SHELL_ASSERT(pArgv != NULL);
    WSH_SHELL_ASSERT(argc != 0);
    WSH_SHELL_ASSERT(pTokenPos != NULL);
    WSH_SHELL_ASSERT(CmdTable.pCmd != NULL);

    WshShellCmd_OptDescr_t optDescr = {0};

    if (pCmd == NULL || pArgv == NULL || argc == 0 || pTokenPos == NULL || CmdTable.pCmd == NULL || *pTokenPos >= argc)
        return optDescr;

    // If we got only command we need to check if command can be called without any options
    if (argc == 1) {
        // Let's search
        for (const WshShellOption_t* pOpt = pCmd->pOptions; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
            if (pOpt->Type == WSH_SHELL_OPTION_NO) {
                optDescr.pOpt = pOpt;
                break;
            }
        }
    } else {
        // If there are options => we need to parse them
        if (*pTokenPos == 0)
            *pTokenPos += 1;
        const char* pStr             = pArgv[*pTokenPos];
        WshShell_Size_t strLen       = WSH_SHELL_STRLEN(pArgv[*pTokenPos]);
        const WshShellOption_t* pOpt = WshShellCmd_FindOpt(pCmd, pStr, strLen);
        if (pOpt != NULL) {
            optDescr.pOpt     = pOpt;
            optDescr.TokenPos = *pTokenPos;
            // skip option argument if exists
            if (pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT) {
                *pTokenPos = argc;
            } else {
                *pTokenPos += pOpt->ArgNum;
            }
        }
    }
    *pTokenPos += 1;

    return optDescr;
}

WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellCmd_OptDescr_t* pOptDescr, WshShell_Size_t argc,
                                              const char* pArgv[], WshShell_Size_t valueSize, WshShell_Size_t* pValue) {
    WSH_SHELL_ASSERT(pArgv != NULL);
    WSH_SHELL_ASSERT(pOptDescr != NULL);
    WSH_SHELL_ASSERT(valueSize != 0);
    WSH_SHELL_ASSERT(pValue != NULL);
    WSH_SHELL_ASSERT(CmdTable.pCmd != NULL);

    if (pArgv == NULL || pOptDescr == NULL || valueSize == 0 || pValue == NULL || CmdTable.pCmd == NULL)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (argc < 2 || pOptDescr->TokenPos == 0)
        return WSH_SHELL_RET_STATE_ERR_EMPTY;

    if (pOptDescr->TokenPos + 1 >= argc)
        return WSH_SHELL_RET_STATE_ERR_OVERFLOW;

    switch (pOptDescr->pOpt->Type) {
        case WSH_SHELL_OPTION_STR:
            WSH_SHELL_STRNCPY((char*)pValue, pArgv[pOptDescr->TokenPos + 1], valueSize);
            break;

        case WSH_SHELL_OPTION_INT:
            *((WshShell_Size_t*)pValue) = WSH_SHELL_STRTOL(pArgv[pOptDescr->TokenPos + 1], NULL, 10);
            break;

        case WSH_SHELL_OPTION_FLOAT:
            *((float*)pValue) = WSH_SHELL_STRTOF(pArgv[pOptDescr->TokenPos + 1], NULL);
            break;

        case WSH_SHELL_OPTION_NO:
        case WSH_SHELL_OPTION_HELP:
        case WSH_SHELL_OPTION_WO_PARAM:
        case WSH_SHELL_OPTION_MULTI_ARG:
        case WSH_SHELL_OPTION_WAITS_INPUT:
        case WSH_SHELL_OPTION_END:
        case WSH_SHELL_OPTION_ENUM_SIZE:
        default:
            return WSH_SHELL_RET_STATE_ERR_EMPTY;
            break;
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}
