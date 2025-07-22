/**
 * @file wsh_shell_history.h
 * @brief File that contains history operations.
 * @author Whoosh Embedded Team
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_HISTORY_H
#define __WSH_SHELL_HISTORY_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_types.h"

/**
 * @defgroup WshShellHistory History
 * @brief Functionality for context save.
 * @{
 */

/**
 * @ingroup WshShellHistory
 * @struct WshShellHistory_Data_t
 * @brief History buffer
 * 
 * Actual history data buffer that contains string tokens of the called commands.
 * Each token represents one command that have been called previously.
 */
typedef struct {
    char pBuffer[WSH_SHELL_HISTORY_BUFF_SIZE]; /**< Array for storing command strings. */
    WshShell_Size_t HeadIdx;                   /**< Pointer to a buffer begining. */
    WshShell_Size_t TailIdx;                   /**< Pointer to a buffer end. */
    WshShell_Size_t LastSavedCmdIdx;           /**< Pointer to a last saved command. */
} WshShellHistory_Data_t;

/**
 * @ingroup WshShellHistory
 * @struct WshShellHistory_DataStorage_t
 * @brief History buffer
 * 
 * Structure for storing history data and calculated CRC for data validation.
 */
typedef struct {
    WshShell_U32_t CRC;          /**< CRC32 sum for checking of history buffer validness. */
    WshShellHistory_Data_t Data; /**< Data structure. */
} WshShellHistory_t;

/**
 * @defgroup PublicHistoryFunc Public functions
 * @brief Public functions for interaction with WshShellHistory_t object.
 * @{
 */

__attribute__((weak)) void WshShellHistory_Write(WshShellHistory_t history);
__attribute__((weak)) WshShellHistory_t WshShellHistory_Read(void);

/**
 * @brief Create histroy tracking object.
 */
void WshShellHistory_Init(void);

/**
 * @brief Function for saving command string.
 * 
 * @param[in] pCmdStr: Pointer to a command string.
 * @param[in] cmdStrLen: Length of the command string without EOL symbol.
 */
void WshShellHistory_SaveCmd(const char* pCmdStr, WshShell_Size_t cmdStrLen);

/**
 * @brief Get command previous to current command string.
 * 
 * @param[out] pOutBuff: Pointer to a buffer for command string writing.
 * @param[in] buffSize: Size of output buffer.
 * 
 * @return WshShell_Size_t: Length of the command line.
 */
WshShell_Size_t WshShellHistory_GetPrevCmd(char* pOutBuff, WshShell_Size_t buffSize);

/**
 * @brief Get command next to current command string.
 * 
 * @param[out] pOutBuff: Pointer to a buffer for command string writing.
 * @param[in] buffSize: Size of output buffer.
 * 
 * @return WshShell_Size_t: Length of the command line.
 */
WshShell_Size_t WshShellHistory_GetNextCmd(char* pOutBuff, WshShell_Size_t buffSize);

/**
 * @brief Erase all saved command strings.
 */
void WshShellHistory_Flush(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* __WSH_SHELL_HISTORY_H */
