#include "wsh_shell_history.h"

#if WSH_SHELL_HISTORY

static WshShell_U32_t WshShellHistory_CRC32(const WshShell_U8_t* pBuff, size_t len) {
    WshShell_U32_t crc_table[256];
    WshShell_U32_t crc;

    for (WshShell_S32_t i = 0; i < 256; i++) {
        crc = i;
        for (WshShell_S32_t j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    };

    crc = 0xFFFFFFFFUL;

    while (len--)
        crc = crc_table[(crc ^ *pBuff++) & 0xFF] ^ (crc >> 8);

    return crc ^ 0xFFFFFFFFUL;
}

__attribute__((weak)) void WshShellHistory_Write(WshShellHistory_t history) {
}

__attribute__((weak)) WshShellHistory_t WshShellHistory_Read(void) {
    WshShellHistory_t localHistory = {0};

    return localHistory;
}

void WshShellHistory_Init(void) {
    WshShellHistory_t extHistory = WshShellHistory_Read();
    WshShell_U32_t extCrc =
        WshShellHistory_CRC32((WshShell_U8_t*)&extHistory.Data.pBuffer, WSH_SHELL_HISTORY_BUFF_SIZE);
    if (extCrc != extHistory.CRC) {
        WshShellHistory_t localHistory = {0};
        localHistory.CRC =
            WshShellHistory_CRC32((WshShell_U8_t*)&localHistory.Data.pBuffer, WSH_SHELL_HISTORY_BUFF_SIZE);
        WshShellHistory_Write(localHistory);
    }
}

static void WshShellHistory_IncrIdx(WshShell_Size_t* pDataIdx) {
    if (++(*pDataIdx) > WSH_SHELL_HISTORY_BUFF_SIZE - 1)
        *pDataIdx = 0;
}

static void WshShellHistory_DecrIdx(WshShell_Size_t* pDataIdx) {
    if (--(*pDataIdx) < 0)
        *pDataIdx = WSH_SHELL_HISTORY_BUFF_SIZE - 1;
}

static void WshShellHistory_GetTokenFromBuffer(char* pWriteBuff, const char* pReadBuff, WshShell_Size_t startIdx,
                                               WshShell_Size_t charsToRead) {
    WshShell_Size_t readIdx  = startIdx;
    WshShell_Size_t writeIdx = 0;
    while (writeIdx < charsToRead) {
        pWriteBuff[writeIdx] = pReadBuff[readIdx];
        WshShellHistory_IncrIdx(&readIdx);
        writeIdx++;
    }
    pWriteBuff[writeIdx] = '\0';
}

void WshShellHistory_SaveCmd(const char* pCmdStr, WshShell_Size_t cmdStrLen) {
    WSH_SHELL_ASSERT(cmdStrLen != 0);
    WSH_SHELL_ASSERT(pCmdStr != NULL);
    if (pCmdStr == NULL || cmdStrLen == 0)
        return;

    WshShellHistory_t localHistory = WshShellHistory_Read();

    /* If there was exact same command called previously => return */
    if (WSH_SHELL_STRNCMP(pCmdStr, &localHistory.Data.pBuffer[localHistory.Data.LastSavedCmdIdx], cmdStrLen) == 0)
        return;

    localHistory.Data.LastSavedCmdIdx = localHistory.Data.HeadIdx;
    for (WshShell_Size_t i = 0; i < cmdStrLen; i++) {
        localHistory.Data.pBuffer[localHistory.Data.HeadIdx] = pCmdStr[i];
        WshShellHistory_IncrIdx(&localHistory.Data.HeadIdx);
    }
    localHistory.Data.pBuffer[localHistory.Data.HeadIdx] = '\0';
    WshShellHistory_IncrIdx(&localHistory.Data.HeadIdx);
    localHistory.Data.TailIdx = localHistory.Data.HeadIdx;
    WshShellHistory_Write(localHistory);
}

static WshShell_Size_t WshShellHistory_GetPrevToken(WshShellHistory_t* pHistory, WshShell_Size_t* pTokenIdx) {
    WshShell_Size_t localTail = pHistory->Data.TailIdx;

    /*
	 * One step back and check two conditions:
	 * 1) There must be '\0' because command tokens are separated by this symbol
	 * 2) It does not equals to head of our buffer.
	 * If it does => we have performed a loop so there are no more saved commands.
	 */
    WshShellHistory_DecrIdx(&localTail);
    if (pHistory->Data.pBuffer[localTail] != '\0' || localTail == pHistory->Data.HeadIdx)
        return 0;

    /*
	 * One more step back and check two conditions:
	 * 1) There SHOULDN'T be '\0' because. If it does => we have reached the end of the buffer.
	 * 2) It does not equals to head of our buffer.
	 */
    WshShellHistory_DecrIdx(&localTail);
    if (pHistory->Data.pBuffer[localTail] == '\0' || localTail == pHistory->Data.HeadIdx)
        return 0;

    /*
	 * Token can be placed at the end or beginnig of ur buffer.
	 * That means that we can't use strlen => we need to count size manualy.
	 */
    WshShell_Size_t tokenSize = 0;

    /*
	 * Now we know that there can be at least one more token.
	 * Lets try to step back to its beginning.
	 */
    while (pHistory->Data.pBuffer[localTail] != '\0') {
        WshShellHistory_DecrIdx(&localTail);
        tokenSize++;
        if (localTail == pHistory->Data.HeadIdx)
            return 0;
    }
    WshShellHistory_IncrIdx(&localTail);

    /*
	 * From know we know that there IS a valid token and pLocalTail points to its beginning.
	 */
    pHistory->Data.TailIdx = localTail;
    *pTokenIdx             = localTail;
    return tokenSize;
}

static WshShell_Size_t WshShellHistory_GetNextToken(WshShellHistory_t* pHistory, WshShell_Size_t* pTokenIdx) {
    WshShell_Size_t localTail = pHistory->Data.TailIdx;
    WshShell_Size_t startIdx  = pHistory->Data.TailIdx;
    WshShell_Size_t tokenSize = 0;
    if (localTail == pHistory->Data.HeadIdx)
        return tokenSize;

    /*
	 * If pTail != pHead then there should be at least one token.
	 * Lets try to reach it.
	 */
    while (pHistory->Data.pBuffer[localTail] != '\0') {
        WshShellHistory_IncrIdx(&localTail);
        tokenSize++;
        if (localTail == pHistory->Data.HeadIdx)
            return 0;
    }

    WshShellHistory_IncrIdx(&localTail);
    tokenSize++;
    // if (localTail == pHistory->Data.HeadIdx)
    //     return 0;

    /*
	 * From know we know that there IS a valid token and pLocalTail points to its beginning.
	 */
    pHistory->Data.TailIdx = localTail;
    *pTokenIdx             = startIdx;
    return tokenSize;
}

/*                                          locTail == CmdHistoryTail -> moving right to 0 if incrIsForward
											  |*|       ->      {*}
CmdHistoryBuffer: |c|m|d| 0 |p|r|e|v|c|m|d| 0 |t|a|i|l|c|m|d| 0 |n|e|x|t|c|m|d| 0 |
							{*}   <-    |*|
	 moving left if !incrIsForward <- locTail
*/
WshShell_Size_t WshShellHistory_GetPrevCmd(char* pOutBuff, WshShell_Size_t buffSize) {
    WSH_SHELL_ASSERT(buffSize != 0);
    WSH_SHELL_ASSERT(pOutBuff != NULL);
    if (buffSize == 0 || pOutBuff == NULL)
        return 0;

    WshShellHistory_t localHistory = WshShellHistory_Read();
    WshShell_Size_t prevCmdIdx     = 0;
    WshShell_Size_t prevCmdStrLen  = WshShellHistory_GetPrevToken(&localHistory, &prevCmdIdx);

    if (prevCmdStrLen != 0 && prevCmdStrLen + 1 < buffSize)
        WshShellHistory_GetTokenFromBuffer(pOutBuff, localHistory.Data.pBuffer, prevCmdIdx, prevCmdStrLen);
    WshShellHistory_Write(localHistory);

    return prevCmdStrLen;
}

WshShell_Size_t WshShellHistory_GetNextCmd(char* pOutBuff, WshShell_Size_t buffSize) {
    WSH_SHELL_ASSERT(buffSize != 0);
    WSH_SHELL_ASSERT(pOutBuff != NULL);
    if (buffSize == 0 || pOutBuff == NULL)
        return 0;

    WshShellHistory_t localHistory = WshShellHistory_Read();
    WshShell_Size_t nextCmdIdx     = 0;
    WshShell_Size_t nextCmdStrLen  = WshShellHistory_GetNextToken(&localHistory, &nextCmdIdx);

    if (nextCmdStrLen != 0 && nextCmdStrLen + 1 < buffSize)
        WshShellHistory_GetTokenFromBuffer(pOutBuff, localHistory.Data.pBuffer, nextCmdIdx, nextCmdStrLen);
    else
        WSH_SHELL_MEMSET(pOutBuff, '\0', buffSize);
    WshShellHistory_Write(localHistory);

    return nextCmdStrLen;
}

void WshShellHistory_Flush(void) {
    WshShellHistory_t localHistory = (WshShellHistory_t){0};
    localHistory.CRC = WshShellHistory_CRC32((WshShell_U8_t*)&localHistory.Data.pBuffer, WSH_SHELL_HISTORY_BUFF_SIZE);
    WshShellHistory_Write(localHistory);
}

#else  /* WSH_SHELL_HISTORY */

void WshShellHistory_Init(void) {
}

void WshShellHistory_SaveCmd(const char* pCmdStr, WshShell_Size_t cmdStrLen) {
}

WshShell_Size_t WshShellHistory_GetPrevCmd(char* pOutBuff, WshShell_Size_t buffSize) {
    return 0;
}

WshShell_Size_t WshShellHistory_GetNextCmd(char* pOutBuff, WshShell_Size_t buffSize) {
    return 0;
}

void WshShellHistory_Flush(void) {
}
#endif /* WSH_SHELL_HISTORY */
