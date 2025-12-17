

# File wsh\_shell\_esc.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_esc.h**](wsh__shell__esc_8h.md)

[Go to the documentation of this file](wsh__shell__esc_8h.md)


```C++

#ifndef __WSH_SHELL_ESC_H
#define __WSH_SHELL_ESC_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_history.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WshShellEsc_Hadler_t)(WshShellHistoryIO_t* pHistIO,
                                     WshShellIO_CommandLine_t* pCommandLine);

typedef struct {
    const WshShell_Char_t* Sequence; 
    WshShellEsc_Hadler_t Handler;    
} WshShellEsc_Action_t;

typedef struct {
    WshShell_Size_t Cnt;                          
    WshShell_Char_t Buff[WSH_SHELL_ESC_BUFF_LEN]; 
} WshShellEsc_Storage_t;

void WshShellEsc_Handler(WshShellHistoryIO_t* pHistIO, WshShellIO_CommandLine_t* pCommandLine,
                         WshShellEsc_Storage_t* pEscStorage, const WshShell_Char_t symbol);

void WshShellEsc_StartSeq(WshShellEsc_Storage_t* pEscStorage);

WshShell_Bool_t WshShellEsc_IsSeqStarted(WshShellEsc_Storage_t* pEscStorage);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_ESC_H */
```


