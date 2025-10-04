

# File wsh\_shell.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell.h**](wsh__shell_8h.md)

[Go to the documentation of this file](wsh__shell_8h.md)


```C++

#ifndef __WSH_SHELL_H
#define __WSH_SHELL_H

#include "wsh_shell_autocomplete.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_esc.h"
#include "wsh_shell_history.h"
#include "wsh_shell_interact.h"
#include "wsh_shell_io.h"
#include "wsh_shell_promptwait.h"
#include "wsh_shell_ps1_custom.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"
#include "wsh_shell_version.h"

/* detect operating system name */
#if defined(__linux)
    #define OS_NAME "Linux"
#elif defined(__unix)
    #define OS_NAME "Unix"
#elif defined(__APPLE__)
    #define OS_NAME "Darwin"
#elif defined(_WIN32)
    #define OS_NAME "Windows"
#else
    #define OS_NAME "Unknown OS"
#endif

/* detect compiler name and version */
#if defined(__clang__)
    #define COMPILER "clang " __clang_version__
#elif defined(__GNUC__)
    #define COMPILER "GCC " __VERSION__
#elif defined(_MSC_VER)
    #define COMPILER "MSVC"
#elif defined(__CC_ARM)
    #define COMPILER "ARMCC"
#elif defined(__ICCARM__)
    #define COMPILER "IAR"
#else
    #define COMPILER "Unknown Compiler"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WshShell_ExtClbk_t)(void* pCtx);

typedef struct {
    WshShell_ExtClbk_t Auth;     
    WshShell_ExtClbk_t DeAuth;   
    WshShell_ExtClbk_t SymbolIn; 
} WshShell_ExtCallbacks_t;

typedef struct {
    WshShell_Char_t Login[WSH_SHELL_LOGIN_LEN]; 
    WshShell_Char_t Pass[WSH_SHELL_PASS_LEN];   
} WshShell_AuthContext_t;

typedef struct {
    WshShell_Char_t* Version;                           
    WshShell_Char_t DeviceName[WSH_SHELL_DEV_NAME_LEN]; 
    WshShell_Char_t PS1[WSH_SHELL_PS1_MAX_LEN];         
    WshShell_Char_t PrevSym;                            
    WshShellIO_CommandLine_t CommandLine; 
    const WshShellUser_t* CurrUser;       
    WshShell_AuthContext_t TmpAuth;       
    WshShellEsc_Storage_t EscStorage;     
    WshShellUser_Table_t Users;   
    WshShellCmd_Table_t Commands; 
    WshShellHistory_IO_t HistoryIO; 
    WshShell_Interact_t Interact;   
    WshShellPromptWait_t PromptWait;

    WshShell_ExtCallbacks_t ExtCallbacks; 
} WshShell_t;

WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShell, const WshShell_Char_t* pcDevName,
                                    const WshShell_Char_t* pcCustomHeader,
                                    WshShell_ExtCallbacks_t* pExtClbks);

WshShell_Bool_t WshShell_Auth(WshShell_t* pShell, const WshShell_Char_t* pcLogin,
                              const WshShell_Char_t* pcPass);

WshShell_Bool_t WshShell_IsAuth(WshShell_t* pShell);

void WshShell_DeAuth(WshShell_t* pShell, const WshShell_Char_t* pcReason);

void WshShell_InsertChar(WshShell_t* pShell, const WshShell_Char_t symbol);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_H */
```


