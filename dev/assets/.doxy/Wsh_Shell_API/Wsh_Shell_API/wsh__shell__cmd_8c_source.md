

# File wsh\_shell\_cmd.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd.c**](wsh__shell__cmd_8c.md)

[Go to the documentation of this file](wsh__shell__cmd_8c.md)


```C++
#include "wsh_shell_cmd.h"

static WSH_SHELL_RET_STATE_t WshShellCmd_ValidateCmd(const WshShellCmd_t* pcCmd);

static WSH_SHELL_RET_STATE_t WshShellCmd_ValidateOptions(const WshShellCmd_t* pcCmd) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;

    if (!pcCmd->Options)
        return retState;

    const WshShellOption_t* pcOptOuter = pcCmd->Options;
    for (; pcOptOuter->Type != WSH_SHELL_OPTION_END; pcOptOuter++) {
        const WshShellOption_t* pcOptInner = pcOptOuter + 1;

        for (; pcOptInner->Type != WSH_SHELL_OPTION_END; pcOptInner++) {
            if (pcOptOuter->ShortName && pcOptInner->ShortName && pcOptOuter->ShortName[0] &&
                pcOptInner->ShortName[0] && strcmp(pcOptOuter->ShortName, pcOptInner->ShortName) == 0) {
                WSH_SHELL_PRINT_ERR("Duplicate short option name detected: %s %s\r\n", pcCmd->Name,
                                    pcOptInner->ShortName);
                WSH_SHELL_ASSERT(0);
                retState = WSH_SHELL_RET_STATE_ERROR;
            }

            if (pcOptOuter->LongName && pcOptInner->LongName && pcOptOuter->LongName[0] && pcOptInner->LongName[0] &&
                strcmp(pcOptOuter->LongName, pcOptInner->LongName) == 0) {
                WSH_SHELL_PRINT_ERR("Duplicate long option name detected: %s %s\r\n", pcCmd->Name,
                                    pcOptInner->LongName);
                WSH_SHELL_ASSERT(0);
                retState = WSH_SHELL_RET_STATE_ERROR;
            }
        }
    }

    return retState;
}

#if WSH_SHELL_SUBCOMMANDS
static WSH_SHELL_RET_STATE_t WshShellCmd_ValidateSubCmds(const WshShellCmd_t* pcCmd) {
    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;

    if (pcCmd->SubCmdNum == 0 || !pcCmd->SubCmds)
        return retState;

    for (WshShell_Size_t i = 0; i < pcCmd->SubCmdNum; i++) {
        const WshShellCmd_t* pcSubI = pcCmd->SubCmds[i];
        if (!pcSubI) {
            WSH_SHELL_PRINT_ERR("Null subcommand pointer in %s [%d]\r\n", pcCmd->Name, i);
            WSH_SHELL_ASSERT(0);
            retState = WSH_SHELL_RET_STATE_ERROR;
            continue;
        }

        for (WshShell_Size_t j = i + 1; j < pcCmd->SubCmdNum; j++) {
            const WshShellCmd_t* pcSubJ = pcCmd->SubCmds[j];
            if (pcSubJ && pcSubI->Name && pcSubJ->Name && strcmp(pcSubI->Name, pcSubJ->Name) == 0) {
                WSH_SHELL_PRINT_ERR("Duplicate subcommand name detected: %s %s\r\n", pcCmd->Name, pcSubJ->Name);
                WSH_SHELL_ASSERT(0);
                retState = WSH_SHELL_RET_STATE_ERROR;
            }
        }

        WSH_SHELL_RET_STATE_t subRet = WshShellCmd_ValidateCmd(pcSubI);
        if (subRet != WSH_SHELL_RET_STATE_SUCCESS)
            retState = subRet;
    }

    return retState;
}
#endif /* WSH_SHELL_SUBCOMMANDS */

static WSH_SHELL_RET_STATE_t WshShellCmd_ValidateCmd(const WshShellCmd_t* pcCmd) {
    WSH_SHELL_RET_STATE_t retState = WshShellCmd_ValidateOptions(pcCmd);
#if WSH_SHELL_SUBCOMMANDS
    WSH_SHELL_RET_STATE_t subRet = WshShellCmd_ValidateSubCmds(pcCmd);
    if (subRet != WSH_SHELL_RET_STATE_SUCCESS)
        retState = subRet;
#endif
    return retState;
}

WSH_SHELL_RET_STATE_t WshShellCmd_Attach(WshShellCmd_Table_t* pShellCommands, const WshShellCmd_t* pcCmdTable[],
                                         WshShell_Size_t cmdNum) {
    WSH_SHELL_ASSERT(pShellCommands && pcCmdTable && cmdNum > 0);
    if (!pShellCommands || !pcCmdTable || cmdNum == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pShellCommands->List != NULL)
        return WSH_SHELL_RET_STATE_ERR_BUSY;  // Already inited

    WSH_SHELL_RET_STATE_t retState = WSH_SHELL_RET_STATE_SUCCESS;

    for (WshShell_Size_t cmd = 0; cmd < cmdNum; cmd++) {
        const WshShellCmd_t* pcCmd = pcCmdTable[cmd];
        if (pcCmd == NULL) {
            WSH_SHELL_ASSERT(0);
            continue;
        }

        WSH_SHELL_RET_STATE_t vRet = WshShellCmd_ValidateCmd(pcCmd);
        if (vRet != WSH_SHELL_RET_STATE_SUCCESS)
            retState = vRet;
    }

    pShellCommands->List = pcCmdTable;
    pShellCommands->Num  = cmdNum;

    return retState;
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

const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShellCmd_Table_t* pShellCommands, WshShell_Size_t idx) {
    WSH_SHELL_ASSERT(pShellCommands);
    if (!pShellCommands || !pShellCommands->List)
        return NULL;

    WSH_SHELL_ASSERT(idx < pShellCommands->Num);

    return idx < pShellCommands->Num ? pShellCommands->List[idx] : NULL;
}

const WshShellCmd_t* WshShellCmd_SearchCmd(WshShellCmd_Table_t* pShellCommands, const WshShell_Char_t* pcCmdName) {
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

#if WSH_SHELL_SUBCOMMANDS
WshShell_Size_t WshShellCmd_GetSubCmdNum(const WshShellCmd_t* pcCmd) {
    if (!pcCmd || !pcCmd->SubCmds)
        return 0;

    return pcCmd->SubCmdNum;
}

const WshShellCmd_t* WshShellCmd_GetSubCmdByIndex(const WshShellCmd_t* pcCmd, WshShell_Size_t idx) {
    if (!pcCmd || !pcCmd->SubCmds || idx >= pcCmd->SubCmdNum)
        return NULL;

    return pcCmd->SubCmds[idx];
}

const WshShellCmd_t* WshShellCmd_SearchSubCmd(const WshShellCmd_t* pcCmd, const WshShell_Char_t* pcSubName) {
    if (!pcCmd || !pcSubName || !pcCmd->SubCmds)
        return NULL;

    for (WshShell_Size_t i = 0; i < pcCmd->SubCmdNum; i++) {
        const WshShellCmd_t* pcSub = pcCmd->SubCmds[i];
        if (!pcSub || !pcSub->Name)
            continue;

        if (WSH_SHELL_STRNCMP(pcSub->Name, pcSubName, WSH_SHELL_CMD_NAME_LEN) == 0)
            return pcSub;
    }

    return NULL;
}
#endif /* WSH_SHELL_SUBCOMMANDS */

const WshShellOption_t* WshShellCmd_FindOptByName(const WshShellCmd_t* pcCmd, const WshShell_Char_t* pcName) {
    if (!pcCmd || !pcName || !pcCmd->Options)
        return NULL;

    const WshShellOption_t* pcOpt = pcCmd->Options;
    for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
        if (pcOpt->ShortName && WSH_SHELL_STRNCMP(pcOpt->ShortName, pcName, WSH_SHELL_OPTION_SHORT_NAME_LEN + 1) == 0)
            return pcOpt;
        if (pcOpt->LongName && WSH_SHELL_STRNCMP(pcOpt->LongName, pcName, WSH_SHELL_OPTION_LONG_NAME_LEN) == 0)
            return pcOpt;
    }
    return NULL;
}

static const WshShellOption_t* WshShellCmd_FindOpt(const WshShellCmd_t* pcCmd, const WshShell_Char_t* pcStr,
                                                   WshShell_Size_t strLen) {
    WSH_SHELL_ASSERT(pcCmd && pcStr);
    if (!pcCmd || !pcStr)
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
                const WshShell_Char_t* pRefStr =
                    (strLen == WSH_SHELL_OPTION_SHORT_NAME_LEN) ? pcOpt->ShortName : pcOpt->LongName;
                WshShell_Size_t cmpLen = (strLen == WSH_SHELL_OPTION_SHORT_NAME_LEN) ? WSH_SHELL_OPTION_SHORT_NAME_LEN
                                                                                     : WSH_SHELL_OPTION_LONG_NAME_LEN;

                if (WSH_SHELL_STRNCMP(pRefStr, pcStr, cmpLen) == 0)
                    return pcOpt;
            }
        }
    }

    return pcWaitsInputOpt;
}
WshShellOption_Ctx_t WshShellCmd_ParseOpt(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                          const WshShell_Char_t* pArgv[], WshShell_Size_t rights,
                                          WshShell_Size_t* pTokenPos) {
    WSH_SHELL_ASSERT(pcCmd && pArgv && argc > 0 && pTokenPos);

    WshShellOption_Ctx_t optCtx = {0};
    if (!pcCmd || !pArgv || argc == 0 || !pTokenPos || *pTokenPos >= argc)
        return optCtx;

    /* Always skip command token */
    if (*pTokenPos == 0)
        (*pTokenPos)++;

    WshShell_Size_t startTokenPos = *pTokenPos;

    /* Try to find option in argv */
    while (*pTokenPos < argc) {
        const WshShell_Char_t* pcStr  = pArgv[*pTokenPos];
        WshShell_Size_t strLen        = WSH_SHELL_STRLEN(pcStr);
        const WshShellOption_t* pcOpt = WshShellCmd_FindOpt(pcCmd, pcStr, strLen);

        if (!pcOpt) {
            WSH_SHELL_PRINT_WARN("Unknown option: %s\r\n", pcStr);
            /* Signal parse error to the caller via ParseError flag.
             * Advance past the bad token so the OPTION_NO fallback
             * (tokenPos == startTokenPos check) is not triggered. */
            optCtx.ParseError = true;
            (*pTokenPos)++;
            break;
        }

        /* Verify enough tokens follow for all required arguments */
        if (pcOpt->ArgNum > 0 && *pTokenPos + pcOpt->ArgNum >= argc) {
            WSH_SHELL_PRINT_WARN("Option %s requires %d argument(s)\r\n", pcStr, pcOpt->ArgNum);
            optCtx.ParseError = true;
            (*pTokenPos)++;
            break;
        }

        optCtx.Option   = pcOpt;
        optCtx.TokenPos = *pTokenPos;

        if (pcOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT) {
            *pTokenPos = argc;
        } else {
            *pTokenPos += pcOpt->ArgNum + 1;
        }

        break;
    }

    /* If no option found and no tokens were skipped — use OPTION_NO (empty call) */
    if (!optCtx.Option && *pTokenPos == startTokenPos) {
        const WshShellOption_t* pcOpt = pcCmd->Options;
        for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
            if (pcOpt->Type == WSH_SHELL_OPTION_NO) {
                optCtx.Option   = pcOpt;
                optCtx.TokenPos = *pTokenPos;
                break;
            }
        }
    }

    /* Access rights check (single point) */
    if (optCtx.Option) {
        if (((optCtx.Option->Access & rights) == 0) && !(rights & WSH_SHELL_OPT_ACCESS_ADMIN)) {

            WshShell_Char_t optRightsRow[5];
            WshShell_Char_t usrRightsRow[5];
            WshShellStr_AccessBitsToStr(optCtx.Option->Access, optRightsRow);
            WshShellStr_AccessBitsToStr(rights, usrRightsRow);

            WSH_SHELL_PRINT_ERR("No access rights for option (%s), user (%s)\r\n", optRightsRow, usrRightsRow);

            optCtx.Option = NULL;
        }
    }

    return optCtx;
}

WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellOption_Ctx_t* pOptCtx, WshShell_Size_t argc,
                                              const WshShell_Char_t* pArgv[], WshShell_Size_t valueSize, void* pValue) {
    WSH_SHELL_ASSERT(pOptCtx && pOptCtx->Option && pArgv && pValue && valueSize);
    if (!pOptCtx || !pOptCtx->Option || !pArgv || !pValue || valueSize == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (argc < 2 || pOptCtx->TokenPos == 0)
        return WSH_SHELL_RET_STATE_ERR_EMPTY;

    WshShell_Size_t valIdx = pOptCtx->TokenPos + 1;
    if (valIdx >= argc)
        return WSH_SHELL_RET_STATE_ERR_EMPTY;

    switch (pOptCtx->Option->Type) {
        case WSH_SHELL_OPTION_STR:
            WSH_SHELL_STRNCPY((WshShell_Char_t*)pValue, pArgv[valIdx], valueSize);
            break;

        case WSH_SHELL_OPTION_INT:
            /* base 0 → auto-detect: 0x prefix → hex, 0 prefix → octal, else decimal */
            *((WshShell_Size_t*)pValue) = WSH_SHELL_STRTOL(pArgv[valIdx], NULL, 0);
            break;

        case WSH_SHELL_OPTION_FLOAT:
            *((float*)pValue) = WSH_SHELL_STRTOF(pArgv[valIdx], NULL);
            break;

        case WSH_SHELL_OPTION_ENUM:
            if (pOptCtx->Option->Enum) {
                const WshShellOptionEnum_t* pEnum = pOptCtx->Option->Enum;
                WshShell_Bool_t found             = false;
                for (WshShell_Size_t i = 0; i < pEnum->Count; i++) {
                    if (WSH_SHELL_STRNCMP(pArgv[valIdx], pEnum->Values[i], WSH_SHELL_ENUM_VALUE_MAX_LEN) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    WSH_SHELL_PRINT_WARN("Invalid value \"%s\" for %s\r\n", pArgv[valIdx], pOptCtx->Option->LongName);
                    return WSH_SHELL_RET_STATE_ERR_PARAM;
                }
            }
            WSH_SHELL_STRNCPY((WshShell_Char_t*)pValue, pArgv[valIdx], valueSize);
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
    WSH_SHELL_ASSERT(pcCmd);
    if (!pcCmd)
        return;

    WSH_SHELL_PRINT_SYS("%s\r\n", pcCmd->Descr);

#if WSH_SHELL_CMD_PRINT_OPT_OVERVIEW

    const WshShell_Size_t shortNameMaxLen = WSH_SHELL_OPTION_SHORT_NAME_LEN + 5;
    const WshShell_Size_t longNameMaxLen  = WSH_SHELL_OPTION_LONG_NAME_LEN;
    const WshShell_Size_t typeMaxLen      = 10;
    const WshShell_Size_t accessMaxLen    = 6;

    if (pcCmd->Options) {
        WSH_SHELL_PRINT_SYS("\r\nOptions overview:\r\n");

        WshShell_Char_t headTemplate[64];
        WSH_SHELL_SNPRINTF(headTemplate, sizeof(headTemplate),
                           WSH_SHELL_COLOR_SYS "  %%-%ds %%-%ds %%-%ds %%-%ds %%s\r\n" WSH_SHELL_ESC_RESET_STYLE,
                           shortNameMaxLen, longNameMaxLen, typeMaxLen, accessMaxLen);

        WSH_SHELL_PRINT(headTemplate, "Short", "Long", "Type", "Access", "Descr");

        WshShell_Char_t rowTemplate[64];
        WSH_SHELL_SNPRINTF(rowTemplate, sizeof(rowTemplate), "  %%-%ds %%-%ds %%-%ds %%-%ds %%s\r\n", shortNameMaxLen,
                           longNameMaxLen, typeMaxLen, accessMaxLen);

        const WshShellOption_t* pcOpt = pcCmd->Options;
        for (; pcOpt->Type != WSH_SHELL_OPTION_END; pcOpt++) {
            if (pcOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                continue;

            WshShell_Char_t accessRow[8];
            WshShellStr_AccessBitsToStr(pcOpt->Access, accessRow);
            WSH_SHELL_PRINT(rowTemplate, pcOpt->ShortName, pcOpt->LongName, WshShell_OptTypeStr_Get(pcOpt->Type),
                            accessRow, pcOpt->Descr);
        }
    }

#if WSH_SHELL_SUBCOMMANDS
    if (pcCmd->SubCmdNum > 0 && pcCmd->SubCmds != NULL) {
        const WshShell_Size_t subNameMaxLen = WSH_SHELL_CMD_NAME_LEN;

        WshShell_Char_t subHeadTemplate[64];
        WSH_SHELL_SNPRINTF(subHeadTemplate, sizeof(subHeadTemplate),
                           WSH_SHELL_COLOR_SYS "\r\nSubcommands:\r\n  %%-%ds %%s\r\n" WSH_SHELL_ESC_RESET_STYLE,
                           subNameMaxLen);
        WSH_SHELL_PRINT(subHeadTemplate, "Name", "Descr");

        WshShell_Char_t subRowTemplate[32];
        WSH_SHELL_SNPRINTF(subRowTemplate, sizeof(subRowTemplate), "  %%-%ds %%s\r\n", subNameMaxLen);

        for (WshShell_Size_t i = 0; i < pcCmd->SubCmdNum; i++) {
            const WshShellCmd_t* pcSub = pcCmd->SubCmds[i];
            if (!pcSub || !pcSub->Name)
                continue;
            WSH_SHELL_PRINT(subRowTemplate, pcSub->Name, pcSub->Descr ? pcSub->Descr : "");
        }
    }
#endif /* WSH_SHELL_SUBCOMMANDS */

#else /* WSH_SHELL_CMD_PRINT_OPT_OVERVIEW */

    return;

#endif /* WSH_SHELL_CMD_PRINT_OPT_OVERVIEW */
}
```


