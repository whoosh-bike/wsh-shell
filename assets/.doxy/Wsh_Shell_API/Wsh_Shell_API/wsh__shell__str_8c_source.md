

# File wsh\_shell\_str.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_str.c**](wsh__shell__str_8c.md)

[Go to the documentation of this file](wsh__shell__str_8c.md)


```C++
#include "wsh_shell_str.h"

#define WSH_PS1_SPACE_LEFT(out, base) ((out) - (base) < (WSH_SHELL_PS1_MAX_LEN - 1))

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
    pOutStr[3] = (access == WSH_SHELL_OPT_ACCESS_ANY) ? 'A' : '\0';
    pOutStr[4] = '\0';
}

void WshShellStr_GroupBitsToStr(WshShell_Size_t group, WshShell_Char_t* pOutStr) {
    WSH_SHELL_ASSERT(pOutStr);
    if (!pOutStr)
        return;

    for (WshShell_S32_t groupIdx = WSH_SHELL_CMD_GROUP_MAX_COUNT - 1; groupIdx >= 0; --groupIdx) {
        *pOutStr++ = (group == WSH_SHELL_CMD_GROUP_ALL || (group & (1U << groupIdx))) ? '*' : '-';
    }
    *pOutStr = '\0';
}

#if WSH_SHELL_PS1_CUSTOM

static const WshShell_Char_t* WshShellStr_ColorMap[] = {
    WSH_SHELL_COLOR_BLACK,   // %c0
    WSH_SHELL_COLOR_RED,     // %c1
    WSH_SHELL_COLOR_GREEN,   // %c2
    WSH_SHELL_COLOR_YELLOW,  // %c3
    WSH_SHELL_COLOR_BLUE,    // %c4
    WSH_SHELL_COLOR_PURPLE,  // %c5
    WSH_SHELL_COLOR_CYAN,    // %c6
    WSH_SHELL_COLOR_WHITE    // %c7
};

void WshShellStr_GeneratePS1(WshShell_Char_t* pPS1, WshShellStr_PS1Data_t* pPS1Data) {
    WSH_SHELL_ASSERT(pPS1 && pPS1Data->UserName && pPS1Data->DevName);
    if (!pPS1 || !pPS1Data->UserName || !pPS1Data->DevName)
        return;

    const WshShell_Size_t colorMapSize = WSH_SHELL_ARR_LEN(WshShellStr_ColorMap);
    WshShell_Char_t* pOut              = pPS1;
    const WshShell_Char_t* pcIn        = WSH_SHELL_PS1_TEMPLATE;
    const WshShell_Char_t* pcCurrColor = "";

    while (*pcIn && WSH_PS1_SPACE_LEFT(pOut, pPS1)) {
        if (*pcIn == '%') {
            ++pcIn;
            if (!*pcIn)
                break;

            if (*pcIn == 'u') {  // User name
                const WshShell_Char_t* pcUser = pPS1Data->UserName;
                while (*pcUser && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcUser++;
            } else if (*pcIn == 'd') {  // Device name
                const WshShell_Char_t* pcDev = pPS1Data->DevName;
                while (*pcDev && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcDev++;
            } else if (*pcIn == 'c') {  // Color choice
                ++pcIn;
                if (*pcIn >= '0' && *pcIn <= '9') {  // Valid colors range
                    WshShell_Size_t idx = *pcIn - '0';
                    const WshShell_Char_t* pcColor =
                        (idx < colorMapSize) ? WshShellStr_ColorMap[idx] : "";

                    pcCurrColor = pcColor;

                    while (*pcColor && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcColor++;
                }
            } else if (*pcIn == 'r') {  // Reset style
                const WshShell_Char_t* pcReset = WSH_SHELL_ESC_RESET_STYLE;
                while (*pcReset && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcReset++;
            } else if (*pcIn == 'b') {  // Bold style
                const WshShell_Char_t* pcBold = WSH_SHELL_ECS_SET_MODE_BOLD;
                while (*pcBold && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcBold++;
            } else if (*pcIn == 'i') {  // Interactive command name
                if (pPS1Data->InterCmdName && *pPS1Data->InterCmdName) {
                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = ' ';

                    const WshShell_Char_t* pcWhite = WSH_SHELL_COLOR_WHITE;
                    while (*pcWhite && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcWhite++;

                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = '(';

                    const WshShell_Char_t* pcColor = pcCurrColor;
                    while (*pcColor && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcColor++;

                    const WshShell_Char_t* pcInter = pPS1Data->InterCmdName;
                    while (*pcInter && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcInter++;

                    pcWhite = WSH_SHELL_COLOR_WHITE;
                    while (*pcWhite && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcWhite++;

                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = ')';
                }
            } else {
                if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = '%';
                if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcIn;
            }
        } else {
            *pOut++ = *pcIn;
        }
        ++pcIn;
    }

    *pOut = '\0';
}

#else /* WSH_SHELL_PS1_CUSTOM */

void WshShellStr_GeneratePS1(WshShell_Char_t* pPS1, WshShellStr_PS1Data_t* pPS1Data) {
    WSH_SHELL_ASSERT(pPS1 && pPS1Data->UserName && pPS1Data->DevName);
    if (!pPS1 || !pPS1Data->UserName || !pPS1Data->DevName)
        return;

    const WshShell_Char_t* pcInterCmd = "";

    if (pPS1Data->InterCmdName && *pPS1Data->InterCmdName) {
        static WshShell_Char_t interCmdBuff[2 * WSH_SHELL_CMD_NAME_LEN];
        WSH_SHELL_SNPRINTF(interCmdBuff, sizeof(interCmdBuff),
                           WSH_SHELL_COLOR_WHITE " (" WSH_SHELL_COLOR_GREEN
                                                 "%s" WSH_SHELL_COLOR_WHITE ")",
                           pPS1Data->InterCmdName);
        pcInterCmd = interCmdBuff;
    }

    WSH_SHELL_SNPRINTF(pPS1, WSH_SHELL_PS1_MAX_LEN,
                       WSH_SHELL_ESC_RESET_STYLE WSH_SHELL_ECS_SET_MODE_BOLD WSH_SHELL_COLOR_CYAN
                       "%s" WSH_SHELL_COLOR_WHITE "@" WSH_SHELL_COLOR_PURPLE "%s"
                       "%s"  // pcInterCmd
                       WSH_SHELL_COLOR_WHITE " > " WSH_SHELL_ESC_RESET_STYLE,
                       pPS1Data->UserName, pPS1Data->DevName, pcInterCmd);
}

#endif /* WSH_SHELL_PS1_CUSTOM */
```


