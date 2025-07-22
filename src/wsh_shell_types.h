#ifndef __WSH_SHELL_TYPES_H
#define __WSH_SHELL_TYPES_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t WshShell_S8_t;
typedef int32_t WshShell_S32_t;
typedef uint32_t WshShell_U32_t;
typedef uint16_t WshShell_U16_t;
typedef uint8_t WshShell_U8_t;
typedef size_t WshShell_Size_t;

/* clang-format off */

#define WSH_SHELL_RETURN_IF_UNSUCCESS(a) if ((a) != WSH_SHELL_RET_STATE_SUCCESS) return (a)
#define WSH_SHELL_ARR_LEN(a) (sizeof((a)) / sizeof((a[0])))

#define WSH_SHELL_RET_STATE_TABLE() \
X(WSH_SHELL_RET_STATE_UNDEF, "UNDEF") \
X(WSH_SHELL_RET_STATE_SUCCESS, "SUCCESS") \
X(WSH_SHELL_RET_STATE_ERR_MEMORY, "ERR_MEMORY") \
X(WSH_SHELL_RET_STATE_ERR_CRC, "ERR_CRC") \
X(WSH_SHELL_RET_STATE_ERR_EMPTY, "ERR_EMPTY") \
X(WSH_SHELL_RET_STATE_ERR_PARAM, "ERR_PARAM") \
X(WSH_SHELL_RET_STATE_ERR_BUSY, "ERR_BUSY") \
X(WSH_SHELL_RET_STATE_ERR_OVERFLOW, "ERR_OVERFLOW") \
X(WSH_SHELL_RET_STATE_ERR_TIMEOUT, "ERR_TIMEOUT") \
X(WSH_SHELL_RET_STATE_ERROR, "ERROR") \
X(WSH_SHELL_RET_STATE_WARNING, "WARNING")

/* clang-format on */

#define X(a, b) a,
typedef enum { WSH_SHELL_RET_STATE_TABLE() } WSH_SHELL_RET_STATE_t;
#undef X

#define X(a, b) b,
static const char* RetStateBuff[] = {WSH_SHELL_RET_STATE_TABLE()};
#undef X

static inline const char* WshShell_RetStateStr_Get(WSH_SHELL_RET_STATE_t retState) {
    return RetStateBuff[retState];
}

#endif /* __WSH_SHELL_TYPES_H */
