/**
 * @file wsh_shell_cmd.h
 * @brief Command object definition and API's
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_CMD_H
#define __WSH_SHELL_CMD_H

#include "wsh_shell_option.h"
#include "wsh_shell_types.h"

/**
 * @defgroup WshShellCmd Commands
 * @brief Functionality for shell commands.
 * @{
 */

/**
 * @ingroup WshShellCmd
 * @struct WshShellCmd_t wsh_shell_cmd.h "wsh_shell_cmd.h"
 * @brief A structure to represent shell command object.
 * 
 * Command is a collection of flags associated with unique string token called *Name*.
 * Each unique name input calls execution of corresponding *Callback function*.
 * Callback functions are users piece of code and are called by reference.
 */
typedef struct WshShellCmd {
    const char* pName;                /**< Command name. */
    const char* pHelp;                /**< Command description. */
    WshShell_Size_t Group;            /**< Command group */
    const WshShellOption_t* pOptions; /**< Array of options. */
    WshShell_Size_t OptNum;           /**< Amount of options in array */
    WSH_SHELL_RET_STATE_t (*Exec)(const struct WshShellCmd* pCmd, WshShell_Size_t argc, const char* pArgv[]);
} WshShellCmd_t;

/**
 * @ingroup WshShellCmd
 * @struct WshShellCmd_OptDescr_t wsh_shell_cmd.h "wsh_shell_cmd.h"
 * @brief A structure of founded option after command string parsing.
 */
typedef struct {
    const WshShellOption_t* pOpt; /**< Const pointer to a founded option structure */
    WshShell_Size_t TokenPos;     /**< Position of a string token */
} WshShellCmd_OptDescr_t;

/**
 * @brief Command executable format
 */
typedef WSH_SHELL_RET_STATE_t (*WshShellCmd_Exec_t)(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                    const char* pArgv[]);

/**
 * @defgroup PublicCmdFunc Public functions
 * @brief Public functions for interaction with WshShellCmd_t object.
 * @{
 */

/**
 * @brief Initialize command table for further shell usage.
 * 
 * @param[in] pCmdTable: Pointer to a table with defined commands.
 * @param[in] cmdNum: Number of commands in table.
 * 
 * @retval WSH_SHELL_RET_STATE_ERR_PARAM: Something wrong with input arguments.
 * @retval WSH_SHELL_RET_STATE_SUCCESS: Success.
 */
WSH_SHELL_RET_STATE_t WshShellCmd_Init(const WshShellCmd_t* pCmdTable, WshShell_Size_t cmdNum);

/**
 * @brief Print help and available options for specific command.
 * 
 * @param[in] pCmd: Pointer to a command.
 */
void WshShellCmd_PrintInfo(const WshShellCmd_t* pCmd);

/**
 * @brief Get amount of available commands.
 * @return WshShell_Size_t: Commands number.
 */
WshShell_Size_t WshShellCmd_GetCmdNum(void);

/**
 * @brief Get command pointer by it's index.
 * 
 * @param[in] id: Commands index.
 * 
 * @retval const WshShellCmd_t*: Pointer to a command.
 * @retval NULL: No command with such index.
 */
const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShell_Size_t id);

/**
 * @brief Find an option in string tokens.
 * 
 * @param[in] pCmd: Pointer to a command which options are we checking.
 * @param[in] argc: Number of string tokens.
 * @param[in] pArgv: Array of string tokens.
 * @param[in] pTokenPos: Pointer to a current token position.
 * 
 * @return WshShellCmd_OptDescr_t: Structure with founded option and it's string token index.
 */
WshShellCmd_OptDescr_t WshShellCmd_ParseOpt(const WshShellCmd_t* pCmd, WshShell_Size_t argc, const char* pArgv[],
                                            WshShell_Size_t* pTokenPos);

/**
 * @brief Get value from a founded option.
 * 
 * @param[in] pOptDescr: Pointer to a founded option description.
 * @param[in] argc: Number of string tokens.
 * @param[in] pArgv: Array of string tokens.
 * @param[in] valueSize: Size of a value to copy.
 * @param[out] pValue: Pointer to a value container.
 * 
 * @retval WSH_SHELL_RET_STATE_ERR_PARAM: Something wrong with input arguments.
 * @retval WSH_SHELL_RET_STATE_ERR_EMPTY: No tokens to read value.
 * @retval WSH_SHELL_RET_STATE_ERR_OVERFLOW: Too few tokens to read value.
 * @retval WSH_SHELL_RET_STATE_SUCCESS: Success.
 */
WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellCmd_OptDescr_t* pOptDescr, WshShell_Size_t argc,
                                              const char* pArgv[], WshShell_Size_t valueSize, WshShell_Size_t* pValue);

/**
 * @brief Forget current commands table.
 */
void WshShellCmd_Destroy(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* __WSH_SHELL_CMD_H */
