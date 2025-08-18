/**
 * @file wsh_shell_promptwait.h
 * @brief One-time user input waiting mechanism for WshShell.
 *
 * This module allows the shell to temporarily override normal input handling
 * to wait for a specific key press (e.g., <Enter>, Y/N confirmation, etc.).
 *
 * Once the input is received, the registered callback is executed,
 * and normal shell behavior resumes.
 *
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_PROMPTWAIT_H
#define __WSH_SHELL_PROMPTWAIT_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of command structure
struct WshShellPromptWait;

/**
 * @brief Function pointer type for prompt-wait callbacks.
 *
 * @param symbol The character that was entered by the user.
 * @param pCtx   Optional user context passed to the handler.
 */
typedef WshShell_Bool_t (*WshShellPromptWait_Handler_t)(WshShell_Char_t symbol,
                                                        struct WshShellPromptWait* pWait);

/**
 * @brief Prompt-wait control structure.
 *
 * Holds the active handler and optional context pointer.
 * If Handler is NULL, no prompt-wait is active.
 */
typedef struct WshShellPromptWait {
    WshShellPromptWait_Handler_t Handler;
    void* Ctx;
} WshShellPromptWait_t;

/**
 * @brief Flush the current prompt-wait mode.
 *
 * Resets the handler to NULL so that normal shell input is processed.
 *
 * @param[in,out] pWait Pointer to prompt-wait control object.
 */
void WshShellPromptWait_Flush(WshShellPromptWait_t* pWait);

/**
 * @brief Attach a one-time input handler.
 *
 * Replaces normal shell input handling with a temporary callback.
 * The callback will receive the first character entered by the user,
 * after which it should typically disable itself by calling
 * `WshShellPromptWait_Flush()`.
 *
 * @param[in,out] pWait   Pointer to prompt-wait control object.
 * @param[in]     handler Callback function to invoke on user input.
 * @param[in]     pCtx    Optional pointer to user context.
 */
void WshShellPromptWait_Attach(WshShellPromptWait_t* pWait, WshShellPromptWait_Handler_t handler,
                               void* pCtx);

/**
 * @brief Handle a symbol when in prompt-wait mode.
 *
 * If a handler is set, it is called with the given symbol and context.
 *
 * @param[in,out] pWait   Pointer to prompt-wait control object.
 * @param[in]     symbol  Character entered by the user.
 * @return  WSH_SHELL_RET_STATE_ERR_PARAM if some problems with input params
 *          WSH_SHELL_RET_STATE_ERR_EMPTY if there are no handler attached
 *          WSH_SHELL_RET_STATE_ERR_BUSY if normal shell processing should continue
 *          WSH_SHELL_RET_STATE_SUCCESS if the symbol was handled by the prompt-wait mechanism
 */
WSH_SHELL_RET_STATE_t WshShellPromptWait_Handle(WshShellPromptWait_t* pWait,
                                                WshShell_Char_t symbol);

/**
 * @brief User input handler for prompt-wait mode.
 *
 * Processes a character entered by the user while waiting for specific input
 * (e.g., Enter key, Y/N confirmation, etc.).
 *
 * @param symbol The character entered by the user.
 * @param pWait  Pointer to the prompt-wait control structure.
 * @return WshShell_Bool_t
 *         - true if the input was handled and waiting can end;
 *         - false if waiting should continue.
 */
WshShell_Bool_t WshShellPromptWait_Enter(WshShell_Char_t symbol, WshShellPromptWait_t* pWait);
WshShell_Bool_t WshShellPromptWait_YesNo(WshShell_Char_t symbol, WshShellPromptWait_t* pWait);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_PROMPTWAIT_H */
