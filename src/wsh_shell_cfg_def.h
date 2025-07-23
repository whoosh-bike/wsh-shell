#ifndef __WSH_SHELL_CFG_H
#define __WSH_SHELL_CFG_H

#include "wsh_shell_types.h"

#define WSH_SHELL_HISTORY 1

#define WSH_SHELL_AUTOCOMPLETE         1
#define WSH_SHELL_AUTOCOMPLETE_PAD_LEN 32
#define WSH_SHELL_AUTOCOMPLETE_PAD_SYM '.'

#define WSH_SHELL_PRINT_SYS_ENABLE  1
#define WSH_SHELL_PRINT_INFO_ENABLE 1
#define WSH_SHELL_PRINT_WARN_ENABLE 1
#define WSH_SHELL_PRINT_ERR_ENABLE  1

#define WSH_SHELL_OPT_HELP_ENABLE 1

/* clang-format off */
#define WSH_SHELL_INTRO "\r\n\
                __               __         ____  \r\n\
 _      _______/ /_        _____/ /_  ___  / / /  \r\n\
| | /| / / ___/ __ \\______/ ___/ __ \\/ _ \\/ / /\r\n\
| |/ |/ (__  ) / / /_____(__  ) / / /  __/ / /    \r\n\
|__/|__/____/_/ /_/     /____/_/ /_/\\___/_/_/    \r\n\
\r\n"
/* clang-format on */

#define SHELL_RX_BUFF_LEN 128

#define WSH_SHELL_COLOR_INFO  WSH_SHELL_COLOR_WHITE
#define WSH_SHELL_COLOR_SYS   WSH_SHELL_COLOR_CYAN
#define WSH_SHELL_COLOR_OK    WSH_SHELL_COLOR_GREEN
#define WSH_SHELL_COLOR_WARN  WSH_SHELL_COLOR_YELLOW
#define WSH_SHELL_COLOR_ERROR WSH_SHELL_COLOR_RED
#define WSH_SHELL_COLOR_INTRO WSH_SHELL_COLOR_PURPLE

#define WSH_SHELL_PRINT(_f_, ...)   \
    do {                            \
        printf(_f_, ##__VA_ARGS__); \
        fflush(stdout);             \
    } while (0)

#define WSH_SHELL_CMD_GROUP_NONE 0x00
#define WSH_SHELL_CMD_GROUP_ALL  ((WshShell_Size_t)(~0U))

#define WSH_SHELL_ACCESS_NONE    0x00
#define WSH_SHELL_ACCESS_READ    0x01
#define WSH_SHELL_ACCESS_WRITE   0x02
#define WSH_SHELL_ACCESS_EXECUTE 0x04
#define WSH_SHELL_ACCESS_ANY     ((WshShell_Size_t)(~0U))

/**
 * @def WSH_SHELL_OPTION_SHORT_NAME_LEN
 * @brief Option short name string max length.
 */
#ifndef WSH_SHELL_OPTION_SHORT_NAME_LEN
    #define WSH_SHELL_OPTION_SHORT_NAME_LEN 2
#endif /* WSH_SHELL_OPTION_SHORT_NAME_LEN */

/**
 * @def WSH_SHELL_OPTION_LONG_NAME_LEN
 * @brief Option long name string max length.
 */
#ifndef WSH_SHELL_OPTION_LONG_NAME_LEN
    #define WSH_SHELL_OPTION_LONG_NAME_LEN 32
#endif /* WSH_SHELL_OPTION_LONG_NAME_LEN */

/**
 * @name Constants
 * Macros constants for command objects.
 */
/**@{ */

/**
 * @def WSH_SHELL_CMD_MAX_NAME_LEN
 * @brief Command name string max length.
 */
#ifndef WSH_SHELL_CMD_MAX_NAME_LEN
    #define WSH_SHELL_CMD_MAX_NAME_LEN 32
#endif /* WSH_SHELL_CMD_MAX_NAME_LEN */

/**
 * @def WSH_SHELL_CMD_ARGS_MAX_NUM
 * @brief Max amount of arguments for passing in command.
 */
#ifndef WSH_SHELL_CMD_ARGS_MAX_NUM
    #define WSH_SHELL_CMD_ARGS_MAX_NUM 16
#endif /* WSH_SHELL_CMD_ARGS_MAX_NUM */

/**
 * @def WSH_SHELL_CMD_OPTION_NUM
 * @brief Max amount of founded options in command call string.
 */
#ifndef WSH_SHELL_CMD_OPTIONS_MAX_NUM
    #define WSH_SHELL_CMD_OPTIONS_MAX_NUM 16
#endif /* WSH_SHELL_CMD_OPTIONS_MAX_NUM */

/**@} */

/**
 * @name Constans
 * Macros constants for history module.
 */
/**@{ */

/**
 * @def WSH_SHELL_HISTORY_BUFF_SIZE
 * @brief Size of a buffer for storing command calls history.
 */
#ifndef WSH_SHELL_HISTORY_BUFF_SIZE
    #define WSH_SHELL_HISTORY_BUFF_SIZE 256
#endif /* WSH_SHELL_HISTORY_BUFF_SIZE */

/**@} */

/**
 * @name Constants
 * Macros constants for user objects.
 */
/**@{ */

/**
 * @def WSH_SHELL_LOGIN_MAX_LEN
 * @brief Max length of a user name
 */
#ifndef WSH_SHELL_LOGIN_MAX_LEN
    #define WSH_SHELL_LOGIN_MAX_LEN 32
#endif /* WSH_SHELL_LOGIN_MAX_LEN */

/**
 * @def WSH_SHELL_PASS_MAX_LEN
 * @brief Max length of a user password
 */
#ifndef WSH_SHELL_PASS_MAX_LEN
    #define WSH_SHELL_PASS_MAX_LEN 32
#endif /* WSH_SHELL_PASS_MAX_LEN */

/**@} */

/**
 * @name Constants
 * Macros constants for shell objects.
 */
/**@{ */

/**
 * @def WSH_SHELL_INTR_BUFF_SIZE
 * @brief Interactive buffer size.
 */
#ifndef WSH_SHELL_INTR_BUFF_SIZE
    #define WSH_SHELL_INTR_BUFF_SIZE 64
#endif /* WSH_SHELL_INTR_BUFF_SIZE */

/**
 * @def WSH_SHELL_ESC_BUFF_LEN
 * @brief Escape sequence buffer length.
 */
#ifndef WSH_SHELL_ESC_BUFF_LEN
    #define WSH_SHELL_ESC_BUFF_LEN 8
#endif /* WSH_SHELL_ESC_BUFF_LEN */

/**@} */

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
    #endif /* WSH_SHELL_ASSERT */

#else /* WSH_SHELL_ASSERT_ENABLE */

    #ifndef WSH_SHELL_ASSERT
        #define WSH_SHELL_ASSERT(exp)
    #endif /* WSH_SHELL_ASSERT */

#endif /* WSH_SHELL_ASSERT_ENABLE */

#ifndef WSH_SHELL_MEMSET
    #define WSH_SHELL_MEMSET(pD, c, sz) memset((pD), (c), (sz))
#endif /* WSH_SHELL_MEMSET */

#ifndef WSH_SHELL_MEMCPY
    #define WSH_SHELL_MEMCPY(pD, pS, sz) memcpy((pD), (pS), (sz))
#endif /* WSH_SHELL_MEMCPY */

#ifndef WSH_SHELL_MEMCMP
    #define WSH_SHELL_MEMCMP(pD, pS, sz) memcmp((pD), (pS), (sz))
#endif /* WSH_SHELL_MEMCPY */

#ifndef WSH_SHELL_STRCPY
    #define WSH_SHELL_STRCPY(pD, pS) strcpy((pD), (pS))
#endif /* WSH_SHELL_STRCPY */

#ifndef WSH_SHELL_STRNCPY
    #define WSH_SHELL_STRNCPY(pD, pS, sz) strncpy((pD), (pS), (sz))
#endif /* WSH_SHELL_STRNCPY */

#ifndef WSH_SHELL_STRLEN
    #define WSH_SHELL_STRLEN(pS) strlen((pS))
#endif /* WSH_SHELL_STRLEN */

#ifndef WSH_SHELL_STRNLEN
    #define WSH_SHELL_STRNLEN(pS, len) strnlen((pS), (len))
#endif /* WSH_SHELL_STRNLEN */

#ifndef WSH_SHELL_STRNCMP
    #define WSH_SHELL_STRNCMP(pS1, pS2, len) strncmp((pS1), (pS2), (len))
#endif /* WSH_SHELL_STRNCMP */

#ifndef WSH_SHELL_STRTOL
    #define WSH_SHELL_STRTOL(pS, pE, radix) strtol((pS), (pE), (radix))
#endif /* WSH_SHELL_STRTOL */

#ifndef WSH_SHELL_STRTOF
    #define WSH_SHELL_STRTOF(pN, pE) strtof((pN), (pE))
#endif /* WSH_SHELL_STRTOF */

// Project-to-shell enum mapping table
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
