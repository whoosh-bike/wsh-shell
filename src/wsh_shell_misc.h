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

/**
 * @brief Print a hex + ASCII dump of a buffer (hexdump -C style).
 *
 * Each row contains the byte offset, up to 16 bytes in hex (split into
 * two groups of 8), and their printable ASCII representation.
 * Non-printable bytes are shown as '.'.
 *
 * Example output:
 * @code
 * 00000000  57 73 68 53 68 65 6c 6c  00 01 7e 7f 80 ff       |WshShell..~...|
 * @endcode
 *
 * @param pBuff  Pointer to the data buffer. May be NULL only when len == 0.
 * @param len    Number of bytes to dump.
 * @param offset Base value printed in the offset column (use 0 for relative offsets).
 */
void WshShellMisc_HexDump(const WshShell_U8_t* pBuff, WshShell_Size_t len, WshShell_Size_t offset);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_MISC_H */
