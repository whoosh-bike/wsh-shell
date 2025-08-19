/**
 * @file wsh_shell_cmd.h
 * @brief Shell command object definition and public API.
 *
 * Provides data structures and APIs for defining and handling shell commands,
 * including command execution, option parsing, and help information printing.
 *
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_CMD_H
#define __WSH_SHELL_CMD_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_io.h"
#include "wsh_shell_option.h"
#include "wsh_shell_str.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of command structure
struct WshShellCmd;

/**
 * @brief Function pointer type for shell command execution.
 *
 * @param[in] pcCmd   Pointer to the command descriptor.
 * @param[in] argc   Number of command-line arguments.
 * @param[in] pArgv  Array of argument strings.
 *
 * @return Command execution result (success, error, etc.).
 */
typedef WSH_SHELL_RET_STATE_t (*WshShellCmdHandler_t)(const struct WshShellCmd* pcCmd,
                                                      WshShell_Size_t argc,
                                                      const WshShell_Char_t* pArgv[], void* pCtx);

/**
 * @brief Descriptor for a shell command.
 */
typedef struct WshShellCmd {
    const WshShell_Char_t* Name;  /**< Command name (e.g., "set", "info"). */
    const WshShell_Char_t* Descr; /**< Human-readable description of the command. */
    WshShell_Size_t Groups; /**< Command group bitmask for access control or categorization. */
    const WshShellOption_t* Options; /**< Pointer to the command's options array. */
    WshShell_Size_t OptNum;          /**< Number of defined options. */
    WshShellCmdHandler_t Handler;    /**< Execution callback function. */
} WshShellCmd_t;

/**
 * @brief Table of shell commands.
 */
typedef struct {
    const WshShellCmd_t** List; /**< Pointer to the array of pointers to commands. */
    WshShell_Size_t Num;        /**< Number of commands in the table. */
} WshShellCmd_Table_t;

/**
 * @brief Initializes the command table.
 *
 * @param[out] pShellCommands  Pointer to the shell command table.
 * @param[in]  pcCmdTable      Pointer to the static list of commands.
 * @param[in]  cmdNum          Number of commands in the list.
 *
 * @return WSH_SHELL_RET_STATE_SUCCESS on success.
 */
WSH_SHELL_RET_STATE_t WshShellCmd_Attach(WshShellCmd_Table_t* pShellCommands,
                                         const WshShellCmd_t* pcCmdTable[], WshShell_Size_t cmdNum);

/**
 * @brief Frees or resets the command table.
 *
 * @param[in,out] pShellCommands Pointer to the command table.
 */
void WshShellCmd_DeAttach(WshShellCmd_Table_t* pShellCommands);

/**
 * @brief Returns the number of registered commands.
 *
 * @param[in] pShellCommands Pointer to the command table.
 *
 * @return Number of commands.
 */
WshShell_Size_t WshShellCmd_GetCmdNum(WshShellCmd_Table_t* pShellCommands);

/**
 * @brief Retrieves a command by index.
 *
 * @param[in] pShellCommands Pointer to the command table.
 * @param[in] idx            Command index.
 *
 * @return Pointer to the command descriptor or NULL if out-of-bounds.
 */
const WshShellCmd_t* WshShellCmd_GetCmdByIndex(WshShellCmd_Table_t* pShellCommands,
                                               WshShell_Size_t idx);

/**
 * @brief Finds a command by its name.
 *
 * @param[in] pShellCommands Pointer to the command table.
 * @param[in] pcCmdName      Command name string.
 *
 * @return Pointer to the matching command descriptor or NULL if not found.
 */
const WshShellCmd_t* WshShellCmd_SearchCmd(WshShellCmd_Table_t* pShellCommands,
                                           const WshShell_Char_t* pcCmdName);

/**
 * @brief Parses a command-line option for a given shell command.
 *
 * This function analyzes the current token in the argument list and attempts
 * to identify a valid option defined in the given command's option table.
 * If a valid option is found, its descriptor is returned. The token position
 * (`*pTokenPos`) is advanced accordingly depending on the number of arguments
 * that the option consumes.
 *
 * If only the command name is present (i.e., `argc == 1`), the function checks
 * for an option of type `WSH_SHELL_OPTION_NO` (indicating the command may be
 * executed without any parameters).
 *
 * @param[in]  pcCmd       Pointer to the shell command definition.
 * @param[in]  argc       Number of arguments in the input array.
 * @param[in]  pArgv      Array of argument strings.
 * @param[in,out] pTokenPos Pointer to the current token position in `pArgv`.
 *                          Will be updated to point past the parsed option.
 *
 * @return A filled option descriptor if a matching option is found,
 *         or an empty descriptor if not.
 */
WshShellOption_Context_t WshShellCmd_ParseOpt(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                              const WshShell_Char_t* pArgv[],
                                              WshShell_Size_t* pTokenPos);

/**
 * @brief Retrieves the value associated with a parsed command option.
 *
 * This function extracts the value of a command-line option parsed by
 * `WshShellCmd_ParseOpt()`, based on its type. Supported types include:
 * - `WSH_SHELL_OPTION_STR`  → copied to `pValue` as a string
 * - `WSH_SHELL_OPTION_INT`  → parsed as integer and written to `pValue`
 * - `WSH_SHELL_OPTION_FLOAT`→ parsed as float and written to `pValue`
 *
 * The function expects the option to be followed by its value in `pArgv`.
 * All other option types return `WSH_SHELL_RET_STATE_ERR_EMPTY`.
 *
 * @param[in]  pOptCtx   Pointer to parsed option descriptor.
 * @param[in]  argc        Argument count.
 * @param[in]  pArgv       Argument vector (array of strings).
 * @param[in]  valueSize   Maximum size of the output buffer (used for strings).
 * @param[out] pValue      Output buffer for value (string/int/float based on type).
 *
 * @return WSH_SHELL_RET_STATE_SUCCESS     if value was successfully retrieved,
 * @return WSH_SHELL_RET_STATE_ERR_PARAM   if input parameters are invalid,
 * @return WSH_SHELL_RET_STATE_ERR_EMPTY   if the option doesn't accept a value,
 * @return WSH_SHELL_RET_STATE_ERR_OVERFLOW if argument list is too short.
 */
WSH_SHELL_RET_STATE_t WshShellCmd_GetOptValue(WshShellOption_Context_t* pOptCtx,
                                              WshShell_Size_t argc, const WshShell_Char_t* pArgv[],
                                              WshShell_Size_t valueSize, void* pValue);

/**
 * @brief Prints detailed information about a shell command and its options.
 *
 * This function outputs the help text and a formatted table describing
 * each available option for the specified command. For each option,
 * the following fields are printed:
 * - Short name
 * - Long name
 * - Option type (e.g., INT, STR, FLOAT)
 * - Access flags (read/write/etc.)
 * - Description/help string
 *
 * Options of type `WSH_SHELL_OPTION_NO` and `WSH_SHELL_OPTION_WAITS_INPUT` are skipped.
 *
 * This is typically used for displaying help information when a user
 * requests `--help` for a specific command.
 *
 * @param[in] pcCmd Pointer to the command descriptor whose options should be printed.
 */
void WshShellCmd_PrintOptionsOverview(const WshShellCmd_t* pcCmd);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_CMD_H */
