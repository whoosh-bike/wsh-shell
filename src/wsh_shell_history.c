#include "wsh_shell_history.h"

#if WSH_SHELL_HISTORY

static void WshShellHistory_CalcHashAndWrite(WshShellHistory_IO_t* pHistIO,
                                             WshShellHistory_t history) {
    WSH_SHELL_ASSERT(pHistIO->Write);
    if (!pHistIO->Write)
        return;

    history.Hash =
        WshShellMisc_CalcJenkinsHash((WshShell_U8_t*)&history.Data, sizeof(history.Data));
    pHistIO->Write(history);
    // WSH_SHELL_PRINT("\r\nh %d, t %d, l %d\r\n", history.Data.HeadIdx, history.Data.TailIdx,
    //                 history.Data.LastSavedCmdIdx);
}

void WshShellHistory_Init(WshShellHistory_IO_t* pHistIO, WshShellHistory_ReadHandler_t readFn,
                          WshShellHistory_WriteHandler_t writeFn) {
    WSH_SHELL_ASSERT(pHistIO && readFn && writeFn);
    if (!pHistIO || !readFn || !writeFn)
        return;

    pHistIO->Read  = readFn;
    pHistIO->Write = writeFn;

    WshShellHistory_t extHistory = pHistIO->Read();
    WshShell_U32_t extHash =
        WshShellMisc_CalcJenkinsHash((WshShell_U8_t*)&extHistory.Data, sizeof(extHistory.Data));

    if (extHash != extHistory.Hash)
        WshShellHistory_CalcHashAndWrite(pHistIO, (WshShellHistory_t){0});
}

static void WshShellHistory_IncrIdx(WshShell_Size_t* pIdx) {
    WSH_SHELL_ASSERT(pIdx);

    if (++(*pIdx) >= WSH_SHELL_HISTORY_BUFF_SIZE)
        *pIdx = 0;
}

static void WshShellHistory_DecrIdx(WshShell_Size_t* pIdx) {
    WSH_SHELL_ASSERT(pIdx);

    *pIdx = *pIdx == 0 ? WSH_SHELL_HISTORY_BUFF_SIZE - 1 : *pIdx - 1;
}

static void WshShellHistory_GetTokenFromBuffer(WshShell_Char_t* pDst, const WshShell_Char_t* pсSrc,
                                               WshShell_Size_t tokStartIdx,
                                               WshShell_Size_t tokLen) {
    WSH_SHELL_ASSERT(pDst && pсSrc && tokLen > 0);

    WshShell_Size_t readIdx = tokStartIdx;
    for (WshShell_Size_t tokIdx = 0; tokIdx < tokLen; tokIdx++) {
        pDst[tokIdx] = pсSrc[readIdx];
        WshShellHistory_IncrIdx(&readIdx);
    }
    pDst[tokLen] = '\0';
}

void WshShellHistory_SaveCmd(WshShellHistory_IO_t* pHistIO, const WshShell_Char_t* pcCmdStr,
                             WshShell_Size_t cmdStrLen) {
    WSH_SHELL_ASSERT(pHistIO && pcCmdStr && cmdStrLen < WSH_SHELL_HISTORY_BUFF_SIZE);
    if (!pHistIO || !pcCmdStr || cmdStrLen == 0 || cmdStrLen >= WSH_SHELL_HISTORY_BUFF_SIZE)
        return;

    WSH_SHELL_ASSERT(pHistIO->Read);
    if (!pHistIO->Read)
        return;

    WshShellHistory_t locHist = pHistIO->Read();

    /* Skip duplicate command */
    WshShell_Char_t* pHistCmd = &locHist.Data.StorageBuff[locHist.Data.LastSavedCmdIdx];
    if (WSH_SHELL_STRNCMP(pcCmdStr, pHistCmd, cmdStrLen) == 0) {
        /* Reset history rollback on cmd execution */
        locHist.Data.TailIdx = locHist.Data.HeadIdx;
        WshShellHistory_CalcHashAndWrite(pHistIO, locHist);

        return;
    }

    locHist.Data.LastSavedCmdIdx = locHist.Data.HeadIdx;

    for (WshShell_Size_t strIdx = 0; strIdx < cmdStrLen; strIdx++) {
        WSH_SHELL_ASSERT(WshShellStr_IsPrintableAscii(pcCmdStr[strIdx]));
        locHist.Data.StorageBuff[locHist.Data.HeadIdx] = pcCmdStr[strIdx];
        WshShellHistory_IncrIdx(&locHist.Data.HeadIdx);
    }

    /* Null-terminate and update tail */
    locHist.Data.StorageBuff[locHist.Data.HeadIdx] = '\0';
    WshShellHistory_IncrIdx(&locHist.Data.HeadIdx);
    locHist.Data.TailIdx = locHist.Data.HeadIdx;

    WshShellHistory_CalcHashAndWrite(pHistIO, locHist);
}

static WshShell_Size_t WshShellHistory_GetPrevToken(WshShellHistory_Data_t* pHistData,
                                                    WshShell_Size_t* pTokenIdx) {
    WSH_SHELL_ASSERT(pHistData && pTokenIdx);

    const WshShell_Char_t* pBuff = pHistData->StorageBuff;
    WshShell_Size_t head         = pHistData->HeadIdx;
    WshShell_Size_t tail         = pHistData->TailIdx;

    WshShellHistory_DecrIdx(&tail);
    while (pBuff[tail] == '\0') {
        if (tail == head)
            return 0;

        WshShellHistory_DecrIdx(&tail);
    }

    WshShell_Size_t tokLen = 0;
    while (pBuff[tail] != '\0') {
        if (tail == head)
            return 0;

        WshShellHistory_DecrIdx(&tail);
        tokLen++;
    }

    WshShellHistory_IncrIdx(&tail);
    pHistData->TailIdx = tail;
    *pTokenIdx         = tail;

    return tokLen;
}

static WshShell_Size_t WshShellHistory_GetNextToken(WshShellHistory_Data_t* pHistData,
                                                    WshShell_Size_t* pTokenIdx) {
    WSH_SHELL_ASSERT(pHistData && pTokenIdx);

    WshShell_Size_t tail = pHistData->TailIdx;
    if (tail == pHistData->HeadIdx)
        return 0;  // If tail reached head, no more commands

    *pTokenIdx = tail;

    WshShell_Size_t tokLen       = 0;
    const WshShell_Char_t* pBuff = pHistData->StorageBuff;
    while (pBuff[tail] != '\0') {
        if (tail == pHistData->HeadIdx)
            return 0;

        WshShellHistory_IncrIdx(&tail);
        tokLen++;
    }

    WshShellHistory_IncrIdx(&tail);
    pHistData->TailIdx = tail;
    return tokLen;
}

static WshShell_Size_t WshShellHistory_GetCmd(WshShellHistory_IO_t* pHistIO,
                                              WshShell_Char_t* pOutBuff,
                                              WshShell_Size_t outBuffSize,
                                              WSH_SHELL_HIST_CMD_DIR_t dir) {
    WSH_SHELL_ASSERT(pHistIO && pOutBuff && outBuffSize > 0);
    if (!pHistIO || !pOutBuff || outBuffSize == 0)
        return 0;

    WSH_SHELL_ASSERT(pHistIO->Read);
    if (!pHistIO->Read)
        return 0;

    WshShellHistory_t hist = pHistIO->Read();
    if (hist.Data.HeadIdx == 0 && hist.Data.TailIdx == 0 && hist.Data.LastSavedCmdIdx == 0)
        return 0;

    WshShell_Size_t tokIdx = 0;
    WshShell_Size_t tokLen = 0;

    if (dir == WSH_SHELL_HIST_CMD_PREV) {
        tokLen = WshShellHistory_GetPrevToken(&hist.Data, &tokIdx);
        if (hist.Data.PrevDir == WSH_SHELL_HIST_CMD_NEXT && !hist.Data.LimitIsReached)
            tokLen = WshShellHistory_GetPrevToken(&hist.Data, &tokIdx);
    } else {
        tokLen = WshShellHistory_GetNextToken(&hist.Data, &tokIdx);
        if (hist.Data.PrevDir == WSH_SHELL_HIST_CMD_PREV && !hist.Data.LimitIsReached)
            tokLen = WshShellHistory_GetNextToken(&hist.Data, &tokIdx);
    }

    if (tokLen != 0 && tokLen + 1 < outBuffSize) {
        hist.Data.LimitIsReached = false;
        WshShellHistory_GetTokenFromBuffer(pOutBuff, hist.Data.StorageBuff, tokIdx, tokLen);
    } else {
        hist.Data.LimitIsReached = true;
        WSH_SHELL_MEMSET(pOutBuff, 0, outBuffSize);
    }

    hist.Data.PrevDir = dir;
    WshShellHistory_CalcHashAndWrite(pHistIO, hist);
    return tokLen;
}

WshShell_Size_t WshShellHistory_GetPrevCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize) {
    return WshShellHistory_GetCmd(pHistIO, pOutBuff, outBuffSize, WSH_SHELL_HIST_CMD_PREV);
}

WshShell_Size_t WshShellHistory_GetNextCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize) {
    return WshShellHistory_GetCmd(pHistIO, pOutBuff, outBuffSize, WSH_SHELL_HIST_CMD_NEXT);
}

WshShell_Size_t WshShellHistory_GetTokenNum(WshShellHistory_IO_t* pHistIO) {
    WSH_SHELL_ASSERT(pHistIO);
    if (!pHistIO)
        return 0;

    WSH_SHELL_ASSERT(pHistIO->Read);
    if (!pHistIO->Read)
        return 0;

    WshShellHistory_t hist        = pHistIO->Read();
    WshShellHistory_Data_t* pHist = &hist.Data;
    pHist->TailIdx                = pHist->HeadIdx;

    WshShell_Size_t dummyTokIdx = 0;
    WshShell_Size_t count       = 0;

    while (WshShellHistory_GetPrevToken(pHist, &dummyTokIdx))
        count++;

    return count;
}

WshShell_Size_t WshShellHistory_GetTokenByIndex(WshShellHistory_IO_t* pHistIO,
                                                WshShell_Char_t* pOutBuff,
                                                WshShell_Size_t outBuffSize,
                                                WshShell_Size_t index) {
    WSH_SHELL_ASSERT(pHistIO && pOutBuff && outBuffSize > 0);
    if (!pHistIO || !pOutBuff || outBuffSize == 0)
        return 0;

    WSH_SHELL_ASSERT(pHistIO->Read);
    if (!pHistIO->Read)
        return 0;

    WshShellHistory_t hist        = pHistIO->Read();
    WshShellHistory_Data_t* pHist = &hist.Data;

    /* Set start position on last cmd for search */
    pHist->TailIdx = pHist->HeadIdx;

    WshShell_Size_t tokIdx = 0;
    WshShell_Size_t tokLen = 0;

    for (WshShell_Size_t idx = 0; idx <= index; idx++) {
        tokLen = WshShellHistory_GetPrevToken(pHist, &tokIdx);
        if (tokLen == 0)
            return 0;
    }

    if (tokLen + 1 >= outBuffSize)
        return 0;

    WshShellHistory_GetTokenFromBuffer(pOutBuff, pHist->StorageBuff, tokIdx, tokLen);
    return tokLen;
}

void WshShellHistory_Flush(WshShellHistory_IO_t* pHistIO) {
    WSH_SHELL_ASSERT(pHistIO);
    if (!pHistIO)
        return;

    WshShellHistory_CalcHashAndWrite(pHistIO, (WshShellHistory_t){0});
}

#else /* WSH_SHELL_HISTORY */

void WshShellHistory_Init(WshShellHistory_IO_t* pHistIO, WshShellHistory_ReadHandler_t readFn,
                          WshShellHistory_WriteHandler_t writeFn) {
    (void)(pHistIO);
    (void)(readFn);
    (void)(writeFn);
}

void WshShellHistory_SaveCmd(WshShellHistory_IO_t* pHistIO, const WshShell_Char_t* pcCmdStr,
                             WshShell_Size_t cmdStrLen) {
    (void)(pHistIO);
    (void)(pcCmdStr);
    (void)(cmdStrLen);
}

WshShell_Size_t WshShellHistory_GetPrevCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize) {
    (void)(pHistIO);
    (void)(pOutBuff);
    (void)(outBuffSize);

    return 0;
}

WshShell_Size_t WshShellHistory_GetNextCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize) {
    (void)(pHistIO);
    (void)(pOutBuff);
    (void)(outBuffSize);

    return 0;
}

WshShell_Size_t WshShellHistory_GetTokenNum(WshShellHistory_IO_t* pHistIO) {
    (void)(pHistIO);

    return 0;
}

WshShell_Size_t WshShellHistory_GetTokenByIndex(WshShellHistory_IO_t* pHistIO,
                                                WshShell_Char_t* pOutBuff,
                                                WshShell_Size_t outBuffSize,
                                                WshShell_Size_t index) {
    (void)(pHistIO);
    (void)(pOutBuff);
    (void)(outBuffSize);
    (void)(index);

    return 0;
}

void WshShellHistory_Flush(WshShellHistory_IO_t* pHistIO) {
    (void)(pHistIO);
}

#endif /* WSH_SHELL_HISTORY */
