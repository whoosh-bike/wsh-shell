

# File wsh\_shell\_cmd.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd.c**](wsh__shell__cmd_8c.md)

[Go to the documentation of this file](wsh__shell__cmd_8c.md)


```C++
#include "wsh_shell_cmd.h"

WSH_SHELL_RET_STATE_t WshShellCmd_Attach(WshShellCmd_Table_t* pShellCommands,
                                         const WshShellCmd_t* pcCmdTable[],
                                         WshShell_Size_t cmdNum) {
    WSH_SHELL_ASSERT(pShellCommands && pcCmdTable && cmdNum > 0);
    if (!pShellCommands || !pcCmdTable || cmdNum == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pShellCommands->List != NULL)
        return WSH_SHELL_RET_STATE_ERR_BUSY;  // Already inited

    pShellCommands->List = pcCmdTable;
    pShellCommands->Num  = cmdNum;

    return WSH_SHELL_RET_STATE_SUCCESS;
}

void WshShellCmd_DeAttach(WshShellCmd_Table_t* pShellCommands) {
    WSH_SHELL_ASSERT(pShellCommands);

    if (pShellCommands)
        *pShellCommands = (WshShellCmd_Table_t){0};
}

WshShell_Size_t WshShellCmd_GetCmdNum(WshShellCmd_Table_t* pShellCommands) {
    WSH_SHELL_ASSERT(pShellCommands);

    if (!pShellCommands || !pShellCommands->List)
        return 0;

    return pShellCommands->Num;
}

const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShellCmd_Table_t* pShellCommands,
                                               WshShell_Size_t idx) {
    WSH_SHELL_ASSERT(pShellCommands);
    if (!pShellCommands || !pShellCommands->List)
        return NULL;

    WSH_SHELL_ASSERT(idx < pShellCommands->Num);

    return idx < pShellCommands->Num ? pShellCommands->List[idx] : NULL;
}

const WshShellCmd_t* WshShellCmd_SearchCmd(WshShellCmd_Table_t* pShellCommands,
                                           const WshShell_Char_t* pcCmdName) {
    WSH_SHELL_ASSERT(pcCmdName);
    if (!pcCmdName)
        return NULL;

    for (WshShell_Size_t cmd = 0; cmd < WshShellCmd_GetCmdNum(pShellCommands); cmd++) {
        const WshShellCmd_t* pcCmd = WshShellCmd_GetCmdByIndex(pShellCommands, cmd);
        if (WSH_SHELL_STRNCMP(pcCmd->Name, pcCmdName, WSH_SHELL_CMD_NAME_LEN) == 0)
            return pcCmd;
    }

    return NULL;
}

static const WshShellOption_t* WshShellCmd_FindOpt(const WshShellCmd_t* pcCmd,
                                                   const WshShell_Char_t* pcStr,
                                                   WshShell_Size_t strLen) {
    WSH_SHELL_ASSERT(pcCmd && pcStr);
    if (!pcCmd || !pcCmd)
        return NULL;

    const WshShellOption_t* pcWaitsInputOpt = NULL;

    const WshShellOption_t* pcOpt = pcCmd->Options;
    for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
        switch (pcOpt->Type) {
            case WSH_SHELL_OPTION_NO:
                continue;

            case WSH_SHELL_OPTION_WAITS_INPUT:
                pcWaitsInputOpt = pcOpt;
                continue;

            default: {
                const WshShell_Char_t* pRefStr = (strLen == WSH_SHELL_OPTION_SHORT_NAME_LEN)
                                                     ? pcOpt->ShortName
                                                     : pcOpt->LongName;
                WshShell_Size_t cmpLen         = (strLen == WSH_SHELL_OPTION_SHORT_NAME_LEN)
                                                     ? WSH_SHELL_OPTION_SHORT_NAME_LEN
                                                     : WSH_SHELL_OPTION_LONG_NAME_LEN;

                if (WSH_SHELL_STRNCMP(pRefStr, pcStr, cmpLen) == 0)
                    return pcOpt;
            }
        }
    }

    return pcWaitsInputOpt;
}

WshShellOption_Context_t WshShellCmd_ParseOpt(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                              const WshShell_Char_t* pArgv[],
                                              WshShell_Size_t* pTokenPos) {
    WSH_SHELL_ASSERT(pcCmd && pArgv && argc > 0 && pTokenPos);
    WshShellOption_Context_t optCtx = {0};
    if (!pcCmd || !pArgv || argc == 0 || !pTokenPos || *pTokenPos >= argc)
        return optCtx;

    if (argc == 1) {  // Only command without options
        const WshShellOption_t* pcOpt = pcCmd->Options;
        for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
            if (pcOpt->Type == WSH_SHELL_OPTION_NO) {
                optCtx.Option = pcOpt;
                break;
            }
        }

        (*pTokenPos)++;
        return optCtx;
    }

    if (*pTokenPos == 0)  // Skip command token if not yet skipped
        (*pTokenPos)++;

    const WshShell_Char_t* pcStr  = pArgv[*pTokenPos];
    WshShell_Size_t strLen        = WSH_SHELL_STRLEN(pcStr);
    const WshShellOption_t* pcOpt = WshShellCmd_FindOpt(pcCmd, pcStr, strLen);

    if (pcOpt) {
        optCtx.Option   = pcOpt;
        optCtx.TokenPos = *pTokenPos;

        if (pcOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT) {
            // Consume all remaining arguments
            *pTokenPos = argc;
        } else {
            *pTokenPos += pcOpt->ArgNum;
        }
    }

    (*pTokenPos)++;
    return optCtx;
}

WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellOption_Context_t* pOptCtx,
                                              WshShell_Size_t argc, const WshShell_Char_t* pArgv[],
                                              WshShell_Size_t valueSize, void* pValue) {
    WSH_SHELL_ASSERT(pOptCtx && pOptCtx->Option && pArgv && pValue && valueSize);
    if (!pOptCtx || !pOptCtx->Option || !pArgv || !pValue || valueSize == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (argc < 2 || pOptCtx->TokenPos == 0)
        return WSH_SHELL_RET_STATE_ERR_EMPTY;

    WshShell_Size_t valIdx = pOptCtx->TokenPos + 1;
    if (valIdx >= argc)
        return WSH_SHELL_RET_STATE_ERR_OVERFLOW;  //FIXME ?

    switch (pOptCtx->Option->Type) {
        case WSH_SHELL_OPTION_STR:
            WSH_SHELL_STRNCPY((WshShell_Char_t*)pValue, pArgv[valIdx], valueSize);
            break;

        case WSH_SHELL_OPTION_INT:
            *((WshShell_Size_t*)pValue) = WSH_SHELL_STRTOL(pArgv[valIdx], NULL, 10);
            break;

        case WSH_SHELL_OPTION_FLOAT:
            *((float*)pValue) = WSH_SHELL_STRTOF(pArgv[valIdx], NULL);
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
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

void WshShellCmd_PrintOptionsOverview(const WshShellCmd_t* pcCmd) {
#if WSH_SHELL_CMD_PRINT_OPT_OVERVIEW

    WSH_SHELL_ASSERT(pcCmd);
    if (!pcCmd)
        return;

    WSH_SHELL_PRINT_SYS("%s\r\nOptions overview:\r\n", pcCmd->Descr);

    const WshShell_Size_t shortNameMaxLen = WSH_SHELL_OPTION_SHORT_NAME_LEN + 5;
    const WshShell_Size_t longNameMaxLen  = WSH_SHELL_OPTION_LONG_NAME_LEN;
    const WshShell_Size_t typeMaxLen      = 10;
    const WshShell_Size_t accessMaxLen    = 6;

    WshShell_Char_t headTemplate[64];
    WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                       WSH_SHELL_COLOR_SYS
                       "  %%-%ds %%-%ds %%-%ds %%-%ds %%s\r\n" WSH_SHELL_ESC_RESET_STYLE,
                       shortNameMaxLen, longNameMaxLen, typeMaxLen, accessMaxLen);

    WSH_SHELL_PRINT(headTemplate, "Short", "Long", "Type", "Access", "Descr");

    WshShell_Char_t rowTemplate[64];
    WSH_SHELL_SNPRINTF(rowTemplate, sizeof(rowTemplate), "  %%-%ds %%-%ds %%-%ds %%-%ds %%s\r\n",
                       shortNameMaxLen, longNameMaxLen, typeMaxLen, accessMaxLen);

    const WshShellOption_t* pcOpt = pcCmd->Options;
    for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
        if (pcOpt->Type == WSH_SHELL_OPTION_NO || pcOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
            continue;

        WshShell_Char_t accessRow[8];
        WshShellStr_AccessBitsToStr(pcOpt->Access, accessRow);
        WSH_SHELL_PRINT(rowTemplate, pcOpt->ShortName, pcOpt->LongName,
                        WshShell_OptTypeStr_Get(pcOpt->Type), accessRow, pcOpt->Descr);
    }

#else /* WSH_SHELL_CMD_PRINT_OPT_OVERVIEW */

    return;

#endif /* WSH_SHELL_CMD_PRINT_OPT_OVERVIEW */
}
```


