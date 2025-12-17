

# File wsh\_shell\_history.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_history.h**](wsh__shell__history_8h.md)

[Go to the documentation of this file](wsh__shell__history_8h.md)


```C++

#ifndef __WSH_SHELL_HISTORY_H
#define __WSH_SHELL_HISTORY_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_misc.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WSH_SHELL_HIST_CMD_PREV = 0, 
    WSH_SHELL_HIST_CMD_NEXT      
} WSH_SHELL_HIST_CMD_DIR_t;

typedef struct {
    WshShell_Char_t StorageBuff[WSH_SHELL_HISTORY_BUFF_SIZE]; 
    WshShell_Size_t HeadIdx;                                  
    WshShell_Size_t TailIdx;                                  
    WshShell_Size_t LastSavedCmdIdx;  
    WSH_SHELL_HIST_CMD_DIR_t PrevDir; 
    WshShell_Bool_t LimitIsReached;   
} WshShellHistory_Data_t;

typedef struct {
    WshShell_U32_t Hash;         
    WshShellHistory_Data_t Data; 
} WshShellHistory_t;

typedef WshShellHistory_t (*WshShellHistory_ReadHandler_t)(void);

typedef void (*WshShellHistory_WriteHandler_t)(WshShellHistory_t);

typedef struct {
    WshShellHistory_ReadHandler_t Read;   
    WshShellHistory_WriteHandler_t Write; 
} WshShellHistoryIO_t;

void WshShellHistory_Init(WshShellHistoryIO_t* pHistIO, WshShellHistory_ReadHandler_t readFn,
                          WshShellHistory_WriteHandler_t writeFn);

void WshShellHistory_SaveCmd(WshShellHistoryIO_t* pHistIO, const WshShell_Char_t* pcCmdStr,
                             WshShell_Size_t cmdStrLen);

WshShell_Size_t WshShellHistory_GetPrevCmd(WshShellHistoryIO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize);

WshShell_Size_t WshShellHistory_GetNextCmd(WshShellHistoryIO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize);

WshShell_Size_t WshShellHistory_GetTokenNum(WshShellHistoryIO_t* pHistIO);

WshShell_Size_t WshShellHistory_GetTokenByIndex(WshShellHistoryIO_t* pHistIO,
                                                WshShell_Char_t* pOutBuff,
                                                WshShell_Size_t outBuffSize, WshShell_Size_t index);

void WshShellHistory_Flush(WshShellHistoryIO_t* pHistIO);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_HISTORY_H */
```


