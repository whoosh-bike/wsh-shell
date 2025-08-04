#include "wsh_shell.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_history.h"
#include "wsh_shell_io.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"

typedef struct {
    const char* Entry;
    void (*Func)(void);
} WshShell_EscAction_t;

static WshShell_t* pShell = NULL;

static void WshShell_RefreshConsoleFromInterBuff(void);

static void WshShell_ArrowUp(void) {
    WshShellHistory_GetPrevCmd(pShell->InteractiveBuff, WSH_SHELL_INTR_BUFF_SIZE);
    WshShell_RefreshConsoleFromInterBuff();
}

static void WshShell_ArrowDown(void) {
    WshShellHistory_GetNextCmd(pShell->InteractiveBuff, WSH_SHELL_INTR_BUFF_SIZE);
    WshShell_RefreshConsoleFromInterBuff();
}

static void WshShell_ArrowRight(void) {
    if (pShell->CursorPos < pShell->InterLineLength) {
        WSH_SHELL_PRINT(WSH_SHELL_ESC_ARROW_RIGHT);
        WshShellStr_IncrInterCnt(&pShell->CursorPos, WSH_SHELL_INTR_BUFF_SIZE);
    }
}

static void WshShell_ArrowLeft(void) {
    if (pShell->CursorPos > 0) {
        WSH_SHELL_PRINT(WSH_SHELL_ESC_ARROW_LEFT);
        WshShellStr_DecrInterCnt(&pShell->CursorPos);
    }
}

/* clang-format off */
static WshShell_EscAction_t EscArray[] = {
	{ WSH_SHELL_ESC_ARROW_UP,       WshShell_ArrowUp    },
	{ WSH_SHELL_ESC_ARROW_DOWN,     WshShell_ArrowDown  },
	{ WSH_SHELL_ESC_ARROW_LEFT,     WshShell_ArrowLeft  },
	{ WSH_SHELL_ESC_ARROW_RIGHT,    WshShell_ArrowRight },
};
/* clang-format on */

static void WshShell_ClearInterBuff(void) {
    WSH_SHELL_MEMSET(pShell->InteractiveBuff, 0, sizeof(char) * WSH_SHELL_INTR_BUFF_SIZE);
    pShell->InterLineLength = 0;
    pShell->CursorPos       = 0;
}

static void WshShell_WriteToInterBuff(char symbol) {
    pShell->InteractiveBuff[pShell->CursorPos] = symbol;
    WshShellStr_IncrInterCnt(&pShell->CursorPos, WSH_SHELL_INTR_BUFF_SIZE);
    WshShellStr_IncrInterCnt(&pShell->InterLineLength, WSH_SHELL_INTR_BUFF_SIZE);
}

static void WshShell_RefreshConsoleFromInterBuff(void) {
    for (WshShell_Size_t i = pShell->CursorPos; i > 0; i--)
        WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_BACKSPACE);  //Move crs left 1 char
    // Clear str rigth from crs
    WSH_SHELL_PRINT(WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS);
    pShell->InterLineLength = WSH_SHELL_STRLEN(pShell->InteractiveBuff);
    pShell->CursorPos       = pShell->InterLineLength;
    WSH_SHELL_PRINT("%s", &pShell->InteractiveBuff[0]);
}

WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShellStorage) {
    WSH_SHELL_ASSERT(pShellStorage != NULL);
    WSH_SHELL_ASSERT(pShell == NULL);
    if (pShellStorage == NULL || pShell != NULL)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    pShell = pShellStorage;
    WSH_SHELL_MEMSET(pShell, 0, sizeof(WshShell_t));
    return WSH_SHELL_RET_STATE_SUCCESS;
}

static const WshShellCmd_t* WshShell_SearchCmd(const char* pName) {
    for (WshShell_Size_t i = 0; i < WshShellCmd_GetCmdNum(); i++) {
        const WshShellCmd_t* pCmd = WshShellCmd_GetCmdByIndex(i);
        if (WSH_SHELL_STRNCMP(pCmd->pName, pName, WSH_SHELL_CMD_MAX_NAME_LEN) == 0)
            return pCmd;
    }
    return NULL;
}

static void WshShell_StringHandler(void) {
    pShell->InteractiveBuff[pShell->InterLineLength] = 0;

    char* pCmdStr                         = WshShellStr_TrimString(pShell->InteractiveBuff, pShell->InterLineLength);
    char cmdStr[WSH_SHELL_INTR_BUFF_SIZE] = {0};
    WSH_SHELL_STRNCPY(cmdStr, pCmdStr, WSH_SHELL_INTR_BUFF_SIZE - 1);
    WshShell_Size_t argc                          = 0;
    const char* pArgv[WSH_SHELL_CMD_ARGS_MAX_NUM] = {0};
    if (pShell->pIntCmd != NULL) {
        pArgv[0] = pShell->pIntCmd->pName;
        WshShellStr_ParseToArgcArgv(cmdStr, &argc, &pArgv[1]);
        argc++;
    } else
        WshShellStr_ParseToArgcArgv(cmdStr, &argc, pArgv);
    if (argc == 0)
        return;

    WshShellHistory_SaveCmd(pCmdStr, WSH_SHELL_STRLEN(pCmdStr));

    const WshShellCmd_t* pCmd = WshShell_SearchCmd(pArgv[0]);
    if (pCmd != NULL) {
        if (pShell->pUser->Groups & pCmd->Group) {
            WSH_SHELL_RET_STATE_t retState = pCmd->Exec(pCmd, argc, pArgv);
            if (retState != WSH_SHELL_RET_STATE_SUCCESS)
                WSH_SHELL_PRINT_ERR("Command: %s\r\nError: %s\r\n", pCmdStr, WshShell_RetStateStr_Get(retState));
        }
    } else {
        WSH_SHELL_PRINT_WARN("\"%s\" command not found!\r\n", pCmdStr);
    }

    WshShell_ClearInterBuff();
}

static void WshShell_EscHandler(const char symbol) {
    pShell->EscBuff[pShell->EscCnt++] = symbol;
    for (WshShell_Size_t i = 0; i < WSH_SHELL_ARR_LEN(EscArray); i++) {
        if (WSH_SHELL_STRNCMP(EscArray[i].Entry, pShell->EscBuff, WSH_SHELL_ESC_BUFF_LEN) == 0) {
            EscArray[i].Func();
            WSH_SHELL_MEMSET(pShell->EscBuff, 0, WSH_SHELL_ESC_BUFF_LEN);
            pShell->EscCnt = 0;
        }
    }
}

static void WshShell_Autocomplete(void) {
    char cmdStr[WSH_SHELL_INTR_BUFF_SIZE] = {0};
    WSH_SHELL_STRNCPY(cmdStr, pShell->InteractiveBuff, pShell->InterLineLength);
    const WshShellCmd_t* pFoundedCmd = NULL;
    for (WshShell_Size_t i = 0; i < WshShellCmd_GetCmdNum(); i++) {
        const WshShellCmd_t* pCmd = WshShellCmd_GetCmdByIndex(i);
        if (WSH_SHELL_STRNCMP(cmdStr, pCmd->pName, WSH_SHELL_STRLEN(cmdStr)) == 0) {
            pFoundedCmd = pCmd;
            break;
        }
    }
    if (pFoundedCmd != NULL) {
        WSH_SHELL_STRNCPY(pShell->InteractiveBuff, pFoundedCmd->pName, WSH_SHELL_CMD_MAX_NAME_LEN);
        WshShell_RefreshConsoleFromInterBuff();
    }
}

static void WshShell_SymbolHandler(const char symbol) {
    switch (symbol) {
        case WSH_SHELL_SYM_EXIT:
            WshShell_Exit();
            WSH_SHELL_PRINT(WSH_SHELL_END_LINE);
            WshShell_InvitationPrint();
            break;

        case WSH_SHELL_SYM_BACKSPACE:
            if (pShell->CursorPos > 0) {
                if (pShell->CursorPos <= pShell->InterLineLength) {
                    for (WshShell_Size_t i = --pShell->CursorPos; i <= pShell->InterLineLength; i++)
                        pShell->InteractiveBuff[i] = pShell->InteractiveBuff[i + 1];
                    WshShellStr_DecrInterCnt(&pShell->InterLineLength);
                    WSH_SHELL_PRINT("%c%s%s%s%s", WSH_SHELL_SYM_BACKSPACE, WSH_SHELL_ESC_SAVE_CURSOR,
                                    WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS, &pShell->InteractiveBuff[pShell->CursorPos],
                                    WSH_SHELL_ESC_RESTORE_CURSOR);
                } else {
                    WSH_SHELL_PRINT("%c", WSH_SHELL_SYM_BACKSPACE);
                    pShell->CursorPos--;
                }
            }
            break;

        case WSH_SHELL_SYM_TAB:
            WshShell_Autocomplete();
            break;

        case '\033':
            pShell->EscCnt     = 1;
            pShell->EscBuff[0] = '\033';
            break;

        default:
            if (symbol >= ' ') {
                // If cursor isn't at the end of line we need to add new characters while
                // keeping old ones
                if (pShell->CursorPos < pShell->InterLineLength) {
                    for (WshShell_Size_t i = pShell->InterLineLength; i > pShell->CursorPos; i--)
                        pShell->InteractiveBuff[i] = pShell->InteractiveBuff[i - 1];

                    WshShell_WriteToInterBuff(symbol);
                    // Save and Clear str rigth from crs
                    if (pShell->pUser == NULL && pShell->pLogin[0] != 0)
                        break;
                    WSH_SHELL_PRINT(WSH_SHELL_ESC_SAVE_CURSOR WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS
                                    "%s" WSH_SHELL_ESC_RESTORE_CURSOR WSH_SHELL_ESC_ARROW_RIGHT,
                                    pShell->InteractiveBuff + pShell->CursorPos - 1);
                } else {
                    // If cursor is at the end of line just add symbol to the end
                    WshShell_WriteToInterBuff(symbol);
                    if (pShell->pUser == NULL && pShell->pLogin[0] != 0)
                        break;
                    WSH_SHELL_PRINT("%c", symbol);
                }
            }
            break;
    }
}

void WshShell_InvitationPrint(void) {
    WSH_SHELL_ASSERT(pShell != NULL);
    if (pShell == NULL)
        return;

    if (pShell->pUser == NULL) {
        if (pShell->pLogin[0] == 0) {
            WSH_SHELL_PRINT("Login: ");
        } else if (pShell->pPwd[0] == 0) {
            WSH_SHELL_PRINT("Password: ");
        }
    } else {
        WSH_SHELL_PRINT(WSH_SHELL_PROMPT_FMT, pShell->pUser->pLogin, WSH_SHELL_DEVICE_NAME,
                        pShell->pIntCmd == NULL ? "" : pShell->pIntCmd->pName);
    }
}

void WshShell_SetIntCmd(const WshShellCmd_t* pCmd) {
    WSH_SHELL_ASSERT(pShell != NULL);
    WSH_SHELL_ASSERT(pCmd != NULL);
    if (pShell == NULL || pCmd == NULL)
        return;

    pShell->pIntCmd = pCmd;
}

void WshShell_Auth(const char* pLogin, const char* pPwd) {
    WSH_SHELL_ASSERT(pShell != NULL);
    WSH_SHELL_ASSERT(pLogin != NULL);
    WSH_SHELL_ASSERT(pPwd != NULL);
    if (pShell == NULL || pLogin == NULL || pPwd == NULL)
        return;

    for (WshShell_Size_t i = 0; i < WshShellUser_GetUsersNum(); i++) {
        if (WshShellUser_CheckCredentials(i, pLogin, pPwd)) {
            pShell->pUser = WshShellUser_GetUserByIndex(i);
            break;
        }
    }
    WSH_SHELL_MEMSET(pShell->pLogin, 0, WSH_SHELL_USER_LOGIN_MAX_LEN);
    WSH_SHELL_MEMSET(pShell->pPwd, 0, WSH_SHELL_USER_PASS_MAX_LEN);
}

bool WshShell_IsAuth(void) {
    WSH_SHELL_ASSERT(pShell != NULL);
    if (pShell == NULL)
        return false;
    return pShell->pUser == NULL ? false : true;
}

bool WshShell_CheckUserAccess(const WshShellCmd_OptDescr_t* pOptDescr) {
    WSH_SHELL_ASSERT(pShell != NULL);
    WSH_SHELL_ASSERT(pOptDescr != NULL);
    if (pShell == NULL || pOptDescr == NULL)
        return false;

    if (pOptDescr->pOpt == NULL || !WshShell_IsAuth())
        return false;
    return (pShell->pUser->Rights & pOptDescr->pOpt->AccessBits);
}

void WshShell_Exit(void) {
    WSH_SHELL_ASSERT(pShell != NULL);
    if (pShell == NULL)
        return;

    if (pShell->pIntCmd != NULL)
        pShell->pIntCmd = NULL;
    else {
        pShell->pUser = NULL;
        WshShellHistory_Flush();
    }
}

static void WshShell_AuthHandler(void) {
    if (pShell->pLogin[0] == 0)
        WSH_SHELL_STRNCPY(pShell->pLogin, pShell->InteractiveBuff, pShell->InterLineLength);
    else if (pShell->pPwd[0] == 0)
        WSH_SHELL_STRNCPY(pShell->pPwd, pShell->InteractiveBuff, pShell->InterLineLength);

    if (pShell->pLogin[0] != 0 && pShell->pPwd[0] != 0)
        WshShell_Auth(pShell->pLogin, pShell->pPwd);

    WshShell_ClearInterBuff();
}

void WshShell_InsertChar(char symbol) {
    if (symbol == 0)
        return;

    if (symbol == '\r' || symbol == '\n') {
        WSH_SHELL_PRINT(WSH_SHELL_END_LINE);
        if (pShell->pUser == NULL)
            WshShell_AuthHandler();
        else
            WshShell_StringHandler();
        WshShell_InvitationPrint();
    } else {
        if (pShell->EscBuff[0] == '\033')
            WshShell_EscHandler(symbol);
        else
            WshShell_SymbolHandler(symbol);
    }
}

void WshShell_Destroy(void) {
    pShell = NULL;
}
