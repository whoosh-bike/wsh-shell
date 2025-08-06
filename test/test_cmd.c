#include "et.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_types.h"

/* TODO: test those functions */
// WshShellCmd_Destroy();
// WshShellCmd_ParseOpt();
// WshShellCmd_GetOptValue();

#define X_CMD_ENTRY(enum, opt) {enum, opt},
static const WshShellOption_t ShellDefOptArr[] = {WSH_SHELL_CMD_DEF_OPT_TABLE()};
#undef X_CMD_ENTRY

static const WshShellCmd_t TestCmdArr[] = {
    {
        .Name    = "shell",
        .Descr   = "Configuring and querying the shell interface",
        .Groups  = WSH_SHELL_CMD_GROUP_ALL,
        .Options = ShellDefOptArr,
        .OptNum  = WSH_SHELL_CMD_DEF_OPT_ENUM_SIZE,
        .Exec    = WshShellCmdDef_Executable,
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
        VERIFY(WshShellCmd_Init(TestCmdArr, WSH_SHELL_ARR_LEN(TestCmdArr)) ==
               WSH_SHELL_RET_STATE_SUCCESS);
        VERIFY(WshShellCmd_GetCmdNum() == WSH_SHELL_ARR_LEN(TestCmdArr));
        for (WshShell_Size_t i = 0; i < WSH_SHELL_ARR_LEN(TestCmdArr); i++) {
            const WshShellCmd_t* pcCmd = WshShellCmd_GetCmdByIndex(i);
            VERIFY(pCmd != NULL);
            VERIFY(WSH_SHELL_MEMCMP(pCmd, &TestCmdArr[i], sizeof(WshShellCmd_t)) == 0);
        }
    }

    TEST("Destroy commands") {
        WshShellCmd_Destroy();
        VERIFY(WshShellCmd_GetCmdNum() == 0);
    }
} /* TEST_GROUP() */
