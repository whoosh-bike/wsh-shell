

# File wsh\_shell\_user.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_user.c**](wsh__shell__user_8c.md)

[Go to the documentation of this file](wsh__shell__user_8c.md)


```C++
#include "wsh_shell_user.h"

static void WshShellUser_DefHashFunc(const WshShell_Char_t* pcSalt, const WshShell_Char_t* pcPass,
                                     WshShell_Char_t* pHash) {
    WshShell_Size_t saltLen = WSH_SHELL_STRLEN(pcSalt);
    WshShell_Size_t passLen = WSH_SHELL_STRLEN(pcPass);

    if (saltLen > WSH_SHELL_SALT_LEN || passLen > WSH_SHELL_PASS_LEN) {
        WSH_SHELL_ASSERT(0);
        return;
    }

    char saltPass[WSH_SHELL_SALT_LEN + WSH_SHELL_PASS_LEN + 1];
    WSH_SHELL_MEMCPY(saltPass, pcSalt, saltLen);
    WSH_SHELL_MEMCPY(saltPass + saltLen, pcPass, passLen);
    saltPass[saltLen + passLen] = '\0';

    WshShell_Size_t hash =
        WshShellMisc_CalcJenkinsHash((const WshShell_U8_t*)saltPass, saltLen + passLen);

    WshShell_Char_t saltPassHashStr[WSH_SHELL_SALT_PASS_HASH_LEN + 1];
    WSH_SHELL_SNPRINTF(saltPassHashStr, sizeof(saltPassHashStr), "%08x", hash);

    WSH_SHELL_MEMCPY(pHash, saltPassHashStr, WSH_SHELL_STRLEN(saltPassHashStr) + 1);
}

WSH_SHELL_RET_STATE_t WshShellUser_Attach(WshShellUser_Table_t* pShellUsers,
                                          const WshShellUser_t* pcUserTable,
                                          WshShell_Size_t userNum,
                                          WshShellUser_HashFunc_t extHashFunc) {
    WSH_SHELL_ASSERT(pShellUsers && pcUserTable && userNum > 0);
    if (!pShellUsers || !pcUserTable || userNum == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    if (pShellUsers->List != NULL)
        return WSH_SHELL_RET_STATE_ERR_BUSY;  // Already inited

    pShellUsers->List = pcUserTable;
    pShellUsers->Num  = userNum;
    pShellUsers->Hash = extHashFunc ? extHashFunc : WshShellUser_DefHashFunc;

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
                                              const WshShell_Char_t* pcPass) {
    WSH_SHELL_ASSERT(pShellUsers && pcLogin && pcPass);
    if (!pShellUsers || !pShellUsers->List || !pcLogin || !pcPass)
        return false;

    WSH_SHELL_ASSERT(UserID < pShellUsers->Num);

    if (UserID >= pShellUsers->Num)
        return false;

    WshShell_Size_t loginLen = WSH_SHELL_STRNLEN(pcLogin, WSH_SHELL_LOGIN_LEN + 1);
    WshShell_Size_t passLen  = WSH_SHELL_STRNLEN(pcPass, WSH_SHELL_PASS_LEN + 1);

    if (loginLen > WSH_SHELL_LOGIN_LEN || passLen > WSH_SHELL_PASS_LEN)
        return false;

    const WshShellUser_t* pUser = &pShellUsers->List[UserID];

    if (WSH_SHELL_STRNCMP(pUser->Login, pcLogin, loginLen) != 0)
        return false;

    WshShell_Char_t locSaltPassHash[WSH_SHELL_SALT_PASS_HASH_LEN + 1] = {0};
    pShellUsers->Hash(pUser->Salt, pcPass, locSaltPassHash);

    if (WSH_SHELL_STRNCMP(locSaltPassHash, pUser->Hash, WSH_SHELL_SALT_PASS_HASH_LEN + 1) != 0)
        return false;

    return true;
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
```


