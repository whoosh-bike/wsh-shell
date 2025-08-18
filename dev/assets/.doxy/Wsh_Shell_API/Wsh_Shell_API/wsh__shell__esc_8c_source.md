

# File wsh\_shell\_esc.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_esc.c**](wsh__shell__esc_8c.md)

[Go to the documentation of this file](wsh__shell__esc_8c.md)


```C++
#include "wsh_shell_esc.h"

#if WSH_SHELL_HISTORY

static void WshShellEsc_ArrowUp(WshShellHistory_IO_t* pHistIO,
                                WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pHistIO && pCommandLine);
    if (!pHistIO || !pCommandLine)
        return;

    WshShellHistory_GetPrevCmd(pHistIO, pCommandLine->Buff, WSH_SHELL_INTR_BUFF_LEN);
    WshShellIO_RefreshConsoleFromInterBuff(pCommandLine);

    return;
}

static void WshShellEsc_ArrowDown(WshShellHistory_IO_t* pHistIO,
                                  WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pHistIO && pCommandLine);
    if (!pHistIO || !pCommandLine)
        return;

    WshShellHistory_GetNextCmd(pHistIO, pCommandLine->Buff, WSH_SHELL_INTR_BUFF_LEN);
    WshShellIO_RefreshConsoleFromInterBuff(pCommandLine);

    return;
}

#else /* WSH_SHELL_HISTORY */

static void WshShellEsc_ArrowUp(WshShellHistory_IO_t* pHistIO,
                                WshShellIO_CommandLine_t* pCommandLine) {
    return;
}

static void WshShellEsc_ArrowDown(WshShellHistory_IO_t* pHistIO,
                                  WshShellIO_CommandLine_t* pCommandLine) {
    return;
}

#endif /* WSH_SHELL_HISTORY */

static void WshShellEsc_ArrowRight(WshShellHistory_IO_t* pHistIO,
                                   WshShellIO_CommandLine_t* pCommandLine) {
    (void)pHistIO;

    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    if (pCommandLine->CursorPos < pCommandLine->Len) {
        WSH_SHELL_PRINT(WSH_SHELL_ESC_ARROW_RIGHT);
        WshShellStr_IncrInterCnt(&(pCommandLine->CursorPos), WSH_SHELL_INTR_BUFF_LEN);
    }
}

static void WshShellEsc_ArrowLeft(WshShellHistory_IO_t* pHistIO,
                                  WshShellIO_CommandLine_t* pCommandLine) {
    (void)pHistIO;

    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    if (pCommandLine->CursorPos > 0) {
        WSH_SHELL_PRINT(WSH_SHELL_ESC_ARROW_LEFT);
        WshShellStr_DecrInterCnt(&(pCommandLine->CursorPos));
    }
}

static void WshShellEsc_ClearStorage(WshShellEsc_Storage_t* pEscStorage) {
    WSH_SHELL_ASSERT(pEscStorage);
    if (!pEscStorage)
        return;

    WSH_SHELL_MEMSET((void*)pEscStorage->Buff, 0, WSH_SHELL_ESC_BUFF_LEN);
    pEscStorage->Cnt = 0;
}

static const WshShellEsc_Action_t WshShellEsc_SeqHandlers[] = {
    {WSH_SHELL_ESC_ARROW_UP, WshShellEsc_ArrowUp},
    {WSH_SHELL_ESC_ARROW_DOWN, WshShellEsc_ArrowDown},
    {WSH_SHELL_ESC_ARROW_LEFT, WshShellEsc_ArrowLeft},
    {WSH_SHELL_ESC_ARROW_RIGHT, WshShellEsc_ArrowRight},
};

void WshShellEsc_Handler(WshShellHistory_IO_t* pHistIO, WshShellIO_CommandLine_t* pCommandLine,
                         WshShellEsc_Storage_t* pEscStorage, const WshShell_Char_t symbol) {
    WSH_SHELL_ASSERT(pHistIO && pCommandLine && pEscStorage);
    if (!pHistIO || !pCommandLine || !pEscStorage)
        return;

    if (pEscStorage->Cnt >= WSH_SHELL_ESC_BUFF_LEN)
        return;

    pEscStorage->Buff[pEscStorage->Cnt++] = symbol;

    for (WshShell_Size_t seq = 0; seq < WSH_SHELL_ARR_LEN(WshShellEsc_SeqHandlers); seq++) {
        if (WSH_SHELL_STRNCMP(WshShellEsc_SeqHandlers[seq].Sequence, pEscStorage->Buff,
                              WSH_SHELL_ESC_BUFF_LEN) == 0) {
            WshShellEsc_SeqHandlers[seq].Handler(pHistIO, pCommandLine);
            WshShellEsc_ClearStorage(pEscStorage);
            break;
        }
    }
}

void WshShellEsc_StartSeq(WshShellEsc_Storage_t* pEscStorage) {
    WSH_SHELL_ASSERT(pEscStorage);
    if (!pEscStorage)
        return;

    pEscStorage->Cnt     = 1;
    pEscStorage->Buff[0] = WSH_SHELL_ESC_SEQ_START_CHAR;
}

WshShell_Bool_t WshShellEsc_IsSeqStarted(WshShellEsc_Storage_t* pEscStorage) {
    WSH_SHELL_ASSERT(pEscStorage);
    if (!pEscStorage)
        return false;

    return (WshShell_Bool_t)(pEscStorage->Cnt > 0 &&
                             pEscStorage->Buff[0] == WSH_SHELL_ESC_SEQ_START_CHAR);
}
```


