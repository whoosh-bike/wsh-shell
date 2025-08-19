

# File wsh\_shell\_cfg\_def.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cfg\_def.h**](wsh__shell__cfg__def_8h.md)

[Go to the documentation of this file](wsh__shell__cfg__def_8h.md)


```C++
#ifndef __WSH_SHELL_CFG_H
#define __WSH_SHELL_CFG_H

#include "wsh_shell_types.h"

/* 
 * ─────────────────────────────────────────────
 * Shell welcome banner (ASCII logo)
 * ───────────────────────────────────────────── 
 */

/* clang-format off */
#define WSH_SHELL_HEADER "\
                __               __         ____  \r\n\
 _      _______/ /_        _____/ /_  ___  / / /  \r\n\
| | /| / / ___/ __ \\______/ ___/ __ \\/ _ \\/ / /\r\n\
| |/ |/ (__  ) / / /_____(__  ) / / /  __/ / /    \r\n\
|__/|__/____/_/ /_/     /____/_/ /_/\\___/_/_/    \r\n\
\r\n"
/* clang-format on */

/* 
 * ─────────────────────────────────────────────
 * Print objects customization
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_PRINT_SYS_ENABLE  1
#define WSH_SHELL_PRINT_INFO_ENABLE 1
#define WSH_SHELL_PRINT_WARN_ENABLE 1
#define WSH_SHELL_PRINT_ERR_ENABLE  1

/* 
 * ─────────────────────────────────────────────
 * Default shell command (executed at startup)
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_DEF_COMMAND 1

/* 
 * ─────────────────────────────────────────────
 * Command descriptions(help) - store and print
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_PRINT_OPT_HELP_ENABLE  1
#define WSH_SHELL_CMD_PRINT_OPT_OVERVIEW 1

/* 
 * ─────────────────────────────────────────────
 * Interactive mode (user input enabled)
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_INTERACTIVE_MODE 1

/* 
 * ─────────────────────────────────────────────
 * Command history
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_HISTORY           1
#define WSH_SHELL_HISTORY_BUFF_SIZE 256

/* 
 * ─────────────────────────────────────────────
 * Command autocompletion
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_AUTOCOMPLETE         1
#define WSH_SHELL_AUTOCOMPLETE_PAD_LEN 32
#define WSH_SHELL_AUTOCOMPLETE_PAD_SYM '.'

/* 
 * ─────────────────────────────────────────────
 * PS1 prompt configuration
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_PS1_CUSTOM   1
#define WSH_SHELL_PS1_TEMPLATE "%r%b%c6%d%c7@%c5%u%c2%i %c7> %r%c7"
#define WSH_SHELL_PS1_MAX_LEN  128

/* 
 * ─────────────────────────────────────────────
 * Prompt waiting
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_PROMPT_WAIT 1

/* 
 * ─────────────────────────────────────────────
 * Command groups & access control
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_CMD_GROUP_NONE      0x00
#define WSH_SHELL_CMD_GROUP_ALL       ((WshShell_Size_t)(~0U))
#define WSH_SHELL_CMD_GROUP_MAX_COUNT 4

#define WSH_SHELL_CMD_GROUP_ADMIN  0x01
#define WSH_SHELL_CMD_GROUP_READER 0x02

#define WSH_SHELL_USER_GROUP_ADMIN  (WSH_SHELL_CMD_GROUP_ALL)
#define WSH_SHELL_USER_ACCESS_ADMIN (WSH_SHELL_OPT_ACCESS_ANY)

#define WSH_SHELL_USER_GROUP_READER  (WSH_SHELL_CMD_GROUP_READER)
#define WSH_SHELL_USER_ACCESS_READER (WSH_SHELL_OPT_ACCESS_ANY)

/* 
 * ─────────────────────────────────────────────
 * Print helper macro
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_PRINT(_f_, ...)   \
    do {                            \
        printf(_f_, ##__VA_ARGS__); \
        fflush(stdout);             \
    } while (0)

/* 
 * ─────────────────────────────────────────────
 * General buffer and length settings
 * ───────────────────────────────────────────── 
 */
#define WSH_SHELL_DEV_NAME_LEN          16  // Device name max length
#define WSH_SHELL_OPTION_SHORT_NAME_LEN 2   // Short option name length
#define WSH_SHELL_OPTION_LONG_NAME_LEN  16  // Long option name length
#define WSH_SHELL_CMD_OPTIONS_MAX_NUM   16  // Max options per command
#define WSH_SHELL_CMD_NAME_LEN          16  // Command name max length
#define WSH_SHELL_CMD_ARGS_MAX_NUM      16  // Max arguments per command
#define WSH_SHELL_LOGIN_LEN             16  // Max username length
#define WSH_SHELL_PASS_LEN              16  // Max password length
#define WSH_SHELL_INTR_BUFF_LEN         64  // Interactive input buffer
#define WSH_SHELL_ESC_BUFF_LEN          8   // Escape sequence buffer

/* 
 * ─────────────────────────────────────────────
 * Assertions (debug only)
 * ───────────────────────────────────────────── 
 */
#ifdef WSH_SHELL_ASSERT_ENABLE
    #include <assert.h>
    #include <signal.h>
    #ifndef WSH_SHELL_ASSERT
        #define WSH_SHELL_ASSERT(exp) \
            do {                      \
                if (!(exp)) {         \
                    raise(SIGTRAP);   \
                    while (1) {       \
                    }                 \
                }                     \
            } while (0)
    #endif
#else
    #ifndef WSH_SHELL_ASSERT
        #define WSH_SHELL_ASSERT(exp)
    #endif
#endif

/* 
 * ─────────────────────────────────────────────
 * Standard library wrappers (can be replaced)
 * ───────────────────────────────────────────── 
 */
#ifndef WSH_SHELL_MEMSET
    #define WSH_SHELL_MEMSET(pD, c, sz) memset((pD), (c), (sz))
#endif

#ifndef WSH_SHELL_MEMCPY
    #define WSH_SHELL_MEMCPY(pD, pS, sz) memcpy((pD), (pS), (sz))
#endif

#ifndef WSH_SHELL_MEMCMP
    #define WSH_SHELL_MEMCMP(pD, pS, sz) memcmp((pD), (pS), (sz))
#endif

#ifndef WSH_SHELL_STRCPY
    #define WSH_SHELL_STRCPY(pD, pS) strcpy((pD), (pS))
#endif

#ifndef WSH_SHELL_STRNCPY
    #define WSH_SHELL_STRNCPY(pD, pS, sz) strncpy((pD), (pS), (sz))
#endif

#ifndef WSH_SHELL_STRLEN
    #define WSH_SHELL_STRLEN(pS) strlen((pS))
#endif

#ifndef WSH_SHELL_STRNLEN
    #define WSH_SHELL_STRNLEN(pS, len) strnlen((pS), (len))
#endif

#ifndef WSH_SHELL_STRCMP
    #define WSH_SHELL_STRCMP(pS1, pS2) strcmp((pS1), (pS2))
#endif

#ifndef WSH_SHELL_STRNCMP
    #define WSH_SHELL_STRNCMP(pS1, pS2, len) strncmp((pS1), (pS2), (len))
#endif

#ifndef WSH_SHELL_STRTOL
    #define WSH_SHELL_STRTOL(pS, pE, radix) strtol((pS), (pE), (radix))
#endif

#ifndef WSH_SHELL_STRTOF
    #define WSH_SHELL_STRTOF(pN, pE) strtof((pN), (pE))
#endif

#ifndef WSH_SHELL_SNPRINTF
    #define WSH_SHELL_SNPRINTF(buf, size, ...) snprintf((buf), (size), __VA_ARGS__)
#endif

/* 
 * ─────────────────────────────────────────────
 * Project-to-shell return state mapping
 * ───────────────────────────────────────────── 
 */
#define RET_STATE_MAP_TABLE()                            \
    X_MAP_ENTRY(true, WSH_SHELL_RET_STATE_SUCCESS)       \
    X_MAP_ENTRY(true, WSH_SHELL_RET_STATE_WARNING)       \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERROR)        \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_EMPTY)    \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_PARAM)    \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_BUSY)     \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_OVERFLOW) \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_MEMORY)   \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_TIMEOUT)  \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_ERR_CRC)      \
    X_MAP_ENTRY(false, WSH_SHELL_RET_STATE_UNDEF)

static inline bool WshShellRetState_TranslateToProject(WSH_SHELL_RET_STATE_t state) {
    switch (state) {
#define X_MAP_ENTRY(proj, shell) \
    case shell:                  \
        return proj;
        RET_STATE_MAP_TABLE()
#undef X_MAP_ENTRY
        default:
            return false;
    }
}

#endif /* __WSH_SHELL_CFG_H */
```


