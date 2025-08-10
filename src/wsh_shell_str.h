/**
 * @file wsh_shell_str.h
 * @brief Common string operations for user input processing
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_STR_H
#define __WSH_SHELL_STR_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_option.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const WshShell_Char_t* UserName;
    const WshShell_Char_t* DevName;
    WshShell_Char_t* InterCmdName;
} WshShellStr_PromptData_t;

/**
 * @defgroup ShellStr String operations
 * @brief Common string operations for user input processing.
 * @{
 */

/**
 * @brief Check whether the given character is a printable ASCII symbol.
 *
 * This function checks if the character belongs to the range of printable ASCII characters
 * (from 0x20 ' ' to 0x7E '~').
 *
 * @param[in] ch Character to check.
 *
 * @return true if the character is printable ASCII; false otherwise.
 */
WshShell_Bool_t WshShellStr_IsPrintableAscii(WshShell_Char_t ch);

/**
 * @brief Trim tabulation and spaces from beginning and end of a string.
 *
 * @param[in] pString: Pointer to a string to be trimmed.
 * @param[in] len: Length of an input string.
 *
 * @return WshShell_Char_t*: Pointer to a trimmed string.
 */
WshShell_Char_t* WshShellStr_TrimString(WshShell_Char_t* pString, WshShell_Size_t len);

/**
 * @brief Parse a string into space-separated tokens, handling quoted substrings as single tokens.
 *
 * This function splits the input string `pStr` into tokens by replacing spaces with null terminators (`\0`).
 * Tokens are written as pointers to the beginning of each token into the `pArgBuff` array.
 * Quoted substrings (enclosed in double quotes `"`), if any, are treated as a single token.
 * Quotes themselves are removed during parsing.
 *ч
 * @note This function modifies the input string in-place. Spaces and quotes are replaced with `\0`.
 * Escaped or nested quotes are not supported.
 *
 * Example:
 * Input:  `"arg1 arg2 \"quoted string\" arg3"`
 * Output: `pArgBuff = { "arg1", "arg2", "quoted string", "arg3" }`, `*pArgNum = 4`
 *
 * @param[in, out] pStr Pointer to the null-terminated string to be parsed. Will be modified in-place.
 * @param[out] pArgNum Pointer to a counter that will be set to the number of parsed tokens.
 * @param[out] pArgBuff Array of string pointers (tokens). Must be large enough to hold all tokens.
 * @param[in] maxArgNum Maximum number of tokens that can be stored in `pArgBuff`. Excess tokens are discarded.
 */
void WshShellStr_ParseToArgcArgv(WshShell_Char_t* pStr, WshShell_Size_t* pArgNum,
                                 const WshShell_Char_t* pArgBuff[], WshShell_Size_t maxArgNum);

/**
 * @brief Increase buffer counter.
 *
 * @param[out] pInterCnt: Pointer to a counter.
 * @param[in] buffSize: Size of a buffer.
 */
void WshShellStr_IncrInterCnt(WshShell_Size_t* pInterCnt, WshShell_Size_t buffSize);

/**
 * @brief Decrease buffer counter.
 *
 * @param[out] pInterCnt: Pointer to a counter.
 */
void WshShellStr_DecrInterCnt(WshShell_Size_t* pInterCnt);

/**
 * @brief Converts access permission bits to a human-readable string like "rwx".
 *
 * This function translates access bit flags into a POSIX-style permission string.
 * The result is written into the provided buffer.
 *
 * The output format is:
 * - `'r'` if `WSH_SHELL_OPT_ACCESS_READ` is set, otherwise `'-'`
 * - `'w'` if `WSH_SHELL_OPT_ACCESS_WRITE` is set, otherwise `'-'`
 * - `'x'` if `WSH_SHELL_OPT_ACCESS_EXECUTE` is set, otherwise `'-'`
 * - `'A'` if `access == WSH_SHELL_OPT_ACCESS_ANY` (optional fourth character)
 * 
 * Examples:
 * - `0x00`      → `"---"`
 * - `0x03`      → `"rw-"`
 * - `0x07`      → `"rwx"`
 * - `0xFFFFFFFF`→ `"rwxA"`
 *
 * @param[in]  access  Bitmask of access flags (e.g. WSH_SHELL_OPT_ACCESS_READ).
 * @param[out] pOutStr  Buffer to write result to. Must be at least 5 bytes long.
 */
void WshShellStr_AccessBitsToStr(WshShell_Size_t access, WshShell_Char_t* pOutStr);

/**
 * @brief Converts group bitmask into symbolic group string (e.g. "*--*", "---*", etc).
 *
 * Produces a fixed-length string of '*' and '-' characters from highest to lowest group index.
 * 
 * Example for 4 groups:
 * - WSH_SHELL_CMD_GROUP_ADMIN  = bit 0 → rightmost char
 * - WSH_SHELL_CMD_GROUP_MANUF  = bit 3 → leftmost char
 *
 * @param[in]  group     Bitmask of groups.
 * @param[out] pOutStr   Output buffer (must be at least WSH_SHELL_GROUP_STR_LEN).
 */
void WshShellStr_GroupBitsToStr(WshShell_Size_t group, WshShell_Char_t* pOutStr);

/**
 * @brief Applies the prompt template and generates the final prompt string.
 *
 * Expands a predefined prompt template (`WSH_SHELL_PROMPT_TEMPLATE`) into a complete prompt
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
 * The output is truncated if it would exceed `WSH_SHELL_PROMPT_MAX_LEN - 1`.
 * The result is always null-terminated.
 *
 * @param[out] pcPrompt Output buffer for the final prompt string.
 *                     Must be at least `WSH_SHELL_PROMPT_MAX_LEN` in size.
 * @param[in]  pcUserName   Pointer to the user name string.
 * @param[in]  pcDevName Pointer to the device name string.
 */
void WshShellStr_GeneratePrompt(WshShell_Char_t* pPrompt, WshShellStr_PromptData_t* pPromptData);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_STR_H */
