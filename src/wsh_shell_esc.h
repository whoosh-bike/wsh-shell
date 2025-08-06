/**
 * @file wsh_shell_esc.h
 * @brief Escape sequence handling logic for WSH shell
 *
 * Provides functionality to process ANSI escape sequences (e.g., arrow keys)
 * and interact with shell history and input buffer.
 *
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_ESC_H
#define __WSH_SHELL_ESC_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_history.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Escape sequence handler function type
 */
typedef void (*WshShellEsc_Hadler_t)(WshShellHistory_IO_t* pHistIO,
                                     WshShellIO_CommandLine_t* pCommandLine);

/**
 * @brief Escape sequence to handler mapping
 */
typedef struct {
    const WshShell_Char_t* Sequence; /**< ANSI escape sequence */
    WshShellEsc_Hadler_t Handler;    /**< Associated handler function */
} WshShellEsc_Action_t;

/**
 * @brief Escape sequence input state tracker
 */
typedef struct {
    WshShell_Size_t Cnt;                          /**< Current number of buffered characters */
    WshShell_Char_t Buff[WSH_SHELL_ESC_BUFF_LEN]; /**< Escape sequence buffer */
} WshShellEsc_Storage_t;

/**
 * @brief Handles incoming escape sequence character
 *
 * Matches current buffer against known ANSI sequences and triggers appropriate handler.
 *
 * @param pHistIO Pointer to shell history interface
 * @param pCommandLine Pointer to input interaction buffer
 * @param pEscStorage Pointer to escape sequence buffer
 * @param symbol Incoming character to append
 */
void WshShellEsc_Handler(WshShellHistory_IO_t* pHistIO, WshShellIO_CommandLine_t* pCommandLine,
                         WshShellEsc_Storage_t* pEscStorage, const WshShell_Char_t symbol);

/**
 * @brief Starts a new escape sequence
 *
 * Initializes the escape buffer with ESC char.
 *
 * @param pEscStorage Pointer to escape sequence buffer
 */
void WshShellEsc_StartSeq(WshShellEsc_Storage_t* pEscStorage);

/**
 * @brief Checks if escape sequence is in progress
 *
 * @param pEscStorage Pointer to escape sequence buffer
 * @return WshShell_Bool_t `true` if sequence has started
 */
WshShell_Bool_t WshShellEsc_IsSeqStarted(WshShellEsc_Storage_t* pEscStorage);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_ESC_H */
