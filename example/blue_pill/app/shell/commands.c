#include "commands.h"

extern const WshShellCmd_t Shell_LedCmd;

static const WshShellCmd_t* Shell_CmdTable[] = {
    &Shell_LedCmd,
};

bool Shell_CmdTable_Init(WshShell_t* pShell) {
    return WshShellRetState_TranslateToProject(
        WshShellCmd_Attach(&(pShell->Commands), Shell_CmdTable, 1));
}
