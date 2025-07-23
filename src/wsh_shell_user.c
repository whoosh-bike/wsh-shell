#include "wsh_shell_user.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_types.h"

typedef struct {
    const WshShellUser_t* pUser;
    WshShell_Size_t Num;
} WshShellUser_Table_t;

static WshShellUser_Table_t UserTable = {0};

WSH_SHELL_RET_STATE_t WshShellUser_Init(const WshShellUser_t* pUserTable, WshShell_Size_t usersNum) {
    WSH_SHELL_ASSERT(pUserTable != NULL);
    WSH_SHELL_ASSERT(usersNum != 0);
    if (pUserTable == NULL || usersNum == 0)
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    UserTable.pUser = pUserTable;
    UserTable.Num   = usersNum;
    return WSH_SHELL_RET_STATE_SUCCESS;
}

WshShell_Size_t WshShellUser_GetUsersNum(void) {
    return UserTable.Num;
}

const WshShellUser_t* WshShellUser_GetUserByIndex(WshShell_Size_t id) {
    WSH_SHELL_ASSERT(id < UserTable.Num);
    return id < UserTable.Num ? &UserTable.pUser[id] : NULL;
}

void WshShellUser_Destroy(void) {
    UserTable = (WshShellUser_Table_t){0};
}

bool WshShellUser_CheckCredentials(WshShell_Size_t UserID, const char* pLogin, const char* pPassword) {
    WSH_SHELL_ASSERT(UserID < UserTable.Num);
    WSH_SHELL_ASSERT(pLogin != NULL);
    WSH_SHELL_ASSERT(pPassword != NULL);

    if (UserID >= UserTable.Num || pLogin == NULL || pPassword == NULL)
        return false;

    bool result = false;
    if (WSH_SHELL_STRNCMP(UserTable.pUser[UserID].pLogin, pLogin, WSH_SHELL_LOGIN_MAX_LEN) == 0 &&
        WSH_SHELL_STRNCMP(UserTable.pUser[UserID].pPwd, pPassword, WSH_SHELL_PASS_MAX_LEN) == 0) {
        result = true;
    }

    return result;
}
