

# File wsh\_shell\_str.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_str.h**](wsh__shell__str_8h.md)

[Go to the documentation of this file](wsh__shell__str_8h.md)


```C++

#ifndef __WSH_SHELL_STR_H
#define __WSH_SHELL_STR_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_option.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WSH_SHELL_PRESS_ENTER_TO_LOG_IN_STR "Press <Enter> to log in..."

typedef struct {
    const WshShell_Char_t* UserName;
    const WshShell_Char_t* DevName;
    WshShell_Char_t* InterCmdName;
} WshShellStr_PS1Data_t;

WshShell_Bool_t WshShellStr_IsPrintableAscii(WshShell_Char_t ch);

WshShell_Char_t* WshShellStr_TrimString(WshShell_Char_t* pString, WshShell_Size_t len);

void WshShellStr_ParseToArgcArgv(WshShell_Char_t* pStr, WshShell_Size_t* pArgNum,
                                 const WshShell_Char_t* pArgBuff[], WshShell_Size_t maxArgNum);

void WshShellStr_IncrInterCnt(WshShell_Size_t* pInterCnt, WshShell_Size_t buffSize);

void WshShellStr_DecrInterCnt(WshShell_Size_t* pInterCnt);

void WshShellStr_AccessBitsToStr(WshShell_Size_t access, WshShell_Char_t* pOutStr);

void WshShellStr_GroupBitsToStr(WshShell_Size_t group, WshShell_Char_t* pOutStr);

void WshShellStr_GeneratePS1(WshShell_Char_t* pPS1, WshShellStr_PS1Data_t* pPS1Data);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_STR_H */
```


