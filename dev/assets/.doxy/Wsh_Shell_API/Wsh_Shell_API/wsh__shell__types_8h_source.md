

# File wsh\_shell\_types.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_types.h**](wsh__shell__types_8h.md)

[Go to the documentation of this file](wsh__shell__types_8h.md)


```C++

#ifndef __WSH_SHELL_TYPES_H
#define __WSH_SHELL_TYPES_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool WshShell_Bool_t;      // just for better
typedef char WshShell_Char_t;      // readability
typedef float WshShell_Float_t;    // and common
typedef double WshShell_Double_t;  // perception
typedef int8_t WshShell_S8_t;
typedef uint8_t WshShell_U8_t;
typedef int32_t WshShell_S32_t;
typedef uint32_t WshShell_U32_t;
typedef int16_t WshShell_S16_t;
typedef uint16_t WshShell_U16_t;
typedef size_t WshShell_Size_t;

#define WSH_SHELL_ARR_LEN(a) (sizeof((a)) / sizeof((a[0])))

#define WSH_SHELL_RET_STATE_TABLE()                           \
    X_ENTRY(WSH_SHELL_RET_STATE_UNDEF, "UNDEF")               \
    X_ENTRY(WSH_SHELL_RET_STATE_SUCCESS, "SUCCESS")           \
    X_ENTRY(WSH_SHELL_RET_STATE_WARNING, "WARNING")           \
    X_ENTRY(WSH_SHELL_RET_STATE_ERROR, "ERROR")               \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_EMPTY, "ERR_EMPTY")       \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_PARAM, "ERR_PARAM")       \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_BUSY, "ERR_BUSY")         \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_OVERFLOW, "ERR_OVERFLOW") \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_MEMORY, "ERR_MEMORY")     \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_TIMEOUT, "ERR_TIMEOUT")   \
    X_ENTRY(WSH_SHELL_RET_STATE_ERR_CRC, "ERR_CRC")

typedef enum {
#define X_ENTRY(en, str) en,
    WSH_SHELL_RET_STATE_TABLE() WSH_SHELL_RET_STATE_ENUM_SIZE
#undef X_ENTRY
} WSH_SHELL_RET_STATE_t;

static const WshShell_Char_t* RetStateBuff[] = {
#define X_ENTRY(en, str) str,
    WSH_SHELL_RET_STATE_TABLE()
#undef X_ENTRY
};

static inline const WshShell_Char_t* WshShell_GetRetStateStr(WSH_SHELL_RET_STATE_t retState) {
    if (retState >= WSH_SHELL_RET_STATE_ENUM_SIZE)
        retState = WSH_SHELL_RET_STATE_UNDEF;

    return RetStateBuff[retState];
}

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_TYPES_H */
```


