/**
 * @file wsh_shell_autocomplete.h
 * @brief Command-line autocompletion for shell commands.
 * 
 * This module provides autocompletion support for the shell interface. It enables 
 * the user to complete partially typed commands based on the currently registered 
 * command set. The algorithm performs prefix matching and modifies the input buffer 
 * accordingly:
 * 
 * - If **no match** is found, the input remains unchanged.
 * - If **exactly one match** is found, the input is completed with that command.
 * - If **multiple matches** exist with a **common prefix**, the input is extended 
 *   to the longest common prefix.
 * 
 * Autocompletion improves user experience and reduces typing effort in shell environments.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_AUTOCOMPLETE_H
#define __WSH_SHELL_AUTOCOMPLETE_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_cmd.h"
#include "wsh_shell_cmd_def.h"
#include "wsh_shell_io.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup WshShellAutocomplete Autocompletion
 * @brief Command-line autocompletion support for shell input.
 * @{
 */

/**
 * @brief Attempts to autocomplete the command in the input buffer.
 * 
 * Scans the registered command table and tries to match the current input as a prefix.
 * If possible, it autocompletes the input buffer:
 * 
 * - If no matches are found, the buffer is not changed.
 * - If one match is found, the buffer is completed with the full command name.
 * - If multiple matches are found with a shared prefix, the buffer is extended to the longest matchable prefix.
 * 
 * This function is typically triggered by the user pressing the Tab key.
 *
 * @param[in,out] pInBuff        Pointer to the input buffer to modify.
 * @param[in]     inBuffLen      Total size of the input buffer.
 * @param[in]     pShellCommands Pointer to the command table to search.
 * 
 * @retval true   The buffer was modified (i.e., autocomplete was applied).
 * @retval false  No autocomplete was possible (zero matches or ambiguous match).
 */
WshShell_Bool_t WshShellAutocomplete_Try(WshShell_Char_t* pInBuff, WshShell_Size_t inBuffLen,
                                         WshShellCmd_Table_t* pShellCommands);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_AUTOCOMPLETE_H */
