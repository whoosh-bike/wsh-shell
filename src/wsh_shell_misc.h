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

/**
 * @brief Calculate Jenkins one-at-a-time hash.
 *
 * This function computes the 32-bit Jenkins one-at-a-time hash
 * for the given buffer. It is a simple and fast non-cryptographic (!)
 * hash function, commonly used for hash tables and quick data
 * checksums.
 *
 * @param pcBuff Pointer to the input buffer.
 * @param len    Number of bytes in the input buffer.
 *
 * @return 32-bit hash value computed over the input buffer.
 *
 * @note This hash is not suitable for cryptographic purposes !!!
 *       It is designed for speed and good distribution in hash tables.
 */
WshShell_U32_t WshShellMisc_CalcJenkinsHash(const WshShell_U8_t* pcBuff, WshShell_Size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_MISC_H */
