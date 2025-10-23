#include "wsh_shell_str.h"

WshShell_Bool_t WshShellStr_IsPrintableAscii(WshShell_Char_t ch) {
    return (bool)(ch >= ' ' && ch <= '~');  // ASCII printable range
}

WshShell_Char_t* WshShellStr_TrimString(WshShell_Char_t* pString, WshShell_Size_t len) {
    WSH_SHELL_ASSERT(pString);
    if (!pString)
        return pString;

    WshShell_Char_t* pStrBegin = pString;
    while (((*pStrBegin == ' ') || (*pStrBegin == '\t')) && (pStrBegin < (pString + len))) {
        pStrBegin++;
    }

    WshShell_Char_t* pStrEnd = pString + len - 1;
    while ((pStrEnd >= pStrBegin) && (*pStrEnd == ' ' || *pStrEnd == '\t')) {
        *pStrEnd = 0;
        pStrEnd--;
    }

    return pStrBegin;
}

void WshShellStr_ParseToArgcArgv(WshShell_Char_t* pStr, WshShell_Size_t* pArgNum,
                                 const WshShell_Char_t* pArgBuff[], WshShell_Size_t maxArgNum) {
    WSH_SHELL_ASSERT(pStr && pArgNum && pArgBuff && maxArgNum > 0);
    if (!pStr || !pArgNum || !pArgBuff || maxArgNum == 0)
        return;

    *pArgNum                 = 0;
    WshShell_Bool_t inQuotes = false;
    WshShell_Size_t len      = WSH_SHELL_STRLEN(pStr);

    for (WshShell_Size_t idx = 0; idx < len; idx++) {
        if (pStr[idx] == '\"') {
            inQuotes  = !inQuotes;
            pStr[idx] = '\0';
        } else if (!inQuotes && pStr[idx] == ' ') {
            pStr[idx] = '\0';
        }
    }

    // Extract pointers to tokens
    for (WshShell_Size_t idx = 0; idx < len;) {
        if (pStr[idx] != '\0') {
            if (*pArgNum < maxArgNum) {
                pArgBuff[(*pArgNum)++] = &pStr[idx];
            }

            // Skip until next '\0'
            while (idx < len && pStr[idx] != '\0')
                idx++;
        } else {
            idx++;
        }
    }
}

void WshShellStr_IncrInterCnt(WshShell_Size_t* pInterCnt, WshShell_Size_t buffSize) {
    WSH_SHELL_ASSERT(pInterCnt && buffSize > 0);
    if (!pInterCnt || buffSize == 0)
        return;

    if (*pInterCnt < buffSize - 1)
        (*pInterCnt)++;
}

void WshShellStr_DecrInterCnt(WshShell_Size_t* pInterCnt) {
    WSH_SHELL_ASSERT(pInterCnt);
    if (!pInterCnt)
        return;

    if (*pInterCnt > 0)
        (*pInterCnt)--;
}

void WshShellStr_AccessBitsToStr(WshShell_Size_t access, WshShell_Char_t* pOutStr) {
    WSH_SHELL_ASSERT(pOutStr);
    if (!pOutStr)
        return;

    pOutStr[0] = (access & WSH_SHELL_OPT_ACCESS_READ) ? 'r' : '-';
    pOutStr[1] = (access & WSH_SHELL_OPT_ACCESS_WRITE) ? 'w' : '-';
    pOutStr[2] = (access & WSH_SHELL_OPT_ACCESS_EXECUTE) ? 'x' : '-';
    pOutStr[3] = (access == WSH_SHELL_OPT_ACCESS_ADMIN) ? 'A' : '-';
    pOutStr[4] = '\0';
}

void WshShellStr_GroupBitsToStr(WshShell_Size_t group, WshShell_Size_t groupMaxNum,
                                WshShell_Char_t* pOutStr) {
    WSH_SHELL_ASSERT(pOutStr);
    if (!pOutStr)
        return;

    for (WshShell_S32_t groupIdx = groupMaxNum - 1; groupIdx >= 0; --groupIdx) {
        *pOutStr++ = (group == WSH_SHELL_CMD_GROUP_ALL || (group & (1U << groupIdx))) ? '*' : '-';
    }
    *pOutStr = '\0';
}
