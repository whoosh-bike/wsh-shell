#include "wsh_shell_str.h"

#define WSH_PROMPT_SPACE_LEFT(out, base) ((out) - (base) < (WSH_SHELL_PROMPT_MAX_LEN - 1))

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

#if WSH_SHELL_CUSTOM_PROMPT

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

void WshShellStr_GeneratePrompt(WshShell_Char_t* pPrompt, WshShellStr_PromptData_t* pPromptData) {
    WSH_SHELL_ASSERT(pPrompt && pPromptData->UserName && pPromptData->DevName);
    if (!pPrompt || !pPromptData->UserName || !pPromptData->DevName)
        return;

    const WshShell_Size_t colorMapSize = WSH_SHELL_ARR_LEN(WshShellStr_ColorMap);
    WshShell_Char_t* pOut              = pPrompt;
    const WshShell_Char_t* pcIn        = WSH_SHELL_PROMPT_TEMPLATE;

    while (*pcIn && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt)) {
        if (*pcIn == '%') {
            ++pcIn;
            if (!*pcIn)
                break;

            if (*pcIn == 'u') {  // User name
                while (*pPromptData->UserName && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = *pPromptData->UserName++;
            } else if (*pcIn == 'd') {  // Device name
                while (*pPromptData->DevName && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = *pPromptData->DevName++;
            } else if (*pcIn == 'c') {  // Color choice
                ++pcIn;
                if (*pcIn >= '0' && *pcIn <= '9') {  // Valid colors range
                    WshShell_Size_t idx = *pcIn - '0';
                    const WshShell_Char_t* pcColor =
                        (idx < colorMapSize) ? WshShellStr_ColorMap[idx] : "";

                    while (*pcColor && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                        *pOut++ = *pcColor++;
                }
            } else if (*pcIn == 'r') {  // Reset style
                const WshShell_Char_t* pcReset = WSH_SHELL_ESC_RESET_STYLE;
                while (*pcReset && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = *pcReset++;
            } else if (*pcIn == 'b') {  // Bold style
                const WshShell_Char_t* pcBold = WSH_SHELL_ECS_SET_MODE_BOLD;
                while (*pcBold && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = *pcBold++;
            } else if (*pcIn == 'i') {  // Interactive command name
                if (pPromptData->InterCmdName && *pPromptData->InterCmdName) {
                    if (WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                        *pOut++ = ' ';

                    if (WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                        *pOut++ = '(';

                    const WshShell_Char_t* pcInter = pPromptData->InterCmdName;
                    while (*pcInter && WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                        *pOut++ = *pcInter++;

                    if (WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                        *pOut++ = ')';
                }
            } else {
                if (WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = '%';
                if (WSH_PROMPT_SPACE_LEFT(pOut, pPrompt))
                    *pOut++ = *pcIn;
            }
        } else {
            *pOut++ = *pcIn;
        }
        ++pcIn;
    }

    *pOut = '\0';
}

#else /* WSH_SHELL_CUSTOM_PROMPT */

void WshShellStr_GeneratePrompt(WshShell_Char_t* pPrompt, WshShellStr_PromptData_t* pPromptData) {
    WSH_SHELL_ASSERT(pPrompt && pPromptData->UserName && pPromptData->DevName);
    if (!pPrompt || !pPromptData->UserName || !pPromptData->DevName)
        return;

    WSH_SHELL_SNPRINTF(pPrompt, WSH_SHELL_PROMPT_MAX_LEN,
                       WSH_SHELL_ESC_RESET_STYLE WSH_SHELL_ECS_SET_MODE_BOLD WSH_SHELL_COLOR_CYAN
                       "%s" WSH_SHELL_COLOR_WHITE "@" WSH_SHELL_COLOR_PURPLE
                       "%s" WSH_SHELL_COLOR_WHITE " > " WSH_SHELL_ESC_RESET_STYLE,
                       pPromptData->UserName, pPromptData->DevName);
}

#endif /* WSH_SHELL_CUSTOM_PROMPT */
