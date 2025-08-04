/**
 * @file wsh_shell_str.h
 * @brief Common string operations for user input processing.
 * @author Whoosh Embedded Team
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_STR_H
#define __WSH_SHELL_STR_H

#include "wsh_shell_types.h"

/**
 * @defgroup ShellStr String operations
 * @brief Common string operations for user input processing.
 * @{
 */

/**
 * @brief Trim tabulation and spaces from beginning and end of a string.
 *
 * @param[in] pString: Pointer to a string to be trimmed.
 * @param[in] length: Length of an input string.
 *
 * @return char*: Pointer to a trimmed string.
 */
char* WshShellStr_TrimString(char* pString, WshShell_Size_t length);

/**
 * @brief Parse string to string tokens.
 *
 * @param[in] pStr: Pointer to a string to be parsed.
 * @param[out] pArgNum: Pointer to a value for storing amount of tokens.
 * @param[out] ppArgBuff: Pointer to an array of string pointers.
 */
void WshShellStr_ParseToArgcArgv(char* pStr, WshShell_Size_t* pArgNum, const char** ppArgBuff);

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
 * @} 
 */

#endif /* __WSH_SHELL_STR_H */
