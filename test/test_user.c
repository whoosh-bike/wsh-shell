#include "et.h"
#include "wsh_shell_cfg_def.h"
#include "wsh_shell_user.h"

#define TEST_USER_ARR_LEN 2

static const WshShellUser_t TestUserArr[TEST_USER_ARR_LEN] = {
    {
        .pLogin = "admin",
        .pPwd   = "admin123",
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_ACCESS_ANY,
    },
    {
        .pLogin = "root",
        .pPwd   = "root123",
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_ACCESS_ANY,
    },
};

void setup(void) {
    /* executed before *every* non-skipped test */
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
}

/* test group --------------------------------------------------------------*/
TEST_GROUP("User") {
    TEST("Init users") {
        VERIFY(WshShellUser_GetUsersNum() == 0);
        VERIFY(WshShellUser_Init(TestUserArr, WSH_SHELL_ARR_LEN(TestUserArr)) == WSH_SHELL_RET_STATE_SUCCESS);
        VERIFY(WshShellUser_GetUsersNum() == TEST_USER_ARR_LEN);
        for (WshShell_Size_t i = 0; i < TEST_USER_ARR_LEN; i++) {
            const WshShellUser_t* pUser = WshShellUser_GetUserByIndex(i);
            VERIFY(pUser != NULL);
            VERIFY(WSH_SHELL_MEMCMP(pUser, &TestUserArr[i], sizeof(WshShellUser_t)) == 0);
        }
    }

    TEST("Check credentials") {
        const char* pLogin = "admin";
        const char* pPwd   = "admin123";
        VERIFY(WshShellUser_CheckCredentials(0, pLogin, pPwd));
        VERIFY(WshShellUser_CheckCredentials(0, "wrongLogin", pPwd) == false);
        VERIFY(WshShellUser_CheckCredentials(0, pLogin, "wrongPwd") == false);
        VERIFY(WshShellUser_CheckCredentials(0, "testUserLoginThatIsVeryVeryLongButCorrectInTheBeginning", pPwd) ==
               false);
        VERIFY(WshShellUser_CheckCredentials(0, pLogin, "testUserPasswordThatIsVeryVeryLongButCorrectInTheBeginning") ==
               false);
    }

    TEST("Destroy users") {
        WshShellUser_Destroy();
        VERIFY(WshShellUser_GetUsersNum() == 0);
    }

} /* TEST_GROUP() */
