

# File wsh\_shell\_autocomplete.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_autocomplete.c**](wsh__shell__autocomplete_8c.md)

[Go to the documentation of this file](wsh__shell__autocomplete_8c.md)


```C++
#include "wsh_shell_autocomplete.h"

#if WSH_SHELL_AUTOCOMPLETE

static WshShell_Size_t WshShellAutocomplete_CommonPrefixLen(const WshShell_Char_t* pFirst,
                                                            WshShell_Size_t stride,
                                                            WshShell_Size_t count,
                                                            WshShell_Size_t startLen,
                                                            WshShell_Size_t maxLen) {
    WshShell_Size_t prefixLen = startLen;
    while (prefixLen < maxLen) {
        WshShell_Char_t nextChar = pFirst[prefixLen];
        WshShell_Bool_t match    = true;
        for (WshShell_Size_t i = 1; i < count; i++) {
            if ((pFirst + i * stride)[prefixLen] != nextChar) {
                match = false;
                break;
            }
        }
        if (!match)
            break;
        prefixLen++;
    }
    return prefixLen;
}

WshShell_Bool_t WshShellAutocomplete_Try(WshShell_Char_t* pInBuff, WshShell_Size_t inBuffLen,
                                         WshShellCmd_Table_t* pShellCommands) {
    WSH_SHELL_ASSERT(pInBuff && pShellCommands);
    if (!pInBuff || !pShellCommands)
        return false;

    WshShell_Char_t inputCopy[WSH_SHELL_INTR_BUFF_LEN] = {0};
    WSH_SHELL_STRNCPY(inputCopy, pInBuff, inBuffLen);

    const WshShell_Char_t* pcInputCopyTrimmed =
        WshShellStr_TrimString(inputCopy, WSH_SHELL_STRLEN(inputCopy));
    WshShell_Size_t inputCopyTrimmedLen = WSH_SHELL_STRLEN(pcInputCopyTrimmed);

    const WshShellCmd_t* pcDefCmd = WshShellDefCmd_GetPtr();

    WshShell_Size_t cmdNum = WshShellCmd_GetCmdNum(pShellCommands);

    // Check if number of commands exceeds safe stack allocation limit
    if (cmdNum + 1 > WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES) {
        WSH_SHELL_PRINT_WARN("Too many commands for autocomplete (%d > %d)\r\n", cmdNum + 1,
                             WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES);
        return false;
    }

    WshShell_Char_t candidates[cmdNum + 1][WSH_SHELL_CMD_NAME_LEN];  // + one more for default cmd
    WshShell_Size_t matchCount      = 0;
    const WshShellCmd_t* pcCmdMatch = NULL;

    // Check default command first
    if (WSH_SHELL_STRNCMP(pcInputCopyTrimmed, pcDefCmd->Name, inputCopyTrimmedLen) == 0) {
        WSH_SHELL_STRNCPY(candidates[matchCount], pcDefCmd->Name, WSH_SHELL_CMD_NAME_LEN - 1);
        candidates[matchCount][WSH_SHELL_CMD_NAME_LEN - 1] = '\0';  // safety null-term
        pcCmdMatch                                         = pcDefCmd;
        matchCount++;
    }

    // Collect other matching commands
    for (WshShell_Size_t cmdIdx = 0; cmdIdx < cmdNum; cmdIdx++) {
        const WshShellCmd_t* pcCmd = WshShellCmd_GetCmdByIndex(pShellCommands, cmdIdx);
        if (WSH_SHELL_STRNCMP(pcInputCopyTrimmed, pcCmd->Name, inputCopyTrimmedLen) == 0) {
            WSH_SHELL_STRNCPY(candidates[matchCount], pcCmd->Name, WSH_SHELL_CMD_NAME_LEN);
            pcCmdMatch = pcCmd;
            matchCount++;
        }
    }

    // Create padding buff for pretty message print
    WshShell_Char_t sysMsgShift[WSH_SHELL_AUTOCOMPLETE_PAD_LEN + 1];
    WshShell_Size_t padLen = (inBuffLen < WSH_SHELL_AUTOCOMPLETE_PAD_LEN)
                                 ? (WSH_SHELL_AUTOCOMPLETE_PAD_LEN - inBuffLen)
                                 : 0;
    WSH_SHELL_MEMSET((void*)sysMsgShift, WSH_SHELL_AUTOCOMPLETE_PAD_SYM, padLen);
    sysMsgShift[0]      = ' ';
    sysMsgShift[padLen] = '\0';

    // If input contains a space — try flag autocomplete for the last typed token
    WshShell_Size_t firstSpaceIdx = inputCopyTrimmedLen;
    WshShell_Size_t lastSpaceIdx  = inputCopyTrimmedLen;
    for (WshShell_Size_t i = 0; i < inputCopyTrimmedLen; i++) {
        if (pcInputCopyTrimmed[i] == ' ') {
            if (firstSpaceIdx == inputCopyTrimmedLen)
                firstSpaceIdx = i;
            lastSpaceIdx = i;
        }
    }

    if (firstSpaceIdx < inputCopyTrimmedLen) {
        WshShell_Size_t cmdPartLen        = firstSpaceIdx;
        const WshShell_Char_t* pcFlagPart = pcInputCopyTrimmed + lastSpaceIdx + 1;
        WshShell_Size_t flagPartLen       = WSH_SHELL_STRLEN(pcFlagPart);

        // Find command matching first token (exact)
        const WshShellCmd_t* pcTargetCmd = NULL;
        if (WSH_SHELL_STRLEN(pcDefCmd->Name) == cmdPartLen &&
            WSH_SHELL_STRNCMP(pcInputCopyTrimmed, pcDefCmd->Name, cmdPartLen) == 0) {
            pcTargetCmd = pcDefCmd;
        }
        for (WshShell_Size_t i = 0; !pcTargetCmd && i < cmdNum; i++) {
            const WshShellCmd_t* pcCmd = WshShellCmd_GetCmdByIndex(pShellCommands, i);
            if (WSH_SHELL_STRLEN(pcCmd->Name) == cmdPartLen &&
                WSH_SHELL_STRNCMP(pcInputCopyTrimmed, pcCmd->Name, cmdPartLen) == 0) {
                pcTargetCmd = pcCmd;
            }
        }

        if (!pcTargetCmd) {
            WSH_SHELL_PRINT_SYS("%s /autocomplete: unknown command\r\n", sysMsgShift);
            return false;
        }

        // Collect flags matching partial flag token
        WshShell_Char_t flagCandidates[WSH_SHELL_CMD_OPTIONS_MAX_NUM][WSH_SHELL_OPTION_LONG_NAME_LEN];
        WshShell_Size_t flagMatchCount = 0;

        const WshShellOption_t* pOpt = pcTargetCmd->Options;
        for (; pOpt->Type != WSH_SHELL_OPTION_END && flagMatchCount < WSH_SHELL_CMD_OPTIONS_MAX_NUM;
             pOpt++) {
            if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                continue;

            const WshShell_Char_t* pcName = NULL;
            if (pOpt->LongName[0] &&
                WSH_SHELL_STRNCMP(pcFlagPart, pOpt->LongName, flagPartLen) == 0) {
                pcName = pOpt->LongName;
            } else if (flagPartLen <= WSH_SHELL_OPTION_SHORT_NAME_LEN && pOpt->ShortName[0] &&
                       WSH_SHELL_STRNCMP(pcFlagPart, pOpt->ShortName, flagPartLen) == 0) {
                pcName = pOpt->ShortName;
            }

            if (pcName) {
                WSH_SHELL_STRNCPY(flagCandidates[flagMatchCount], pcName,
                                  WSH_SHELL_OPTION_LONG_NAME_LEN - 1);
                flagCandidates[flagMatchCount][WSH_SHELL_OPTION_LONG_NAME_LEN - 1] = '\0';
                flagMatchCount++;
            }
        }

        if (flagMatchCount == 0) {
            WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
            return false;
        }

        if (flagMatchCount == 1) {
            WshShell_Size_t flagLen = WSH_SHELL_STRLEN(flagCandidates[0]);
            WshShell_Size_t newLen  = cmdPartLen + 1 + flagLen;
            if (newLen + 2 > WSH_SHELL_INTR_BUFF_LEN) {
                WSH_SHELL_PRINT_WARN("Flag name too long for buffer\r\n");
                return false;
            }
            WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, cmdPartLen);
            pInBuff[cmdPartLen] = ' ';
            WSH_SHELL_MEMCPY(pInBuff + cmdPartLen + 1, flagCandidates[0], flagLen);
            pInBuff[cmdPartLen + 1 + flagLen]     = ' ';
            pInBuff[cmdPartLen + 1 + flagLen + 1] = '\0';
            return true;
        }

        // Find longest common prefix among flag candidates
        WshShell_Size_t flagPrefixLen = WshShellAutocomplete_CommonPrefixLen(
            flagCandidates[0], WSH_SHELL_OPTION_LONG_NAME_LEN, flagMatchCount, flagPartLen,
            WSH_SHELL_OPTION_LONG_NAME_LEN);

        if (flagPrefixLen > flagPartLen) {
            WshShell_Size_t newLen = cmdPartLen + 1 + flagPrefixLen;
            if (newLen + 1 > WSH_SHELL_INTR_BUFF_LEN) {
                WSH_SHELL_PRINT_WARN("Flag prefix too long for buffer\r\n");
                return false;
            }
            WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, cmdPartLen);
            pInBuff[cmdPartLen] = ' ';
            WSH_SHELL_MEMCPY(pInBuff + cmdPartLen + 1, flagCandidates[0], flagPrefixLen);
            pInBuff[newLen] = '\0';
            return true;
        }

        // Print all matching flag candidates
        WSH_SHELL_PRINT_SYS("%s /autocomplete flags found: ", sysMsgShift);
        for (WshShell_Size_t cnt = 0; cnt < flagMatchCount; cnt++)
            WSH_SHELL_PRINT("[%s] ", flagCandidates[cnt]);
        WSH_SHELL_PRINT("\r\n");
        return false;
    }

    // No matches found
    if (matchCount == 0) {
        WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
        return false;
    }

    // Exactly one match found — autocomplete to full command
    if (matchCount == 1) {
        //Fetch options if command already autocomplited
        if (inputCopyTrimmedLen == WSH_SHELL_STRLEN(candidates[0]) &&
            WSH_SHELL_MEMCMP(pcInputCopyTrimmed, candidates[0], inputCopyTrimmedLen) == 0) {
            WSH_SHELL_PRINT_SYS("%s /autocomplete flags found: ", sysMsgShift);

            const WshShellOption_t* pOpt = pcCmdMatch->Options;
            WshShell_Bool_t optFound     = false;
            if (pOpt != NULL) {
                for (; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
                    if (pOpt->Type == WSH_SHELL_OPTION_NO ||
                        pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                        continue;

                    optFound = true;
                    WSH_SHELL_PRINT("[%s/%s] ", pOpt->ShortName, pOpt->LongName);
                }
            }

            WSH_SHELL_PRINT("%s\r\n", optFound ? "" : "none");
            return false;
        }

        WshShell_Size_t candLen = WSH_SHELL_STRLEN(candidates[0]);

        // Check if we have space for command + space + null terminator
        if (candLen + 2 > WSH_SHELL_INTR_BUFF_LEN) {
            WSH_SHELL_PRINT_WARN("Command name too long for buffer\r\n");
            return false;
        }

        candidates[0][candLen++] = ' ';  //add extra space if command found
        WSH_SHELL_STRNCPY(pInBuff, candidates[0], candLen);
        pInBuff[candLen] = '\0';
        return true;
    }

    // Determine longest common prefix among all matches
    WshShell_Size_t prefixLen = WshShellAutocomplete_CommonPrefixLen(
        candidates[0], WSH_SHELL_CMD_NAME_LEN, matchCount, inBuffLen, WSH_SHELL_CMD_NAME_LEN);

    // If common prefix is longer than input, extend input
    if (prefixLen > inBuffLen) {
        // Check if prefix fits in buffer
        if (prefixLen + 1 > WSH_SHELL_INTR_BUFF_LEN) {
            WSH_SHELL_PRINT_WARN("Common prefix too long for buffer\r\n");
            return false;
        }
        WSH_SHELL_MEMCPY((void*)&pInBuff[inBuffLen], (void*)&candidates[0][inBuffLen],
                         prefixLen - inBuffLen);
        pInBuff[prefixLen] = '\0';
        return true;
    }

    // Print all candidates as options
    WSH_SHELL_PRINT_SYS("%s /autocomplete cmd found: ", sysMsgShift);
    for (WshShell_Size_t cnt = 0; cnt < matchCount; cnt++)
        WSH_SHELL_PRINT("%s ", candidates[cnt]);

    WSH_SHELL_PRINT("\r\n");

    return false;
}

#else /* WSH_SHELL_AUTOCOMPLETE */

WshShell_Bool_t WshShellAutocomplete_Try(WshShell_Char_t* pInBuff, WshShell_Size_t inBuffLen,
                                         WshShellCmd_Table_t* pShellCommands) {
    WSH_SHELL_ASSERT(pInBuff && pShellCommands);
    if (!pInBuff || !pShellCommands)
        return false;

    WshShell_Char_t sysMsgShift[WSH_SHELL_AUTOCOMPLETE_PAD_LEN + 1];
    WshShell_Size_t padLen = (inBuffLen < WSH_SHELL_AUTOCOMPLETE_PAD_LEN)
                                 ? (WSH_SHELL_AUTOCOMPLETE_PAD_LEN - inBuffLen)
                                 : 0;
    WSH_SHELL_MEMSET((void*)sysMsgShift, WSH_SHELL_AUTOCOMPLETE_PAD_SYM, padLen);
    sysMsgShift[0]      = ' ';
    sysMsgShift[padLen] = '\0';

    WSH_SHELL_PRINT_WARN("%s /autocomplete disabled", sysMsgShift);
    WSH_SHELL_PRINT("\r\n");

    return false;
}

#endif /* WSH_SHELL_AUTOCOMPLETE */
```


