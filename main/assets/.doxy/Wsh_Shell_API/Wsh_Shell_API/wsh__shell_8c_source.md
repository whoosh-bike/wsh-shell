

# File wsh\_shell.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell.c**](wsh__shell_8c.md)

[Go to the documentation of this file](wsh__shell_8c.md)


```C++
#include "wsh_shell.h"

void WshShell_Stub_ExtClbk(void* pCtx) {
    (void)pCtx;
}

#define WSH_SHELL_USER_IS_AUTH()       (pShell->CurrUser != NULL)
#define WSH_SHELL_TMP_LOGIN_IS_EMPTY() (pShell->TmpAuth.Login[0] == 0)
#define WSH_SHELL_TMP_PASS_IS_EMPTY()  (pShell->TmpAuth.Pass[0] == 0)
#define WSH_SHELL_INTER_CMD_EXISTS()   (pShell->Interact.Handler != NULL)

static void WshShell_InvitationPrint(WshShell_t* pShell) {
    if (!WSH_SHELL_USER_IS_AUTH()) {
        if (WSH_SHELL_TMP_LOGIN_IS_EMPTY()) {
            WSH_SHELL_PRINT_SYS("Login: ");
        } else if (WSH_SHELL_TMP_PASS_IS_EMPTY()) {
            WSH_SHELL_PRINT_SYS("Password: ");
        }

        return;
    }

    WSH_SHELL_PRINT(pShell->PS1);
}

WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShell, const WshShell_Char_t* pcDevName,
                                    const WshShell_Char_t* pcCustomHeader,
                                    WshShell_ExtCallbacks_t* pExtClbks) {
    WSH_SHELL_ASSERT(pShell && pcDevName);
    if (!pShell || !pcDevName)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WSH_SHELL_MEMSET((void*)pShell, 0, sizeof(WshShell_t));

    WshShell_Size_t bufSize = sizeof(pShell->DeviceName);
    WshShell_Size_t len     = WSH_SHELL_STRLEN(pcDevName);
    if (len >= bufSize)
        len = bufSize - 1;

    WSH_SHELL_MEMCPY(pShell->DeviceName, pcDevName, len);
    pShell->DeviceName[len] = '\0';

    pShell->Version = WSH_SHELL_VERSION_STR;

    const WshShell_Size_t numClbks = sizeof(pShell->ExtCallbacks) / sizeof(WshShell_ExtClbk_t);
    WshShell_ExtClbk_t* pClbkArr   = (WshShell_ExtClbk_t*)&pShell->ExtCallbacks;

    if (!pExtClbks) {
        // Fill all with stub
        for (WshShell_Size_t clbk = 0; clbk < numClbks; clbk++) {
            pClbkArr[clbk] = WshShell_Stub_ExtClbk;
        }
    } else {
        // Fill from provided struct
        WshShell_ExtClbk_t* pInputArr = (WshShell_ExtClbk_t*)pExtClbks;
        for (WshShell_Size_t clbk = 0; clbk < numClbks; clbk++) {
            pClbkArr[clbk] = pInputArr[clbk] ? pInputArr[clbk] : WshShell_Stub_ExtClbk;
        }
    }

    WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
    WSH_SHELL_PRINT(WSH_SHELL_COLOR_PURPLE);
    WSH_SHELL_PRINT(pcCustomHeader ? pcCustomHeader : WSH_SHELL_HEADER);
    WSH_SHELL_PRINT_SYS("Serial Shell Service (wsh-shell v%s) started on device "
                        "(%s)\r\n" WSH_SHELL_PRESS_ENTER_TO_LOG_IN_STR "\r\n",
                        pShell->Version, pShell->DeviceName);

    WshShellPromptWait_Attach(&(pShell->PromptWait), WshShellPromptWait_Enter, NULL);

    return WSH_SHELL_RET_STATE_SUCCESS;
}

WshShell_Bool_t WshShell_Auth(WshShell_t* pShell, const WshShell_Char_t* pcLogin,
                              const WshShell_Char_t* pcPass) {
    WSH_SHELL_ASSERT(pShell && pcLogin && pcPass);
    if (!pShell || !pcLogin || !pcPass)
        return false;

    pShell->CurrUser = WshShellUser_FindByCredentials(&(pShell->Users), pcLogin, pcPass);
    if (WSH_SHELL_USER_IS_AUTH()) {
        WshShell_PS1Data_t data = {
            .UserName     = pShell->CurrUser->Login,
            .DevName      = pShell->DeviceName,
            .InterCmdName = WSH_SHELL_INTER_CMD_EXISTS() ? pShell->Interact.CmdName : NULL,
        };
        WshShell_GeneratePS1(pShell->PS1, &data);
        pShell->ExtCallbacks.Auth(NULL);
        WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
    }

    return WSH_SHELL_USER_IS_AUTH();
}

WshShell_Bool_t WshShell_IsAuth(WshShell_t* pShell) {
    WSH_SHELL_ASSERT(pShell);
    if (!pShell)
        return false;

    return (bool)WSH_SHELL_USER_IS_AUTH();
}

void WshShell_DeAuth(WshShell_t* pShell, const WshShell_Char_t* pcReason) {
    WSH_SHELL_ASSERT(pShell && pcReason);
    if (!pShell || !pcReason)
        return;

    pShell->CurrUser = NULL;
    pShell->ExtCallbacks.DeAuth(NULL);
    WshShellHistory_Flush(&(pShell->HistoryIO));

    WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
    WSH_SHELL_PRINT_ERR("Shell deAuthed by `%s`!\r\n", pcReason);
    WSH_SHELL_PRINT_SYS(WSH_SHELL_PRESS_ENTER_TO_LOG_IN_STR "\r\n");

    WshShellPromptWait_Attach(&(pShell->PromptWait), WshShellPromptWait_Enter, NULL);
}

static void WshShell_AuthHandler(WshShell_t* pShell) {
    WshShell_Size_t len = pShell->CommandLine.Len;

    if (WSH_SHELL_TMP_LOGIN_IS_EMPTY()) {
        if (len >= WSH_SHELL_LOGIN_LEN)
            len = WSH_SHELL_LOGIN_LEN - 1;

        WSH_SHELL_MEMCPY(pShell->TmpAuth.Login, pShell->CommandLine.Buff, len);
        pShell->TmpAuth.Login[len] = '\0';
    } else if (WSH_SHELL_TMP_PASS_IS_EMPTY()) {
        if (len >= WSH_SHELL_PASS_LEN)
            len = WSH_SHELL_PASS_LEN - 1;

        WSH_SHELL_MEMCPY(pShell->TmpAuth.Pass, pShell->CommandLine.Buff, len);
        pShell->TmpAuth.Pass[len] = '\0';
    }

    if (!WSH_SHELL_TMP_LOGIN_IS_EMPTY() && !WSH_SHELL_TMP_PASS_IS_EMPTY()) {
        WshShell_Bool_t isAuthOk =
            WshShell_Auth(pShell, pShell->TmpAuth.Login, pShell->TmpAuth.Pass);

        WSH_SHELL_MEMSET((void*)pShell->TmpAuth.Login, 0, WSH_SHELL_LOGIN_LEN);
        WSH_SHELL_MEMSET((void*)pShell->TmpAuth.Pass, 0, WSH_SHELL_PASS_LEN);

        if (!isAuthOk)
            WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
    }

    WshShellIO_ClearInterBuff(&(pShell->CommandLine));
}

static void WshShell_StringHandler(WshShell_t* pShell) {
    pShell->CommandLine.Buff[pShell->CommandLine.Len] = 0;

    const WshShell_Char_t* pcCmdStr =
        WshShellStr_TrimString(pShell->CommandLine.Buff, pShell->CommandLine.Len);
    WshShell_Char_t cmdStr[WSH_SHELL_INTR_BUFF_LEN] = {0};
    WSH_SHELL_STRNCPY(cmdStr, pcCmdStr, WSH_SHELL_INTR_BUFF_LEN - 1);

    WshShell_Size_t argc                                      = 0;
    const WshShell_Char_t* pсArgv[WSH_SHELL_CMD_ARGS_MAX_NUM] = {0};

    WshShellStr_ParseToArgcArgv(cmdStr, &argc, pсArgv, WSH_SHELL_CMD_ARGS_MAX_NUM);
    if (argc == 0)
        return;

    WshShellHistory_SaveCmd(&(pShell->HistoryIO), pcCmdStr, WSH_SHELL_STRLEN(pcCmdStr));

    const WshShellCmd_t* pcCmd      = WshShellDefCmd_GetPtr();
    WshShellCmdHandler_t cmdHandler = NULL;

    if (WSH_SHELL_STRNCMP(pcCmd->Name, pсArgv[0], WSH_SHELL_CMD_NAME_LEN) == 0) {
        cmdHandler = pcCmd->Handler;
    } else {
        pcCmd = WshShellCmd_SearchCmd(&(pShell->Commands), pсArgv[0]);
        if (pcCmd == NULL) {
            WSH_SHELL_PRINT_WARN("Command \"%s\" not found!\r\n", pcCmdStr);
        } else if ((pShell->CurrUser->Groups & pcCmd->Groups) != 0) {
            cmdHandler = pcCmd->Handler;
        } else {
            WSH_SHELL_PRINT_WARN("Access denied for command \"%s\"\r\n", pсArgv[0]);
        }
    }

    if (cmdHandler) {
        WSH_SHELL_RET_STATE_t retState = cmdHandler(pcCmd, argc, pсArgv, pShell);

        if (WSH_SHELL_INTER_CMD_EXISTS()) {
            WshShell_PS1Data_t data = {
                .UserName     = pShell->CurrUser->Login,
                .DevName      = pShell->DeviceName,
                .InterCmdName = pShell->Interact.CmdName,
            };
            WshShell_GeneratePS1(pShell->PS1, &data);
        }

        if (retState != WSH_SHELL_RET_STATE_SUCCESS) {
            WSH_SHELL_PRINT_ERR("Command handler internal error: %s\r\n",
                                WshShell_GetRetStateStr(retState));
        }
    }

    WshShellIO_ClearInterBuff(&(pShell->CommandLine));
}

static void WshShell_SymbolHandler(WshShell_t* pShell, const WshShell_Char_t symbol) {
    switch (symbol) {
        case WSH_SHELL_SYM_EXIT:
            if (WSH_SHELL_INTER_CMD_EXISTS()) {
                WshShellInteract_Flush(&(pShell->Interact));

                WshShell_PS1Data_t data = {
                    .UserName     = pShell->CurrUser->Login,
                    .DevName      = pShell->DeviceName,
                    .InterCmdName = NULL,
                };
                WshShell_GeneratePS1(pShell->PS1, &data);
                WSH_SHELL_PRINT(WSH_SHELL_END_LINE);
                WshShell_InvitationPrint(pShell);
            } else
                WshShell_DeAuth(pShell, "Ctrl+D");
            break;

        case WSH_SHELL_SYM_BACKSPACE:
        case WSH_SHELL_SYM_DELETE:
            WshShellIO_RemoveLeftSymbol(&(pShell->CommandLine));
            break;

        case WSH_SHELL_SYM_TAB:
            if (!WSH_SHELL_USER_IS_AUTH())
                break;

            if (WshShellAutocomplete_Try(pShell->CommandLine.Buff, pShell->CommandLine.Len,
                                         &(pShell->Commands))) {
                WshShellIO_RefreshConsoleFromInterBuff(&(pShell->CommandLine));
            } else {
                WshShell_InvitationPrint(pShell);
                WshShellIO_PrintInterBuff(&(pShell->CommandLine));
            }
            break;

        case WSH_SHELL_ESC_SEQ_START_CHAR:
            WshShellEsc_StartSeq(&(pShell->EscStorage));
            break;

        default:
            if (!WshShellStr_IsPrintableAscii(symbol)) {
                WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_SOUND);
                return;
            }

            WshShell_Bool_t starsOrChars =
                (bool)(!WSH_SHELL_USER_IS_AUTH() && !WSH_SHELL_TMP_LOGIN_IS_EMPTY());
            WshShellIO_InsertSymbol(&(pShell->CommandLine), symbol, starsOrChars);
            break;
    }
}

#define SHELL_SAVE_PREV_AND_RETURN(pShell, sym) \
    do {                                        \
        (pShell)->PrevSym = (sym);              \
        return;                                 \
    } while (0)

void WshShell_InsertChar(WshShell_t* pShell, const WshShell_Char_t symbol) {
    WSH_SHELL_ASSERT(pShell);
    if (!pShell)
        return;

    pShell->ExtCallbacks.SymbolIn(NULL);

    WshShell_Bool_t isEnterPressed = false;
    if (symbol == WSH_SHELL_CHAR_CR || symbol == WSH_SHELL_CHAR_LF) {
        if (pShell->PrevSym == WSH_SHELL_CHAR_CR && symbol == WSH_SHELL_CHAR_LF) {
            SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
        }

        isEnterPressed = true;
    }

    WSH_SHELL_RET_STATE_t promptWaitRes = WshShellPromptWait_Handle(&(pShell->PromptWait), symbol);
    if (promptWaitRes == WSH_SHELL_RET_STATE_ERR_BUSY) {
        SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
    }

    if (isEnterPressed) {
        WSH_SHELL_PRINT(WSH_SHELL_END_LINE);

        if (!WSH_SHELL_USER_IS_AUTH()) {
            WshShell_AuthHandler(pShell);
            WshShell_InvitationPrint(pShell);
            SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
        }

        if (WSH_SHELL_INTER_CMD_EXISTS()) {
            pShell->Interact.Handler(&(pShell->CommandLine));
            WshShellIO_ClearInterBuff(&(pShell->CommandLine));
        } else
            WshShell_StringHandler(pShell);

        if (WshShell_IsAuth(pShell))
            WshShell_InvitationPrint(pShell);

        SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
    }

    if (WshShellEsc_IsSeqStarted(&(pShell->EscStorage))) {
        WshShellEsc_Handler(&(pShell->HistoryIO), &(pShell->CommandLine), &(pShell->EscStorage),
                            symbol);
        SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
    }

    WshShell_SymbolHandler(pShell, symbol);
    SHELL_SAVE_PREV_AND_RETURN(pShell, symbol);
}
```


