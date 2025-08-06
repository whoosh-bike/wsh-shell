/**
 * @file wsh_shell.h
 * @brief Core shell interface for command parsing, user authentication, and terminal interaction.
 * 
 * This header provides the main shell object (`WshShell_t`), core APIs for initialization,
 * input processing, user session management, and support for optional external callbacks.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_H
#define __WSH_SHELL_H

#include "wsh_shell_autocomplete.h"
#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_esc.h"
#include "wsh_shell_history.h"
#include "wsh_shell_interact.h"
#include "wsh_shell_io.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"
#include "wsh_shell_version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of external callback used in shell extension hooks.
 */
typedef void (*WshShell_ExtClbk_t)(void* pCtx);

/**
 * @brief Optional external callbacks invoked on user session changes.
 */
typedef struct {
    WshShell_ExtClbk_t Auth;     /**< Called when a user successfully authenticates. */
    WshShell_ExtClbk_t DeAuth;   /**< Called when a user is de-authenticated or logs out. */
    WshShell_ExtClbk_t SymbolIn; /**< Called when a sybol was received and inserted */
} WshShell_ExtCallbacks_t;

/**
 * @brief Temporary authentication context for login input handling.
 */
typedef struct {
    WshShell_Char_t Login[WSH_SHELL_LOGIN_LEN]; /**< Input buffer for username. */
    WshShell_Char_t Pass[WSH_SHELL_PASS_LEN];   /**< Input buffer for password. */
} WshShell_AuthContext_t;

/**
 * @brief Main shell structure containing state, configuration, user context, and subsystems.
 */
typedef struct {
    WshShell_Char_t* Version;                           /**< Version string. */
    WshShell_Char_t DeviceName[WSH_SHELL_DEV_NAME_LEN]; /**< Device name (used in prompt). */
    WshShell_Char_t Prompt[WSH_SHELL_PROMPT_MAX_LEN];   /**< Cached prompt string. */
    WshShellIO_CommandLine_t CommandLine;               /**< Terminal input/output interface. */
    WshShell_Interact_t Interact;                       /**< Interactive command interface. */
    const WshShellUser_t* CurrUser;                     /**< Currently authenticated user. */
    WshShell_AuthContext_t TmpAuth;                     /**< Temporary auth input storage. */
    WshShell_ExtCallbacks_t ExtCallbacks;               /**< Optional external auth callbacks. */
    WshShellUser_Table_t Users;                         /**< Table of available users. */
    WshShellCmd_Table_t Commands;                       /**< Registered command table. */
    WshShellHistory_IO_t HistoryIO;                     /**< Command history buffer and ops. */
    WshShellEsc_Storage_t EscStorage;                   /**< Escape sequence state storage. */
} WshShell_t;

/**
 * @brief Initialize a shell instance.
 *
 * Initializes internal subsystems, assigns device name and optional header,
 * and installs optional external callbacks.
 *
 * @param pShell Pointer to the shell instance.
 * @param pcDevName Device name (e.g., "ttyS0" or "shell0").
 * @param pcCustomHeader Optional header string (can be NULL).
 * @param pExtClbks Pointer to external callback structure (can be NULL).
 * @return Initialization status code.
 */
WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShell, const WshShell_Char_t* pcDevName,
                                    const WshShell_Char_t* pcCustomHeader,
                                    WshShell_ExtCallbacks_t* pExtClbks);

/**
 * @brief Attempt to authenticate a user with given login credentials.
 *
 * Updates the current user context on success, or leaves it unchanged on failure.
 *
 * @param pShell Shell instance.
 * @param pcLogin User name.
 * @param pcPass Password.
 * @return Is auth OK?
 */
WshShell_Bool_t WshShell_Auth(WshShell_t* pShell, const WshShell_Char_t* pcLogin,
                              const WshShell_Char_t* pcPass);

/**
 * @brief Check if a user is currently authenticated.
 *
 * @param pShell Shell instance.
 * @return `WSH_SHELL_TRUE` if a user is authenticated, `WSH_SHELL_FALSE` otherwise.
 */
WshShell_Bool_t WshShell_IsAuth(WshShell_t* pShell);

/**
 * @brief De-authenticate the currently logged-in user.
 *
 * Resets the user context and optionally triggers the DeAuth callback.
 *
 * @param pShell Shell instance.
 */
void WshShell_DeAuth(WshShell_t* pShell);

/**
 * @brief Process a new character entered by the user.
 *
 * Handles interactive editing, history navigation, or command execution if input is complete.
 *
 * @param pShell Shell instance.
 * @param symbol Character to insert.
 */
void WshShell_InsertChar(WshShell_t* pShell, const WshShell_Char_t symbol);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_H */
