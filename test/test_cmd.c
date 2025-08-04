#include "et.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_types.h"

#define TEST_CMD_ARR_LEN 4

/* TODO: test those functions */
// WshShellCmd_Destroy();
// WshShellCmd_ParseOpt();
// WshShellCmd_GetOptValue();

#define X(en, m) {en, m},
static const WshShellOption_t HelpOptArr[] = {WSH_SHELL_HELP_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t ClsOptArr[] = {WSH_SHELL_HELP_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t EchoOptArr[] = {WSH_SHELL_HELP_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t ExitOptArr[] = {WSH_SHELL_HELP_OPT_TABLE()};
#undef X

static const WshShellCmd_t TestCmdArr[TEST_CMD_ARR_LEN] = {
    {
        .pName    = "help",
        .pHelp    = "Help test cmd help",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = HelpOptArr,
        .OptNum   = WSH_SHELL_HELP_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Help_Executable,
    },
    {
        .pName    = "cls",
        .pHelp    = "Cls test cmd help",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = ClsOptArr,
        .OptNum   = WSH_SHELL_CLS_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Cls_Executable,
    },
    {
        .pName    = "echo",
        .pHelp    = "Echo test cmd help",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = EchoOptArr,
        .OptNum   = WSH_SHELL_ECHO_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Echo_Executable,
    },
    {
        .pName    = "exit",
        .pHelp    = "Exit test cmd help",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = ExitOptArr,
        .OptNum   = WSH_SHELL_EXIT_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Exit_Executable,
    },
};

void setup(void) {
    /* executed before *every* non-skipped test */
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
}

TEST_GROUP("Commands") {
    TEST("Init commands") {
        VERIFY(WshShellCmd_GetCmdNum() == 0);
        VERIFY(WshShellCmd_Init(TestCmdArr, WSH_SHELL_ARR_LEN(TestCmdArr)) == WSH_SHELL_RET_STATE_SUCCESS);
        VERIFY(WshShellCmd_GetCmdNum() == TEST_CMD_ARR_LEN);
        for (WshShell_Size_t i = 0; i < TEST_CMD_ARR_LEN; i++) {
            const WshShellCmd_t* pCmd = WshShellCmd_GetCmdByIndex(i);
            VERIFY(pCmd != NULL);
            VERIFY(WSH_SHELL_MEMCMP(pCmd, &TestCmdArr[i], sizeof(WshShellCmd_t)) == 0);
        }
    }

    TEST("Destroy commands") {
        WshShellCmd_Destroy();
        VERIFY(WshShellCmd_GetCmdNum() == 0);
    }
} /* TEST_GROUP() */
