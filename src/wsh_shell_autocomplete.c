#include "wsh_shell_autocomplete.h"

#if WSH_SHELL_AUTOCOMPLETE

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
            for (; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
                if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                    continue;

                optFound = true;
                WSH_SHELL_PRINT_INFO("[%s/%s] ", pOpt->ShortName, pOpt->LongName);
            }

            WSH_SHELL_PRINT_INFO("%s\r\n", optFound ? "" : "none");
            return false;
        }

        WSH_SHELL_STRNCPY(pInBuff, candidates[0], WSH_SHELL_CMD_NAME_LEN);
        pInBuff[WSH_SHELL_CMD_NAME_LEN - 1] = '\0';
        return true;
    }

    // Determine longest common prefix among all matches
    WshShell_Size_t prefixLen = inBuffLen;
    WshShell_Bool_t match     = true;

    while (match && prefixLen < WSH_SHELL_CMD_NAME_LEN) {
        WshShell_Char_t nextChar = candidates[0][prefixLen];
        for (WshShell_Size_t cnt = 1; cnt < matchCount; cnt++) {
            if (candidates[cnt][prefixLen] != nextChar) {
                match = false;
                break;
            }
        }

        if (match)
            prefixLen++;
    }

    // If common prefix is longer than input, extend input
    if (prefixLen > inBuffLen) {
        WSH_SHELL_MEMCPY((void*)&pInBuff[inBuffLen], (void*)&candidates[0][inBuffLen],
                         prefixLen - inBuffLen);
        pInBuff[prefixLen] = '\0';
        return true;
    }

    // Print all candidates as options
    WSH_SHELL_PRINT_SYS("%s /autocomplete cmd found: ", sysMsgShift);
    for (WshShell_Size_t cnt = 0; cnt < matchCount; cnt++)
        WSH_SHELL_PRINT_INFO("%s ", candidates[cnt]);

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

    WSH_SHELL_PRINT_SYS("%s /autocomplete disabled", sysMsgShift);
    WSH_SHELL_PRINT("\r\n");

    return false;
}

#endif /* WSH_SHELL_AUTOCOMPLETE */
