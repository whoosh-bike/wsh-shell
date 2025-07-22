/**
 * @file wsh_shell.h
 * @brief User object definition and API's
 * @author Whoosh Embedded Team
 *
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_H
#define __WSH_SHELL_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_io.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"

/* clang-format off */
#if WSH_SHELL_PRINT_INFO_ENABLE
#define WSH_SHELL_PRINT_INFO(_f_, ...) WSH_SHELL_PRINT(WSH_SHELL_COLOR_INFO _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__)
#else /* WSH_SHELL_PRINT_INFO */
#define WSH_SHELL_PRINT_INFO(_f_, ...)
#endif /* WSH_SHELL_PRINT_INFO */

#if WSH_SHELL_PRINT_WARN_ENABLE
#define WSH_SHELL_PRINT_WARN(_f_, ...) WSH_SHELL_PRINT(WSH_SHELL_COLOR_WARN _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__);
#else /* WSH_SHELL_PRINT_WARN_ENABLE */
#define WSH_SHELL_PRINT_WARN(_f_, ...)
#endif /* WSH_SHELL_PRINT_WARN_ENABLE */

#if WSH_SHELL_PRINT_ERR_ENABLE
#define WSH_SHELL_PRINT_ERR(_f_, ...) WSH_SHELL_PRINT(WSH_SHELL_COLOR_ERROR _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__);
#else /* WSH_SHELL_PRINT_ERR_ENABLE */
#define WSH_SHELL_PRINT_ERR(_f_, ...)
#endif /* WSH_SHELL_PRINT_ERR_ENABLE */

#define WSH_SHELL_PRINT_INTRO() WSH_SHELL_PRINT(WSH_SHELL_COLOR_INTRO WSH_SHELL_INTRO WSH_SHELL_ESC_RESET_STYLE)
/* clang-format on */

/**
 * @ingroup WshShell
 * @struct WshShell_t
 * @brief WshShell main structure.
 */
typedef struct {
    WshShell_Size_t InterLineLength;                /**< Length of Interactive buffer. */
    char InteractiveBuff[WSH_SHELL_INTR_BUFF_SIZE]; /**< Interactive buffer. */
    WshShell_Size_t CursorPos;                      /**< Cursor position. */
    const WshShellUser_t* pUser;                    /**< Current active user */
    const WshShellCmd_t* pIntCmd;                   /**< Current active interactive command */
    char pLogin[WSH_SHELL_USER_LOGIN_MAX_LEN];      /**< Login buffer for authentication */
    char pPwd[WSH_SHELL_USER_PASS_MAX_LEN];         /**< Password buffer for authentication */
    WshShell_Size_t EscCnt;                         /**< Counter for escape sequence characters */
    char EscBuff[WSH_SHELL_ESC_BUFF_LEN];           /**< Buffer for escape sequence characters */
} WshShell_t;

/**
 * @defgroup PublicShellFunc Public functions
 * @brief Public shell functions.
 * @{
 */

/**
 * @brief Initialize shell.
 * 
 * @param[in] pShellStorage: Pointer to a shell storage structure.
 * 
 * @retval WSH_SHELL_RET_STATE_ERR_PARAM: Something wrong with input arguments.
 * @retval WSH_SHELL_RET_STATE_SUCCESS: Success.
 */
WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShellStorage);

/**
 * @brief Print invitation.
 */
void WshShell_InvitationPrint(void);

/**
 * @brief Set interactive command.
 * @param[in] pCmd: Pointer to a command.
 */
void WshShell_SetIntCmd(const WshShellCmd_t* pCmd);

/**
 * @brief Authorize with provided credentials.
 * 
 * @param[in] pLogin: Pointer to a login string.
 * @param[in] pPwd: Pointer to a password string.
 */
void WshShell_Auth(const char* pLogin, const char* pPwd);

/**
 * @brief Check if any user authorized.
 * 
 * @retval false: No authorized users.
 * @retval true: Somebody is authorized.
 */
bool WshShell_IsAuth(void);

/**
 * @brief Exit from shell authorization.
 */
void WshShell_Exit(void);

/**
 * @brief Main shell input processing function.
 * @param[in] symbol: Next input character.
 */
void WshShell_InsertChar(char symbol);

/**
 * @brief Deinit current shell instance.
 */
void WshShell_Destroy(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* __WSH_SHELL_H */
