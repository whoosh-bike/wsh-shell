

# File wsh\_shell\_session.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_session.h**](wsh__shell__session_8h.md)

[Go to the documentation of this file](wsh__shell__session_8h.md)


```C++

#ifndef __WSH_SHELL_SESSION_H
#define __WSH_SHELL_SESSION_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_misc.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    WshShell_U32_t Hash;        
    WshShell_U32_t RebootsLeft; 
    WshShell_U32_t UserIdx;     
} WshShellSession_t;

typedef WshShellSession_t (*WshShellSession_ReadHandler_t)(void);

typedef void (*WshShellSession_WriteHandler_t)(WshShellSession_t);

typedef struct {
    WshShellSession_ReadHandler_t Read;   
    WshShellSession_WriteHandler_t Write; 
} WshShellSessionIO_t;

void WshShellSession_Init(WshShellSessionIO_t* pSessIO, WshShellSession_ReadHandler_t readFn,
                          WshShellSession_WriteHandler_t writeFn);

WshShellSession_t WshShellSession_Read(WshShellSessionIO_t* pSessIO);

void WshShellSession_Store(WshShellSessionIO_t* pSessIO, WshShellSession_t session);

WshShell_Bool_t WshShellSession_IsValid(const WshShellSession_t* pcSession);

void WshShellSession_Clear(WshShellSessionIO_t* pSessIO);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_SESSION_H */
```


