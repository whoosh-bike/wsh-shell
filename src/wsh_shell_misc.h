/**
 * @file wsh_shell_misc.h
 * @brief Miscellaneous helper utilities for the shell.
 *
 * This module provides various helper functions that do not belong
 * to specific subsystems, but are commonly used across the shell.
 * Currently includes hashing utilities and may be extended in the future.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_MISC_H
#define __WSH_SHELL_MISC_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

WshShell_U32_t WshShellMisc_CalcJenkinsHash(const WshShell_U8_t* pcBuff, WshShell_Size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_MISC_H */
