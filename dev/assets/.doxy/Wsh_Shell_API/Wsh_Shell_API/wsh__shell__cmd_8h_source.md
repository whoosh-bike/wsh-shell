

# File wsh\_shell\_cmd.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cmd.h**](wsh__shell__cmd_8h.md)

[Go to the documentation of this file](wsh__shell__cmd_8h.md)


```C++

#ifndef __WSH_SHELL_CMD_H
#define __WSH_SHELL_CMD_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_option.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of command structure
struct WshShellCmd;

typedef WSH_SHELL_RET_STATE_t (*WshShellCmdHandler_t)(const struct WshShellCmd* pcCmd,
                                                      WshShell_Size_t argc,
                                                      const WshShell_Char_t* pArgv[],
                                                      void* pShellCtx);

typedef struct WshShellCmd {
    const WshShell_Char_t* Name;  
    const WshShell_Char_t* Descr; 
    WshShell_Size_t Groups; 
    const WshShellOption_t* Options; 
    WshShell_Size_t OptNum;          
    WshShellCmdHandler_t Handler;    
} WshShellCmd_t;

typedef struct {
    const WshShellCmd_t** List; 
    WshShell_Size_t Num;        
} WshShellCmd_Table_t;

WSH_SHELL_RET_STATE_t WshShellCmd_Attach(WshShellCmd_Table_t* pShellCommands,
                                         const WshShellCmd_t* pcCmdTable[], WshShell_Size_t cmdNum);

void WshShellCmd_DeAttach(WshShellCmd_Table_t* pShellCommands);

WshShell_Size_t WshShellCmd_GetCmdNum(WshShellCmd_Table_t* pShellCommands);

const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShellCmd_Table_t* pShellCommands,
                                               WshShell_Size_t idx);

const WshShellCmd_t* WshShellCmd_SearchCmd(WshShellCmd_Table_t* pShellCommands,
                                           const WshShell_Char_t* pcCmdName);

WshShellOption_Ctx_t WshShellCmd_ParseOpt(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                          const WshShell_Char_t* pArgv[], WshShell_Size_t rights,
                                          WshShell_Size_t* pTokenPos);

WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellOption_Ctx_t* pOptCtx, WshShell_Size_t argc,
                                              const WshShell_Char_t* pArgv[],
                                              WshShell_Size_t valueSize, void* pValue);

void WshShellCmd_PrintOptionsOverview(const WshShellCmd_t* pcCmd);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_CMD_H */
```


