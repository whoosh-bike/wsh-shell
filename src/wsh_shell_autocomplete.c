#include "wsh_shell_autocomplete.h"

#if WSH_SHELL_AUTOCOMPLETE

static WshShell_Size_t WshShellAutocomplete_CommonPrefixLen(const WshShell_Char_t* pFirst, WshShell_Size_t stride,
                                                            WshShell_Size_t count, WshShell_Size_t startLen,
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

    /* Detect trailing space BEFORE TrimString — signals cursor is past the last token */
    WshShell_Bool_t hadTrailingSpace =
        (inBuffLen > 0 && (pInBuff[inBuffLen - 1] == ' ' || pInBuff[inBuffLen - 1] == '\t'));

    const WshShell_Char_t* pcInputCopyTrimmed = WshShellStr_TrimString(inputCopy, WSH_SHELL_STRLEN(inputCopy));
    WshShell_Size_t inputCopyTrimmedLen       = WSH_SHELL_STRLEN(pcInputCopyTrimmed);

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
    WshShell_Size_t padLen =
        (inBuffLen < WSH_SHELL_AUTOCOMPLETE_PAD_LEN) ? (WSH_SHELL_AUTOCOMPLETE_PAD_LEN - inBuffLen) : 0;
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

        /*
         * prefixEnd tracks where in the input string the "context" prefix
         * ends — i.e. the first token plus any fully-matched subcommand
         * tokens. The partial being completed starts right after the final
         * space of that prefix.
         */
        WshShell_Size_t prefixEnd    = cmdPartLen;
        WshShell_Size_t prevTokBound = cmdPartLen;  /* lower bound for prevTok detection, saved before prefixEnd is extended */

#if WSH_SHELL_SUBCOMMANDS
        /*
         * Descend the subcommand tree using the tokens between the first
         * and last space. Flag tokens (starting with '-') are skipped along
         * with their argument values so global flags like `--mem int` don't
         * block descent into a subcommand that follows them.
         */
        WshShell_Size_t walkPos  = firstSpaceIdx;
        WshShell_Size_t flagsEnd = cmdPartLen;
        while (walkPos < lastSpaceIdx) {
            while (walkPos < lastSpaceIdx && pcInputCopyTrimmed[walkPos] == ' ')
                walkPos++;
            if (walkPos >= lastSpaceIdx)
                break;

            if (pcInputCopyTrimmed[walkPos] == '-') {
                /* skip flag name */
                WshShell_Size_t flagStart = walkPos;
                while (walkPos < lastSpaceIdx && pcInputCopyTrimmed[walkPos] != ' ')
                    walkPos++;
                WshShell_Char_t flagBuf[WSH_SHELL_OPTION_LONG_NAME_LEN] = {0};
                WshShell_Size_t fLen                                     = walkPos - flagStart;
                if (fLen >= WSH_SHELL_OPTION_LONG_NAME_LEN)
                    fLen = WSH_SHELL_OPTION_LONG_NAME_LEN - 1;
                WSH_SHELL_MEMCPY(flagBuf, pcInputCopyTrimmed + flagStart, fLen);
                /* skip ArgNum value tokens */
                const WshShellOption_t* pFlagOpt = WshShellCmd_FindOptByName(pcTargetCmd, flagBuf);
                WshShell_Size_t         skipArgs  = pFlagOpt ? pFlagOpt->ArgNum : 0;
                for (WshShell_Size_t sa = 0; sa < skipArgs && walkPos < lastSpaceIdx; sa++) {
                    while (walkPos < lastSpaceIdx && pcInputCopyTrimmed[walkPos] == ' ')
                        walkPos++;
                    while (walkPos < lastSpaceIdx && pcInputCopyTrimmed[walkPos] != ' ')
                        walkPos++;
                }
                flagsEnd = walkPos;
                continue;
            }

            WshShell_Size_t tokStart = walkPos;
            while (walkPos < lastSpaceIdx && pcInputCopyTrimmed[walkPos] != ' ')
                walkPos++;
            WshShell_Size_t tokLen = walkPos - tokStart;

            WshShell_Char_t tokBuf[WSH_SHELL_CMD_NAME_LEN] = {0};
            WshShell_Size_t copyLen = tokLen < (WSH_SHELL_CMD_NAME_LEN - 1) ? tokLen : (WSH_SHELL_CMD_NAME_LEN - 1);
            WSH_SHELL_MEMCPY(tokBuf, pcInputCopyTrimmed + tokStart, copyLen);
            tokBuf[copyLen] = '\0';

            const WshShellCmd_t* pcChild = WshShellCmd_SearchSubCmd(pcTargetCmd, tokBuf);
            if (!pcChild)
                break;

            pcTargetCmd = pcChild;
            prefixEnd   = walkPos;
            flagsEnd    = walkPos;
        }
        /* save original prefixEnd for prevTok detection, then extend it past any flags/values seen */
        prevTokBound = prefixEnd;
        if (flagsEnd > prefixEnd)
            prefixEnd = flagsEnd;
#endif /* WSH_SHELL_SUBCOMMANDS */

        const WshShell_Bool_t isFlagPartial = (flagPartLen == 0) || (pcFlagPart[0] == '-');
        WshShell_Size_t       effFlagPartLen = flagPartLen;

        if (!isFlagPartial) {
            /*
             * Check whether the token immediately before the last space is an
             * ENUM-type flag. If it is, pcFlagPart is a partial enum value —
             * complete it against the allowed list instead of subcommand names.
             *
             * Example: `wsh user list --format ta<TAB>`
             *   prevTok  = "--format"
             *   pcFlagPart = "ta"  →  completes to "table"
             */
            {
                WshShell_Size_t prevEnd = lastSpaceIdx;
                while (prevEnd > prevTokBound && pcInputCopyTrimmed[prevEnd - 1] == ' ')
                    prevEnd--;
                WshShell_Size_t prevStart = prevEnd;
                while (prevStart > prevTokBound && pcInputCopyTrimmed[prevStart - 1] != ' ')
                    prevStart--;

                if (prevStart < prevEnd) {
                    WshShell_Char_t prevTok[WSH_SHELL_OPTION_LONG_NAME_LEN] = {0};
                    WshShell_Size_t prevTokLen                              = prevEnd - prevStart;
                    if (prevTokLen > WSH_SHELL_OPTION_LONG_NAME_LEN - 1)
                        prevTokLen = WSH_SHELL_OPTION_LONG_NAME_LEN - 1;
                    WSH_SHELL_MEMCPY(prevTok, pcInputCopyTrimmed + prevStart, prevTokLen);

                    const WshShellOption_t* pPrevOpt = WshShellCmd_FindOptByName(pcTargetCmd, prevTok);
                    if (pPrevOpt && pPrevOpt->Type == WSH_SHELL_OPTION_ENUM && pPrevOpt->Enum) {
                        const WshShellOptionEnum_t* pEnum = pPrevOpt->Enum;
                        WshShell_Char_t valCandidates[WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES]
                                                     [WSH_SHELL_ENUM_VALUE_MAX_LEN];
                        WshShell_Size_t valMatchCount = 0;

                        for (WshShell_Size_t i = 0;
                             i < pEnum->Count && valMatchCount < WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES; i++) {
                            if (flagPartLen > 0 && WSH_SHELL_STRNCMP(pcFlagPart, pEnum->Values[i], flagPartLen) != 0)
                                continue;
                            WSH_SHELL_STRNCPY(valCandidates[valMatchCount], pEnum->Values[i],
                                              WSH_SHELL_ENUM_VALUE_MAX_LEN - 1);
                            valCandidates[valMatchCount][WSH_SHELL_ENUM_VALUE_MAX_LEN - 1] = '\0';
                            valMatchCount++;
                        }

                        if (valMatchCount == 0) {
                            WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
                            return false;
                        }

                        if (valMatchCount == 1) {
                            WshShell_Size_t valLen = WSH_SHELL_STRLEN(valCandidates[0]);
                            /* exact match + trailing space: user already finished this value, complete next */
                            if (hadTrailingSpace && valLen == flagPartLen &&
                                WSH_SHELL_STRNCMP(valCandidates[0], pcFlagPart, flagPartLen) == 0) {
                                prefixEnd      = inputCopyTrimmedLen;
                                effFlagPartLen = 0;
                            } else {
                                WshShell_Size_t totalLen = lastSpaceIdx + 1 + valLen;
                                if (totalLen + 2 > WSH_SHELL_INTR_BUFF_LEN) {
                                    WSH_SHELL_PRINT_WARN("Enum value too long for buffer\r\n");
                                    return false;
                                }
                                WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, lastSpaceIdx + 1);
                                WSH_SHELL_MEMCPY(pInBuff + lastSpaceIdx + 1, valCandidates[0], valLen);
                                pInBuff[totalLen]     = ' ';
                                pInBuff[totalLen + 1] = '\0';
                                return true;
                            }
                        } else {
                            WSH_SHELL_PRINT_SYS("%s /autocomplete values: ", sysMsgShift);
                            for (WshShell_Size_t i = 0; i < valMatchCount; i++)
                                WSH_SHELL_PRINT("[%s] ", valCandidates[i]);
                            WSH_SHELL_PRINT("\r\n");
                            return false;
                        }
                    } else if (pPrevOpt && pPrevOpt->ArgNum > 0 && hadTrailingSpace) {
                        /* pcFlagPart is a consumed INT/STR value; cursor is ready for the next flag */
                        WSH_SHELL_PRINT_SYS("%s /autocomplete flags found: ", sysMsgShift);
                        WshShell_Bool_t      anyFound = false;
                        const WshShellOption_t* pOpt  = pcTargetCmd->Options;
                        for (; pOpt && pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
                            if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                                continue;
                            anyFound = true;
                            WSH_SHELL_PRINT("[%s/%s] ", pOpt->ShortName, pOpt->LongName);
                        }
                        WSH_SHELL_PRINT("%s\r\n", anyFound ? "" : "none");
                        return false;
                    }
                }
            }
#if WSH_SHELL_SUBCOMMANDS
            /* Subcommand-name completion path. */
            if (pcTargetCmd->SubCmdNum == 0 || !pcTargetCmd->SubCmds) {
                WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
                return false;
            }

            WshShell_Char_t subCandidates[WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES][WSH_SHELL_CMD_NAME_LEN];
            WshShell_Size_t subMatchCount = 0;

            for (WshShell_Size_t i = 0;
                 i < pcTargetCmd->SubCmdNum && subMatchCount < WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES; i++) {
                const WshShellCmd_t* pcSub = pcTargetCmd->SubCmds[i];
                if (!pcSub || !pcSub->Name)
                    continue;
                if (effFlagPartLen > 0 && WSH_SHELL_STRNCMP(pcFlagPart, pcSub->Name, effFlagPartLen) != 0)
                    continue;

                WSH_SHELL_STRNCPY(subCandidates[subMatchCount], pcSub->Name, WSH_SHELL_CMD_NAME_LEN - 1);
                subCandidates[subMatchCount][WSH_SHELL_CMD_NAME_LEN - 1] = '\0';
                subMatchCount++;
            }

            if (subMatchCount == 0) {
                WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
                return false;
            }

            if (subMatchCount == 1) {
                WshShell_Size_t subLen = WSH_SHELL_STRLEN(subCandidates[0]);

                /*
                 * Exact match: user has already typed the full subcommand name.
                 * Instead of re-completing to the same text, list the subcommand's
                 * own options and children so the user can keep drilling down.
                 */
                if (subLen == flagPartLen && WSH_SHELL_STRNCMP(subCandidates[0], pcFlagPart, flagPartLen) == 0) {
                    const WshShellCmd_t* pcLeaf = WshShellCmd_SearchSubCmd(pcTargetCmd, subCandidates[0]);
                    if (pcLeaf) {
                        WSH_SHELL_PRINT_SYS("%s /autocomplete flags found: ", sysMsgShift);
                        WshShell_Bool_t anyFound     = false;
                        const WshShellOption_t* pOpt = pcLeaf->Options;
                        for (; pOpt && pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
                            if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                                continue;
                            anyFound = true;
                            WSH_SHELL_PRINT("[%s/%s] ", pOpt->ShortName, pOpt->LongName);
                        }
                        if (pcLeaf->SubCmdNum > 0 && pcLeaf->SubCmds != NULL) {
                            for (WshShell_Size_t i = 0; i < pcLeaf->SubCmdNum; i++) {
                                const WshShellCmd_t* pcSub = pcLeaf->SubCmds[i];
                                if (!pcSub || !pcSub->Name)
                                    continue;
                                anyFound = true;
                                WSH_SHELL_PRINT("[%s] ", pcSub->Name);
                            }
                        }
                        WSH_SHELL_PRINT("%s\r\n", anyFound ? "" : "none");
                        return false;
                    }
                }

                WshShell_Size_t newLen = prefixEnd + 1 + subLen;
                if (newLen + 2 > WSH_SHELL_INTR_BUFF_LEN) {
                    WSH_SHELL_PRINT_WARN("Subcommand name too long for buffer\r\n");
                    return false;
                }
                WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, prefixEnd);
                pInBuff[prefixEnd] = ' ';
                WSH_SHELL_MEMCPY(pInBuff + prefixEnd + 1, subCandidates[0], subLen);
                pInBuff[prefixEnd + 1 + subLen]     = ' ';
                pInBuff[prefixEnd + 1 + subLen + 1] = '\0';
                return true;
            }

            WshShell_Size_t subPrefixLen = WshShellAutocomplete_CommonPrefixLen(
                subCandidates[0], WSH_SHELL_CMD_NAME_LEN, subMatchCount, effFlagPartLen, WSH_SHELL_CMD_NAME_LEN);

            if (subPrefixLen > flagPartLen) {
                WshShell_Size_t newLen = prefixEnd + 1 + subPrefixLen;
                if (newLen + 1 > WSH_SHELL_INTR_BUFF_LEN) {
                    WSH_SHELL_PRINT_WARN("Subcommand prefix too long for buffer\r\n");
                    return false;
                }
                WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, prefixEnd);
                pInBuff[prefixEnd] = ' ';
                WSH_SHELL_MEMCPY(pInBuff + prefixEnd + 1, subCandidates[0], subPrefixLen);
                pInBuff[newLen] = '\0';
                return true;
            }

            WSH_SHELL_PRINT_SYS("%s /autocomplete subcmds found: ", sysMsgShift);
            for (WshShell_Size_t cnt = 0; cnt < subMatchCount; cnt++)
                WSH_SHELL_PRINT("[%s] ", subCandidates[cnt]);
            WSH_SHELL_PRINT("\r\n");
            return false;
#else  /* WSH_SHELL_SUBCOMMANDS */
            WSH_SHELL_PRINT_SYS("%s /autocomplete: no matches\r\n", sysMsgShift);
            return false;
#endif /* WSH_SHELL_SUBCOMMANDS */
        }

        // Collect flags matching partial flag token
        WshShell_Char_t flagCandidates[WSH_SHELL_CMD_OPTIONS_MAX_NUM][WSH_SHELL_OPTION_LONG_NAME_LEN];
        WshShell_Size_t flagMatchCount = 0;

        const WshShellOption_t* pOpt = pcTargetCmd->Options;
        for (; pOpt && pOpt->Type != WSH_SHELL_OPTION_END && flagMatchCount < WSH_SHELL_CMD_OPTIONS_MAX_NUM; pOpt++) {
            if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                continue;

            const WshShell_Char_t* pcName = NULL;
            if (pOpt->LongName && pOpt->LongName[0] &&
                WSH_SHELL_STRNCMP(pcFlagPart, pOpt->LongName, flagPartLen) == 0) {
                pcName = pOpt->LongName;
            } else if (flagPartLen <= WSH_SHELL_OPTION_SHORT_NAME_LEN && pOpt->ShortName && pOpt->ShortName[0] &&
                       WSH_SHELL_STRNCMP(pcFlagPart, pOpt->ShortName, flagPartLen) == 0) {
                pcName = pOpt->ShortName;
            }

            if (pcName) {
                WSH_SHELL_STRNCPY(flagCandidates[flagMatchCount], pcName, WSH_SHELL_OPTION_LONG_NAME_LEN - 1);
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
            /*
             * Exact flag match + cursor already past the flag (trailing space):
             * show all allowed enum values so the user can choose one.
             *
             * Example: `wsh user list --format <TAB>`
             *   flagCandidates[0] = "--format", flagLen == flagPartLen, hadTrailingSpace
             */
            if (flagLen == flagPartLen && hadTrailingSpace) {
                const WshShellOption_t* pEnumOpt = WshShellCmd_FindOptByName(pcTargetCmd, flagCandidates[0]);
                if (pEnumOpt && pEnumOpt->Type == WSH_SHELL_OPTION_ENUM && pEnumOpt->Enum) {
                    WSH_SHELL_PRINT_SYS("%s /autocomplete values: ", sysMsgShift);
                    for (WshShell_Size_t i = 0; i < pEnumOpt->Enum->Count; i++)
                        WSH_SHELL_PRINT("[%s] ", pEnumOpt->Enum->Values[i]);
                    WSH_SHELL_PRINT("\r\n");
                    return false;
                }
            }
            WshShell_Size_t newLen = prefixEnd + 1 + flagLen;
            if (newLen + 2 > WSH_SHELL_INTR_BUFF_LEN) {
                WSH_SHELL_PRINT_WARN("Flag name too long for buffer\r\n");
                return false;
            }
            WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, prefixEnd);
            pInBuff[prefixEnd] = ' ';
            WSH_SHELL_MEMCPY(pInBuff + prefixEnd + 1, flagCandidates[0], flagLen);
            pInBuff[prefixEnd + 1 + flagLen]     = ' ';
            pInBuff[prefixEnd + 1 + flagLen + 1] = '\0';
            return true;
        }

        // Find longest common prefix among flag candidates
        WshShell_Size_t flagPrefixLen =
            WshShellAutocomplete_CommonPrefixLen(flagCandidates[0], WSH_SHELL_OPTION_LONG_NAME_LEN, flagMatchCount,
                                                 flagPartLen, WSH_SHELL_OPTION_LONG_NAME_LEN);

        if (flagPrefixLen > flagPartLen) {
            WshShell_Size_t newLen = prefixEnd + 1 + flagPrefixLen;
            if (newLen + 1 > WSH_SHELL_INTR_BUFF_LEN) {
                WSH_SHELL_PRINT_WARN("Flag prefix too long for buffer\r\n");
                return false;
            }
            WSH_SHELL_STRNCPY(pInBuff, pcInputCopyTrimmed, prefixEnd);
            pInBuff[prefixEnd] = ' ';
            WSH_SHELL_MEMCPY(pInBuff + prefixEnd + 1, flagCandidates[0], flagPrefixLen);
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
            WshShell_Bool_t anyFound     = false;
            if (pOpt != NULL) {
                for (; pOpt->Type != WSH_SHELL_OPTION_END; pOpt++) {
                    if (pOpt->Type == WSH_SHELL_OPTION_NO || pOpt->Type == WSH_SHELL_OPTION_WAITS_INPUT)
                        continue;

                    anyFound = true;
                    WSH_SHELL_PRINT("[%s/%s] ", pOpt->ShortName, pOpt->LongName);
                }
            }

#if WSH_SHELL_SUBCOMMANDS
            if (pcCmdMatch->SubCmdNum > 0 && pcCmdMatch->SubCmds != NULL) {
                for (WshShell_Size_t i = 0; i < pcCmdMatch->SubCmdNum; i++) {
                    const WshShellCmd_t* pcSub = pcCmdMatch->SubCmds[i];
                    if (!pcSub || !pcSub->Name)
                        continue;
                    anyFound = true;
                    WSH_SHELL_PRINT("[%s] ", pcSub->Name);
                }
            }
#endif

            WSH_SHELL_PRINT("%s\r\n", anyFound ? "" : "none");
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
    WshShell_Size_t prefixLen = WshShellAutocomplete_CommonPrefixLen(candidates[0], WSH_SHELL_CMD_NAME_LEN, matchCount,
                                                                     inBuffLen, WSH_SHELL_CMD_NAME_LEN);

    // If common prefix is longer than input, extend input
    if (prefixLen > inBuffLen) {
        // Check if prefix fits in buffer
        if (prefixLen + 1 > WSH_SHELL_INTR_BUFF_LEN) {
            WSH_SHELL_PRINT_WARN("Common prefix too long for buffer\r\n");
            return false;
        }
        WSH_SHELL_MEMCPY((void*)&pInBuff[inBuffLen], (void*)&candidates[0][inBuffLen], prefixLen - inBuffLen);
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
    WshShell_Size_t padLen =
        (inBuffLen < WSH_SHELL_AUTOCOMPLETE_PAD_LEN) ? (WSH_SHELL_AUTOCOMPLETE_PAD_LEN - inBuffLen) : 0;
    WSH_SHELL_MEMSET((void*)sysMsgShift, WSH_SHELL_AUTOCOMPLETE_PAD_SYM, padLen);
    sysMsgShift[0]      = ' ';
    sysMsgShift[padLen] = '\0';

    WSH_SHELL_PRINT_WARN("%s /autocomplete disabled", sysMsgShift);
    WSH_SHELL_PRINT("\r\n");

    return false;
}

#endif /* WSH_SHELL_AUTOCOMPLETE */
