/**
 * @file wsh_shell_interact.h
 * @brief Interactive command registration and line editing utilities for WshShell.
 *
 * This module provides an interface for attaching interactive commands
 * and for modifying the user input buffer, such as appending carriage return
 * and line feed sequences (`\r\n`) at the end of the command line.
 *
 * It is used internally by the shell to register interactive commands
 * and manage the formatting of user-entered command lines.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_INTERACT_H
#define __WSH_SHELL_INTERACT_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function pointer type for an interactive shell command.
 *
 * This type defines the prototype for interactive command callbacks,
 * which are invoked with a pointer to the current command line buffer.
 */
typedef void (*InteractiveCmd_t)(WshShellIO_CommandLine_t* pInter);

/**
 * @brief Interactive command registration structure.
 *
 * Stores the name of the interactive command and the corresponding execution function.
 */
typedef struct {
    InteractiveCmd_t Exec; /**< Function to be executed in interactive mode. */
    WshShell_Char_t CmdName[WSH_SHELL_CMD_NAME_LEN]; /**< Name of the interactive command. */
} WshShell_Interact_t;

/**
 * @brief Registers an interactive command.
 *
 * Attaches a named command and its execution function to the given interact structure.
 * The command name is truncated if it exceeds the internal buffer size.
 *
 * @param[out] pInteract Pointer to the interact object to initialize.
 * @param[in]  pcName    Null-terminated name of the interactive command.
 * @param[in]  exec      Function pointer to be called when the command is executed.
 */
void WshShellInteract_Attach(WshShell_Interact_t* pInteract, const WshShell_Char_t* pcName,
                             InteractiveCmd_t exec);

/**
 * @brief Appends a CRLF (`\r\n`) sequence to the interaction buffer.
 *
 * Adds a carriage return and line feed to the end of the current input buffer,
 * if there is enough space. This is typically used to terminate user input lines.
 *
 * If the buffer is too full to append both characters, an error is printed.
 *
 * @param[in,out] pInter Pointer to the interaction buffer (command line).
 */
void WshShellInteract_AppendLineBreak(WshShellIO_CommandLine_t* pInter);

/**
 * @brief Clears the contents of an interactive command slot.
 *
 * Resets the execution function pointer to `NULL` and clears the command name buffer.
 * This effectively "removes" the interactive command from the shell registry.
 *
 * @param[in,out] pInteract Pointer to the interact structure to flush.
 */
void WshShellInteract_Flush(WshShell_Interact_t* pInteract);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_INTERACT_H */
