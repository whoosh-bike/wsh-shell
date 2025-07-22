#include "wsh_shell_str.h"
#include "wsh_shell_cfg.h"

char* WshShellStr_TrimString(char* pString, WshShell_Size_t length) {
    char* pStrBegin = pString;
    while (((*pStrBegin == ' ') || (*pStrBegin == '\t')) && (pStrBegin < (pString + length))) {
        pStrBegin++;
    }

    char* pStrEnd = pString + length - 1;
    while (((*pStrEnd == ' ') || (*pStrEnd == '\t')) && (pStrEnd > pString)) {
        *pStrEnd = 0;
        pStrEnd--;
    }
    return pStrBegin;
}

void WshShellStr_ParseToArgcArgv(char* pStr, WshShell_Size_t* pArgNum, const char** ppArgBuff) {
    WshShell_Size_t strLen = WSH_SHELL_STRLEN(pStr);
    bool inString          = pStr[0] == '\"' ? true : false;
    if (inString)
        pStr[0] = '\0';

    for (WshShell_Size_t idx = 0; idx < strLen; idx++) {
        if (pStr[idx] == ' ' && inString == false) {
            if (pStr[idx + 1] == '\"') {
                pStr[idx] = pStr[idx + 1] = '\0';
                inString                  = true;
                continue;
            }
        }

        if (pStr[idx] == '\"' && inString == true) {
            if (pStr[idx + 1] == ' ' || pStr[idx + 1] == '\0') {
                pStr[idx] = pStr[idx + 1] = '\0';
                inString                  = false;
                continue;
            }
        }

        if (pStr[idx] == ' ' && inString == false) {
            pStr[idx] = '\0';
            continue;
        }
    }

    for (WshShell_Size_t idx = 0; idx < strLen; idx++) {
        if (pStr[idx] != '\0') {
            ppArgBuff[(*pArgNum)++] = &pStr[idx];
            while (pStr[idx + 1] != '\0')
                idx++;
        }
    }
}

void WshShellStr_IncrInterCnt(WshShell_Size_t* pInterCnt, WshShell_Size_t buffSize) {
    if (++(*pInterCnt) > (buffSize - 2))
        *pInterCnt = (buffSize - 2);
}

void WshShellStr_DecrInterCnt(WshShell_Size_t* pInterCnt) {
    if (*pInterCnt > 0)
        (*pInterCnt)--;
    else
        *pInterCnt = 0;
}
