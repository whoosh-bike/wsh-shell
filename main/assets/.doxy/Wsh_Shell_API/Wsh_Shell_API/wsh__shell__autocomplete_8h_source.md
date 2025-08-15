

# File wsh\_shell\_autocomplete.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_autocomplete.h**](wsh__shell__autocomplete_8h.md)

[Go to the documentation of this file](wsh__shell__autocomplete_8h.md)


```C++

#ifndef __WSH_SHELL_AUTOCOMPLETE_H
#define __WSH_SHELL_AUTOCOMPLETE_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_io.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

WshShell_Bool_t WshShellAutocomplete_Try(WshShell_Char_t* pInBuff, WshShell_Size_t inBuffLen,
                                         WshShellCmd_Table_t* pShellCommands);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_AUTOCOMPLETE_H */
```


