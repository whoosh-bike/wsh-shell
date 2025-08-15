

# File wsh\_shell\_io.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_io.c**](wsh__shell__io_8c.md)

[Go to the documentation of this file](wsh__shell__io_8c.md)


```C++
#include "wsh_shell_io.h"

void WshShellIO_ClearInterBuff(WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    WSH_SHELL_MEMSET((void*)pCommandLine->Buff, 0,
                     sizeof(WshShell_Char_t) * WSH_SHELL_INTR_BUFF_LEN);
    pCommandLine->Len       = 0;
    pCommandLine->CursorPos = 0;
}

void WshShellIO_WriteToInterBuff(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t symbol) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    pCommandLine->Buff[pCommandLine->CursorPos] = symbol;
    WshShellStr_IncrInterCnt(&(pCommandLine->CursorPos), WSH_SHELL_INTR_BUFF_LEN);
    WshShellStr_IncrInterCnt(&(pCommandLine->Len), WSH_SHELL_INTR_BUFF_LEN);
}

void WshShellIO_PrintInterBuff(WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    WSH_SHELL_PRINT(pCommandLine->Buff);
}

void WshShellIO_RefreshConsoleFromInterBuff(WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    // Move cursor to the beginning
    for (WshShell_Size_t cur = pCommandLine->CursorPos; cur > 0; cur--)
        WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_BACKSPACE);  //Move crs left 1 char

    // Clear line to the right of cursor
    WSH_SHELL_PRINT(WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS);

    // Update line length and cursor position
    pCommandLine->Len       = WSH_SHELL_STRLEN(pCommandLine->Buff);
    pCommandLine->CursorPos = pCommandLine->Len;

    WshShellIO_PrintInterBuff(pCommandLine);
}

void WshShellIO_RemoveLeftSymbol(WshShellIO_CommandLine_t* pCommandLine) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine || pCommandLine->CursorPos == 0)
        return;

    WshShell_Size_t cursorPos = pCommandLine->CursorPos - 1;

    if (pCommandLine->CursorPos <= pCommandLine->Len) {
        // Shift buffer left from pos
        for (WshShell_Size_t pos = cursorPos; pos < pCommandLine->Len; pos++) {
            pCommandLine->Buff[pos] = pCommandLine->Buff[pos + 1];
        }

        WshShellStr_DecrInterCnt(&pCommandLine->Len);
        pCommandLine->CursorPos = cursorPos;

        WSH_SHELL_PRINT("%c%s%s%s%s", WSH_SHELL_SYM_BACKSPACE, WSH_SHELL_ESC_SAVE_CURSOR,
                        WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS, &pCommandLine->Buff[cursorPos],
                        WSH_SHELL_ESC_RESTORE_CURSOR);
    } else {
        // This should not happen, but safe fallback
        pCommandLine->CursorPos = cursorPos;
        WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_BACKSPACE);
    }
}

static const WshShell_Char_t* WshShellIO_InsertStringTemplate =
    WSH_SHELL_ESC_SAVE_CURSOR WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS
    "%s" WSH_SHELL_ESC_RESTORE_CURSOR WSH_SHELL_ESC_ARROW_RIGHT;

void WshShellIO_InsertSymbol(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t ch,
                             WshShell_Bool_t starsOrChars) {
    WSH_SHELL_ASSERT(pCommandLine);
    if (!pCommandLine)
        return;

    WshShell_Bool_t insertInMiddle = (WshShell_Bool_t)(pCommandLine->CursorPos < pCommandLine->Len);

    // Shift characters right if inserting in the middle of the line
    if (insertInMiddle) {
        for (WshShell_Size_t pos = pCommandLine->Len; pos > pCommandLine->CursorPos; pos--)
            pCommandLine->Buff[pos] = pCommandLine->Buff[pos - 1];
    }

    WshShellIO_WriteToInterBuff(pCommandLine, ch);

    WSH_SHELL_ASSERT(pCommandLine->CursorPos >= 1);
    WSH_SHELL_PRINT(WshShellIO_InsertStringTemplate,
                    starsOrChars ? "*" : &pCommandLine->Buff[pCommandLine->CursorPos - 1]);
}
```


