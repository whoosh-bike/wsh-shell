

# File wsh\_shell\_user.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_user.h**](wsh__shell__user_8h.md)

[Go to the documentation of this file](wsh__shell__user_8h.md)


```C++

#ifndef __WSH_SHELL_USER_H
#define __WSH_SHELL_USER_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const WshShell_Char_t* Login; 
    const WshShell_Char_t* Pass;  
    WshShell_Size_t Groups;       
    WshShell_Size_t Rights;       
} WshShellUser_t;

typedef struct {
    const WshShellUser_t* List; 
    WshShell_Size_t Num;        
} WshShellUser_Table_t;

WSH_SHELL_RET_STATE_t WshShellUser_Attach(WshShellUser_Table_t* pShellUsers,
                                          const WshShellUser_t* pcUserTable,
                                          WshShell_Size_t userNum);

void WshShellUser_DeAttach(WshShellUser_Table_t* pShellUsers);

WshShell_Size_t WshShellUser_GetUsersNum(WshShellUser_Table_t* pShellUsers);

const WshShellUser_t* WshShellUser_GetUserByIndex(WshShellUser_Table_t* pShellUsers,
                                                  WshShell_Size_t idx);

WshShell_Bool_t WshShellUser_CheckCredentials(WshShellUser_Table_t* pShellUsers,
                                              WshShell_Size_t UserID,
                                              const WshShell_Char_t* pcLogin,
                                              const WshShell_Char_t* pcPassword);

const WshShellUser_t* WshShellUser_FindByCredentials(WshShellUser_Table_t* pShellUsers,
                                                     const WshShell_Char_t* pcLogin,
                                                     const WshShell_Char_t* pcPass);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_USER_H */
```


