/**
 * @file wsh_shell_history.h
 * @brief Shell history management API
 * 
 * This module provides functionality for storing, navigating, and persisting
 * command history in an embedded shell environment.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_HISTORY_H
#define __WSH_SHELL_HISTORY_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup WshShellHistory Shell Command History
 * @brief Functionality for storing and navigating command history.
 * @{
 */

/**
 * @brief Direction for history navigation.
 */
typedef enum {
    WSH_SHELL_HIST_CMD_PREV = 0, /**< Navigate to the previous command. */
    WSH_SHELL_HIST_CMD_NEXT      /**< Navigate to the next command. */
} WSH_SHELL_HIST_CMD_DIR_t;

/**
 * @brief Internal structure for storing command history buffer.
 * 
 * This structure contains the command history as a raw character buffer and related metadata
 * for navigation and storage management.
 */
typedef struct {
    WshShell_Char_t StorageBuff[WSH_SHELL_HISTORY_BUFF_SIZE]; /**< Circular buffer for commands. */
    WshShell_Size_t HeadIdx;                                  /**< Index of the buffer start. */
    WshShell_Size_t TailIdx;                                  /**< Index of the buffer end. */
    WshShell_Size_t LastSavedCmdIdx;  /**< Index of the last saved command. */
    WSH_SHELL_HIST_CMD_DIR_t PrevDir; /**< Last direction of history navigation. */
    WshShell_Bool_t LimitIsReached;   /**< Flag indicating buffer search boundary reached. */
} WshShellHistory_Data_t;

/**
 * @brief Shell history structure with integrity check.
 * 
 * This structure wraps the command history data along with a hash (CRC) for validation.
 */
typedef struct {
    WshShell_U32_t Hash;         /**< Hash for integrity verification. */
    WshShellHistory_Data_t Data; /**< Command history buffer. */
} WshShellHistory_t;

/**
 * @brief Function pointer type for reading shell history from persistent storage.
 */
typedef WshShellHistory_t (*WshShellHistory_ReadHandler_t)(void);

/**
 * @brief Function pointer type for writing shell history to persistent storage.
 */
typedef void (*WshShellHistory_WriteHandler_t)(WshShellHistory_t);

/**
 * @brief Structure holding function pointers for history persistence I/O.
 *
 * This allows the shell history system to remain storage-agnostic by delegating
 * read/write responsibilities to external code (e.g., flash drivers, NVM emulation).
 */
typedef struct {
    WshShellHistory_ReadHandler_t Read;   /**< Callback for loading saved history. */
    WshShellHistory_WriteHandler_t Write; /**< Callback for saving current history. */
} WshShellHistory_IO_t;

/**
 * @defgroup WshShellHistoryAPI Public API
 * @brief Public functions for managing shell history.
 * @{
 */

/**
 * @brief Initialize the shell history system with custom I/O functions.
 * 
 * This function sets up the internal history system and loads existing history
 * using the provided read/write callbacks.
 * 
 * @param[in,out] pHistIO Pointer to the I/O structure used for read/write access.
 * @param[in]     readFn  Callback to read saved history data.
 * @param[in]     writeFn Callback to persist current history data.
 */
void WshShellHistory_Init(WshShellHistory_IO_t* pHistIO, WshShellHistory_ReadHandler_t readFn,
                          WshShellHistory_WriteHandler_t writeFn);

/**
 * @brief Save a new command to the history buffer.
 * 
 * @param[in] pHistIO    Pointer to the I/O structure.
 * @param[in] pcCmdStr   Pointer to the command string (without EOL).
 * @param[in] cmdStrLen  Length of the command string.
 */
void WshShellHistory_SaveCmd(WshShellHistory_IO_t* pHistIO, const WshShell_Char_t* pcCmdStr,
                             WshShell_Size_t cmdStrLen);

/**
 * @brief Retrieve the previous command from history.
 * 
 * @param[in]  pHistIO     Pointer to the I/O structure.
 * @param[out] pOutBuff    Buffer to store the command string.
 * @param[in]  outBuffSize Size of the output buffer.
 * 
 * @return Length of the retrieved command.
 */
WshShell_Size_t WshShellHistory_GetPrevCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize);

/**
 * @brief Retrieve the next command from history.
 * 
 * @param[in]  pHistIO     Pointer to the I/O structure.
 * @param[out] pOutBuff    Buffer to store the command string.
 * @param[in]  outBuffSize Size of the output buffer.
 * 
 * @return Length of the retrieved command.
 */
WshShell_Size_t WshShellHistory_GetNextCmd(WshShellHistory_IO_t* pHistIO, WshShell_Char_t* pOutBuff,
                                           WshShell_Size_t outBuffSize);

/**
 * @brief Get the total number of commands stored in history.
 * 
 * Starts from the most recently saved command and iterates backwards 
 * through the history buffer to count all stored commands.
 * 
 * @param[in] pHistIO Pointer to the I/O structure.
 * 
 * @return Number of stored commands in history.
 */
WshShell_Size_t WshShellHistory_GetTokenNum(WshShellHistory_IO_t* pHistIO);

/**
 * @brief Retrieve a command from history by its index.
 * 
 * Index 0 corresponds to the most recently saved command,
 * index 1 to the previous one, and so on.
 * 
 * @param[in]  pHistIO     Pointer to the I/O structure.
 * @param[out] pOutBuff    Buffer to store the retrieved command.
 * @param[in]  outBuffSize Size of the output buffer.
 * @param[in]  index       Index of the command to retrieve (0 = latest).
 * 
 * @return Length of the retrieved command, or 0 if not found or buffer too small.
 */
WshShell_Size_t WshShellHistory_GetTokenByIndex(WshShellHistory_IO_t* pHistIO,
                                                WshShell_Char_t* pOutBuff,
                                                WshShell_Size_t outBuffSize, WshShell_Size_t index);

/**
 * @brief Clear the command history buffer.
 * 
 * @param[in] pHistIO Pointer to the I/O structure.
 */
void WshShellHistory_Flush(WshShellHistory_IO_t* pHistIO);

/**
 * @} // end of WshShellHistoryAPI
 */

/**
 * @} // end of WshShellHistory
 */

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_HISTORY_H */
