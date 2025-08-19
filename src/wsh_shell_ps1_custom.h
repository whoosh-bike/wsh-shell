/**
 * @file wsh_shell_ps1_custom.h
 * @brief Shell prompt (PS1) customization and formatting.
 *
 * Provides functionality for building and customizing the shell prompt (`PS1`).
 * This includes dynamic substitution of runtime values such as user name, device
 * name, and ANSI styling codes (colors, bold, reset).
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_PS1_CUSTOM_H
#define __WSH_SHELL_PS1_CUSTOM_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Runtime data used for PS1 (prompt string) expansion.
 *
 * This structure provides the dynamic values that can be substituted into
 * the PS1 template string (`WSH_SHELL_PS1_TEMPLATE`).
 *
 * Members:
 * - `UserName`      — Current user name string (used by `%u`).
 * - `DevName`       — Device name string (used by `%d`).
 * - `InterCmdName`  — Optional intermediate command or context-specific string
 *                     that can be injected into the prompt. Can be NULL if not used.
 */
typedef struct {
    const WshShell_Char_t* UserName; /**< User name string, used in `%u`. */
    const WshShell_Char_t* DevName;  /**< Device name string, used in `%d`. */
    WshShell_Char_t* InterCmdName;   /**< Intermediate/temporary command name or context string. */
} WshShell_PS1Data_t;

/**
 * @brief Applies the PS1 template and generates the final PS1 string.
 *
 * Expands a predefined PS1 template (`WSH_SHELL_PS1_TEMPLATE`) into a complete PS1
 * string, substituting special format specifiers with runtime values like username, device name,
 * and ANSI escape sequences for styling (colors, bold, reset).
 *
 * Supported format specifiers in the template:
 * - `%u` — current username (from @p pUser)
 * - `%d` — current device name (from @p pDevice)
 * - `%cN` — ANSI color code from predefined color map (0 ≤ N ≤ 9)
 * - `%b` — ANSI escape sequence for bold text
 * - `%r` — ANSI escape sequence to reset all styles
 * - Any unknown `%` sequence is copied verbatim as `%X`.
 *
 * The output is truncated if it would exceed `WSH_SHELL_PS1_MAX_LEN - 1`.
 * The result is always null-terminated.
 *
 * @param[out] pPS1 Output buffer for the final PS1 string.
 *                  Must be at least `WSH_SHELL_PS1_MAX_LEN` in size.
 * @param[in]  pPS1Data   Pointer to the user name, divice name or other strings.
 */
void WshShell_GeneratePS1(WshShell_Char_t* pPS1, WshShell_PS1Data_t* pPS1Data);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_PS1_CUSTOM_H */
