#include "stdio.h"
#include "wsh_shell.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"

static const WshShellUser_t UserArr[] = {{
    .pLogin = "root",
    .pPwd   = "root",
    .Groups = WSH_SHELL_CMD_GROUP_ALL,
    .Rights = WSH_SHELL_ACCESS_ANY,
}};

#define X(en, m) {en, m},
static const WshShellOption_t HelpOptArr[] = {WSH_SHELL_HELP_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t ClsOptArr[] = {WSH_SHELL_CLS_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t EchoOptArr[] = {WSH_SHELL_ECHO_OPT_TABLE()};
#undef X

#define X(en, m) {en, m},
static const WshShellOption_t ExitOptArr[] = {WSH_SHELL_EXIT_OPT_TABLE()};
#undef X

static const WshShellCmd_t CmdArr[] = {
    {
        .pName    = "help",
        .pHelp    = "List available commands",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = HelpOptArr,
        .OptNum   = WSH_SHELL_HELP_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Help_Executable,
    },
    {
        .pName    = "cls",
        .pHelp    = "Clear screen",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = ClsOptArr,
        .OptNum   = WSH_SHELL_CLS_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Cls_Executable,
    },
    {
        .pName    = "echo",
        .pHelp    = "Echo input",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = EchoOptArr,
        .OptNum   = WSH_SHELL_ECHO_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Echo_Executable,
    },
    {
        .pName    = "exit",
        .pHelp    = "Exit from curren user session",
        .Group    = WSH_SHELL_CMD_GROUP_ALL,
        .pOptions = ExitOptArr,
        .OptNum   = WSH_SHELL_EXIT_OPT_ENUM_SIZE,
        .Exec     = WshShellCmdDef_Exit_Executable,
    },
};

#define DEVICE_NAME
int main(void) {
    WshShell_t Shell = {0};
    WshShellCmd_Init(CmdArr, WSH_SHELL_ARR_LEN(CmdArr));
    WshShellUser_Init(UserArr, WSH_SHELL_ARR_LEN(UserArr));
    WshShell_Init(&Shell);
    int symbol = 0;
    WSH_SHELL_PRINT_INTRO();
    WshShell_InvitationPrint();
    for (;;) {
        symbol = getchar();
        WshShell_InsertChar(symbol);
        symbol = 0;
    }

    return 0;
}
