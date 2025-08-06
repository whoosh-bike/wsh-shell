/**
 * @file wsh_shell_user.h
 * @brief Shell user management API
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_USER_H
#define __WSH_SHELL_USER_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup WshShellUser Shell User Management
 * @brief Functionality for managing user authentication and access rights.
 * @{
 */

/**
 * @brief User object definition.
 * 
 * This structure represents a shell user with login, password,
 * and associated access groups and rights.
 */
typedef struct {
    const WshShell_Char_t* Login; /**< Pointer to login string. */
    const WshShell_Char_t* Pass;  /**< Pointer to password string. */
    WshShell_Size_t Groups;       /**< Bitmask of accessible command groups. */
    WshShell_Size_t Rights;       /**< Bitmask of execution rights. */
} WshShellUser_t;

/**
 * @brief Table of registered shell users.
 * 
 * This structure contains a list of shell users and total count.
 */
typedef struct {
    const WshShellUser_t* List; /**< Pointer to an array of user objects. */
    WshShell_Size_t Num;        /**< Number of users in the list. */
} WshShellUser_Table_t;

/**
 * @defgroup WshShellUserAPI Public API
 * @brief Public functions for managing shell users.
 * @{
 */

/**
 * @brief Initialize the shell user table.
 * 
 * Registers a static user table for the shell instance.
 * 
 * @param[out] pShellUsers Pointer to the shell's user table.
 * @param[in]  pcUserTable Pointer to the static array of user records.
 * @param[in]  userNum    Number of users in the array.
 * 
 * @retval WSH_SHELL_RET_STATE_SUCCESS   Initialization succeeded.
 * @retval WSH_SHELL_RET_STATE_ERR_PARAM Invalid input arguments.
 * @retval WSH_SHELL_RET_STATE_ERR_BUSY  Table was already initialized.
 */
WSH_SHELL_RET_STATE_t WshShellUser_Attach(WshShellUser_Table_t* pShellUsers,
                                          const WshShellUser_t* pcUserTable,
                                          WshShell_Size_t userNum);

/**
 * @brief Destroy the user table.
 * 
 * Resets the user list and count to zero.
 * 
 * @param[in,out] pShellUsers Pointer to the user table to reset.
 */
void WshShellUser_DeAttach(WshShellUser_Table_t* pShellUsers);

/**
 * @brief Get the number of users registered in the shell.
 * 
 * @param[in] pShellUsers Pointer to the user table.
 * 
 * @return Number of users, or 0 if uninitialized or NULL.
 */
WshShell_Size_t WshShellUser_GetUsersNum(WshShellUser_Table_t* pShellUsers);

/**
 * @brief Retrieve a user by index.
 * 
 * @param[in] pShellUsers Pointer to the user table.
 * @param[in] idx         Index of the user.
 * 
 * @return Pointer to the user object, or NULL if invalid.
 */
const WshShellUser_t* WshShellUser_GetUserByIndex(WshShellUser_Table_t* pShellUsers,
                                                  WshShell_Size_t idx);

/**
 * @brief Verify login credentials of a user.
 * 
 * Validates login and password against the given user index.
 * 
 * @param[in] pShellUsers Pointer to the user table.
 * @param[in] UserID      Index of the user to validate.
 * @param[in] pcLogin     Pointer to the login string.
 * @param[in] pcPassword  Pointer to the password string.
 * 
 * @retval true  If credentials match.
 * @retval false If mismatch or error.
 */
WshShell_Bool_t WshShellUser_CheckCredentials(WshShellUser_Table_t* pShellUsers,
                                              WshShell_Size_t UserID,
                                              const WshShell_Char_t* pcLogin,
                                              const WshShell_Char_t* pcPassword);

/**
 * @brief Finds a user by login and password credentials.
 *
 * Searches the given user table for a user whose login and password match
 * the provided credentials. Comparison is done using the `WshShellUser_CheckCredentials` function.
 *
 * @param[in] pShellUsers Pointer to the user table.
 * @param[in] pcLogin     Pointer to the login string.
 * @param[in] pcPass      Pointer to the password string.
 *
 * @return Pointer to the matching user object if found; NULL otherwise.
 *
 * @note Returns NULL if any input pointer is NULL or if the user table is empty.
 */
const WshShellUser_t* WshShellUser_FindByCredentials(WshShellUser_Table_t* pShellUsers,
                                                     const WshShell_Char_t* pcLogin,
                                                     const WshShell_Char_t* pcPass);

/**
 * @} // end of WshShellUserAPI
 */

/**
 * @} // end of WshShellUser
 */

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_USER_H */
