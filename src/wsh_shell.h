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
#include "wsh_shell_misc.h"
#include "wsh_shell_promptwait.h"
#include "wsh_shell_ps1_custom.h"
#include "wsh_shell_session.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"
#include "wsh_shell_user.h"
#include "wsh_shell_version.h"

/* detect operating system name */
#if defined(__linux)
#define OS_NAME "Linux"
#elif defined(__unix)
#define OS_NAME "Unix"
#elif defined(__APPLE__)
#define OS_NAME "Darwin"
#elif defined(_WIN32)
#define OS_NAME "Windows"
#elif defined(tskKERNEL_VERSION_NUMBER)
#define OS_NAME "FreeRTOS " tskKERNEL_VERSION_NUMBER
#else
#define OS_NAME WSH_SHELL_TARGET_OS
#endif

/* detect compiler name and version */
#if defined(__clang__)
#define COMPILER "clang " __clang_version__
#elif defined(__GNUC__)
#define COMPILER "GCC " __VERSION__
#elif defined(_MSC_VER)
#define COMPILER "MSVC"
#elif defined(__CC_ARM)
#define COMPILER "ARMCC"
#elif defined(__ICCARM__)
#define COMPILER "IAR"
#else
#define COMPILER "Unknown Compiler"
#endif

/* default welcome banner, printed by WshShell_Init() when no custom one is given;
 * define WSH_SHELL_HEADER in wsh_shell_cfg.h to replace it (or to "" to drop it) */
#ifndef WSH_SHELL_HEADER
/* clang-format off */
#define WSH_SHELL_HEADER "\
                __               __         ____  \r\n\
 _      _______/ /_        _____/ /_  ___  / / /  \r\n\
| | /| / / ___/ __ \\______/ ___/ __ \\/ _ \\/ / /\r\n\
| |/ |/ (__  ) / / /_____(__  ) / / /  __/ / /    \r\n\
|__/|__/____/_/ /_/     /____/_/ /_/\\___/_/_/    \r\n\
\r\n"
/* clang-format on */
#endif

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
} WshShellExtCallbacks_t;

/**
 * @brief Temporary authentication context for login input handling.
 */
typedef struct {
    WshShell_Char_t Login[WSH_SHELL_LOGIN_LEN]; /**< Input buffer for username. */
    WshShell_Char_t Pass[WSH_SHELL_PASS_LEN];   /**< Input buffer for password. */
} WshShellAuthCtx_t;

/**
 * @brief Main shell structure containing state, configuration, user context, and subsystems.
 */
typedef struct {
    WshShell_Char_t* Version;                           /**< Version string. */
    WshShell_Char_t DeviceName[WSH_SHELL_DEV_NAME_LEN]; /**< Device name (used in PS1 and more). */
    WshShell_Char_t PS1[WSH_SHELL_PS1_MAX_LEN];         /**< Cached PS1 string. */
    WshShell_Char_t PrevSym;                            /**< Previous symbol inserted in. */

    WshShellIO_CommandLine_t CommandLine; /**< Terminal input/output interface. */
    const WshShellUser_t* CurrUser;       /**< Currently authenticated user. */
    WshShellAuthCtx_t TmpAuth;            /**< Temporary auth input storage. */
    WshShellEsc_Storage_t EscStorage;     /**< Escape sequence state storage. */

    WshShellUser_Table_t Users;   /**< Table of available users. */
    WshShellCmd_Table_t Commands; /**< Registered command table. */

    WshShellHistoryIO_t HistoryIO; /**< Command history buffer and ops. */
    WshShellSessionIO_t SessionIO; /**< Persistent login session storage ops. */
    WshShellInteract_t Interact;   /**< Interactive command interface. */
    WshShellPromptWait_t PromptWait;

    WshShellExtCallbacks_t ExtCallbacks; /**< Optional external auth callbacks. */
} WshShell_t;

/**
 * @brief Initialize a shell instance.
 *
 * Initializes internal subsystems, assigns device name and optional header,
 * and installs optional external callbacks.
 *
 * @param pShell Pointer to the shell instance.
 * @param pcDevName Device name (e.g., "ttyS0" or "shell0").
 * @param pcCustomHeader Optional welcome banner for this instance. When NULL,
 *                       WSH_SHELL_HEADER is used: either the one defined in
 *                       wsh_shell_cfg.h or the built-in wsh-shell logo.
 * @param pExtClbks Pointer to external callback structure (can be NULL).
 * @return Initialization status code.
 */
WSH_SHELL_RET_STATE_t WshShell_Init(WshShell_t* pShell, const WshShell_Char_t* pcDevName,
                                    const WshShell_Char_t* pcCustomHeader, WshShellExtCallbacks_t* pExtClbks);

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
WshShell_Bool_t WshShell_Auth(WshShell_t* pShell, const WshShell_Char_t* pcLogin, const WshShell_Char_t* pcPass);

/**
 * @brief Check if a user is currently authenticated.
 *
 * @param pShell Shell instance.
 * @return `WSH_SHELL_TRUE` if a user is authenticated, `WSH_SHELL_FALSE` otherwise.
 */
WshShell_Bool_t WshShell_IsAuth(const WshShell_t* pcShell);

/**
 * @brief De-authenticate the currently logged-in user.
 *
 * Resets the user context and optionally triggers the DeAuth callback.
 *
 * @param pShell Shell instance.
 * @param pcReason Reason or source of deauth.
 */
void WshShell_DeAuth(WshShell_t* pShell, const WshShell_Char_t* pcReason);

/**
 * @brief Arm cross-reboot login persistence for the current user.
 *
 * Records the current login so it can be restored without a password after up to
 * @p reboots reboots. While a session is armed the integrator can also suppress
 * the inactivity auto-logout (see WshShell_SessionIsKeepActive()). Passing 0
 * clears any armed session. Requires the shell to have session I/O installed
 * (WshShellSession_Init()) and a user currently logged in.
 *
 * @param pShell   Shell instance.
 * @param reboots  Number of reboots the login may survive (0 clears).
 * @return `true` if the request was applied.
 */
WshShell_Bool_t WshShell_SessionArm(WshShell_t* pShell, WshShell_U32_t reboots);

/**
 * @brief Restore a previously armed login without a password prompt.
 *
 * Intended to be called once at start-up, after the user table is attached.
 * On success the current user and PS1 are set and the Auth callback is invoked,
 * exactly as a normal login would. Consumes one reboot from the budget.
 *
 * @param pShell Shell instance.
 * @return `true` if a valid session was restored.
 */
WshShell_Bool_t WshShell_SessionRestore(WshShell_t* pShell);

/**
 * @brief Whether an armed keep-session is currently active (budget remaining).
 *
 * The integrator can use this to block the inactivity auto-logout while the host
 * asked to stay logged in.
 *
 * @param pShell Shell instance.
 * @return `true` if a valid session with remaining reboot budget is stored.
 */
WshShell_Bool_t WshShell_SessionIsKeepActive(WshShell_t* pShell);

/**
 * @brief Remaining reboot budget of the armed session, or 0 if none.
 *
 * @param pShell Shell instance.
 * @return Reboots left, or 0 when no valid session is armed.
 */
WshShell_U32_t WshShell_SessionRebootsLeft(WshShell_t* pShell);

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
