/**
 * @file wsh_shell_cmd_def.h
 * @brief Declaration of default shell commands and their option tables.
 * 
 * This header defines macros and functions for registering and executing 
 * a set of built-in shell commands. Each command is associated with a predefined 
 * option table and an executable handler.
 *
 * These default commands provide core shell functionality such as help, 
 * user info, clearing the screen, and managing command history.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_CMD_DEF_H
#define __WSH_SHELL_CMD_DEF_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_history.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get a pointer to the default shell command structure.
 *
 * This function returns a constant pointer to the static command structure
 * WshShellDefCmd, which describes the default command used for configuring
 * and querying the shell interface.
 *
 * @return const WshShellCmd_t* Pointer to the default command structure.
 */
const WshShellCmd_t* WshShellDefCmd_GetPtr(void);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_CMD_DEF_H */
