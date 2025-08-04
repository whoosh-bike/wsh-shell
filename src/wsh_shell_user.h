/**
 * @file wsh_shell_user.h
 * @brief User object definition and API's
 * @author Whoosh Embedded Team
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_USER_H
#define __WSH_SHELL_USER_H

#include "wsh_shell_types.h"

#define WSH_SHELL_USER_UNDEF_ID ((WshShell_Size_t)(~0U))

/**
 * @defgroup ShellUser User
 * @brief User object definition and API's
 * @{
 */

/**
 * @struct WshShellUser_t wsh_shell_user.h "wsh_shell_user.h"
 * @brief Structure implementation of User object
 */
typedef struct {
    const char* pLogin;     /**< Array for storing login string. */
    const char* pPwd;       /**< Array for storing password string. */
    WshShell_Size_t Groups; /**< Command groups that are accessible for user. */
    WshShell_Size_t Rights; /**< Command execution right for accessible for user. */
} WshShellUser_t;

/**
 * @defgroup PublicUserFunctions Public functions
 * @brief Public functions for interaction with user objects.
 * @{
 */

/**
 * @brief Initialize table of users for shell object to use.
 * 
 * @param[in] pUserTable: Pointer to an array with defined users.
 * @param[in] usersNum: Number of users in table.
 *
 * @retval WSH_SHELL_RET_STATE_ERR_PARAM: Something wrong with input arguments.
 * @retval WSH_SHELL_RET_STATE_SUCCESS: Success.
 */
WSH_SHELL_RET_STATE_t WshShellUser_Init(const WshShellUser_t* pUserTable, WshShell_Size_t usersNum);

/**
 * @brief Get number of available users.
 * 
 * @retval WshShellSize_t: Amount of available users.
 */
WshShell_Size_t WshShellUser_GetUsersNum(void);

/**
 * @brief Get user by it's id in users table
 * 
 * @param[in] id: Index of user in table.
 *
 * @retval const WshShellUser_t*: Const pointer to user object.
 * @retval NULL: No user with such ID.
 */
const WshShellUser_t* WshShellUser_GetUserByIndex(WshShell_Size_t id);

/**
 * @brief Check if user credentials are equal to input strings.
 * 
 * @param[in] UserID: Index of a user.
 * @param[in] pLogin: Pointer to a string with user login.
 * @param[in] pPassword: Pointer to a string with user password.
 *
 * @retval true: Input strings are equal to user credentials.
 * @retval false: Input strings are not equal to user credentials.
 */
bool WshShellUser_CheckCredentials(WshShell_Size_t UserID, const char* pLogin, const char* pPassword);

/**
 * @brief Froget current users table.
 */
void WshShellUser_Destroy(void);

/**
 * @}
 */

/**
 * @}
 */

#endif /* __WSH_SHELL_USER_H */
