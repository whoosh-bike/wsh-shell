#include "wsh_shell_user.h"

WSH_SHELL_RET_STATE_t WshShellUser_Attach(WshShellUser_Table_t* pShellUsers,
                                          const WshShellUser_t* pcUserTable,
                                          WshShell_Size_t userNum) {
    WSH_SHELL_ASSERT(pShellUsers && pcUserTable && userNum > 0);
    if (!pShellUsers || !pcUserTable || userNum == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pShellUsers->List != NULL)
        return WSH_SHELL_RET_STATE_ERR_BUSY;  // Already inited

    pShellUsers->List = pcUserTable;
    pShellUsers->Num  = userNum;

    return WSH_SHELL_RET_STATE_SUCCESS;
}

void WshShellUser_DeAttach(WshShellUser_Table_t* pShellUsers) {
    WSH_SHELL_ASSERT(pShellUsers);

    if (pShellUsers)
        *pShellUsers = (WshShellUser_Table_t){0};
}

WshShell_Size_t WshShellUser_GetUsersNum(WshShellUser_Table_t* pShellUsers) {
    WSH_SHELL_ASSERT(pShellUsers);
    if (!pShellUsers || !pShellUsers->List)
        return 0;

    return pShellUsers->Num;
}

const WshShellUser_t* WshShellUser_GetUserByIndex(WshShellUser_Table_t* pShellUsers,
                                                  WshShell_Size_t idx) {
    WSH_SHELL_ASSERT(pShellUsers);
    if (!pShellUsers || !pShellUsers->List)
        return NULL;

    WSH_SHELL_ASSERT(idx < pShellUsers->Num);

    return idx < pShellUsers->Num ? &pShellUsers->List[idx] : NULL;
}

WshShell_Bool_t WshShellUser_CheckCredentials(WshShellUser_Table_t* pShellUsers,
                                              WshShell_Size_t UserID,
                                              const WshShell_Char_t* pcLogin,
                                              const WshShell_Char_t* pcPassword) {
    WSH_SHELL_ASSERT(pShellUsers && pcLogin && pcPassword);
    if (!pShellUsers || !pShellUsers->List || !pcLogin || !pcPassword)
        return false;

    WSH_SHELL_ASSERT(UserID < pShellUsers->Num);

    if (UserID >= pShellUsers->Num)
        return false;

    if (WSH_SHELL_STRNLEN(pcLogin, WSH_SHELL_LOGIN_LEN + 1) > WSH_SHELL_LOGIN_LEN ||
        WSH_SHELL_STRNLEN(pcPassword, WSH_SHELL_PASS_LEN + 1) > WSH_SHELL_PASS_LEN)
        return false;

    const WshShellUser_t* pUser = &pShellUsers->List[UserID];
    return WSH_SHELL_STRNCMP(pUser->Login, pcLogin, WSH_SHELL_LOGIN_LEN) == 0 &&
           WSH_SHELL_STRNCMP(pUser->Pass, pcPassword, WSH_SHELL_PASS_LEN) == 0;
}

const WshShellUser_t* WshShellUser_FindByCredentials(WshShellUser_Table_t* pShellUsers,
                                                     const WshShell_Char_t* pcLogin,
                                                     const WshShell_Char_t* pcPass) {
    WSH_SHELL_ASSERT(pShellUsers && pcLogin && pcPass);
    if (!pShellUsers || !pShellUsers->List || !pcLogin || !pcPass)
        return NULL;

    for (WshShell_Size_t usr = 0; usr < WshShellUser_GetUsersNum(pShellUsers); usr++) {
        if (WshShellUser_CheckCredentials(pShellUsers, usr, pcLogin, pcPass))
            return WshShellUser_GetUserByIndex(pShellUsers, usr);
    }

    return NULL;
}
