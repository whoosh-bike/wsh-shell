

# File wsh\_shell\_user.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_user.c**](wsh__shell__user_8c.md)

[Go to the source code of this file](wsh__shell__user_8c_source.md)



* `#include "wsh_shell_user.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  WSH\_SHELL\_RET\_STATE\_t | [**WshShellUser\_Attach**](#function-wshshelluser_attach) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers, const [**WshShellUser\_t**](structWshShellUser__t.md) \* pcUserTable, WshShell\_Size\_t userNum) <br>_Initialize the shell user table._  |
|  WshShell\_Bool\_t | [**WshShellUser\_CheckCredentials**](#function-wshshelluser_checkcredentials) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers, WshShell\_Size\_t UserID, const WshShell\_Char\_t \* pcLogin, const WshShell\_Char\_t \* pcPassword) <br>_Verify login credentials of a user._  |
|  void | [**WshShellUser\_DeAttach**](#function-wshshelluser_deattach) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers) <br>_Destroy the user table._  |
|  const [**WshShellUser\_t**](structWshShellUser__t.md) \* | [**WshShellUser\_FindByCredentials**](#function-wshshelluser_findbycredentials) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers, const WshShell\_Char\_t \* pcLogin, const WshShell\_Char\_t \* pcPass) <br>_Finds a user by login and password credentials._  |
|  const [**WshShellUser\_t**](structWshShellUser__t.md) \* | [**WshShellUser\_GetUserByIndex**](#function-wshshelluser_getuserbyindex) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers, WshShell\_Size\_t idx) <br>_Retrieve a user by index._  |
|  WshShell\_Size\_t | [**WshShellUser\_GetUsersNum**](#function-wshshelluser_getusersnum) ([**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) \* pShellUsers) <br>_Get the number of users registered in the shell._  |




























## Public Functions Documentation




### function WshShellUser\_Attach 

_Initialize the shell user table._ 
```C++
WSH_SHELL_RET_STATE_t WshShellUser_Attach (
    WshShellUser_Table_t * pShellUsers,
    const WshShellUser_t * pcUserTable,
    WshShell_Size_t userNum
) 
```



Registers a static user table for the shell instance.




**Parameters:**


* `pShellUsers` Pointer to the shell's user table. 
* `pcUserTable` Pointer to the static array of user records. 
* `userNum` Number of users in the array.



**Return value:**


* `WSH_SHELL_RET_STATE_SUCCESS` Initialization succeeded. 
* `WSH_SHELL_RET_STATE_ERR_PARAM` Invalid input arguments. 
* `WSH_SHELL_RET_STATE_ERR_BUSY` Table was already initialized. 




        

<hr>



### function WshShellUser\_CheckCredentials 

_Verify login credentials of a user._ 
```C++
WshShell_Bool_t WshShellUser_CheckCredentials (
    WshShellUser_Table_t * pShellUsers,
    WshShell_Size_t UserID,
    const WshShell_Char_t * pcLogin,
    const WshShell_Char_t * pcPassword
) 
```



Validates login and password against the given user index.




**Parameters:**


* `pShellUsers` Pointer to the user table. 
* `UserID` Index of the user to validate. 
* `pcLogin` Pointer to the login string. 
* `pcPassword` Pointer to the password string.



**Return value:**


* `true` If credentials match. 
* `false` If mismatch or error. 




        

<hr>



### function WshShellUser\_DeAttach 

_Destroy the user table._ 
```C++
void WshShellUser_DeAttach (
    WshShellUser_Table_t * pShellUsers
) 
```



Resets the user list and count to zero.




**Parameters:**


* `pShellUsers` Pointer to the user table to reset. 




        

<hr>



### function WshShellUser\_FindByCredentials 

_Finds a user by login and password credentials._ 
```C++
const WshShellUser_t * WshShellUser_FindByCredentials (
    WshShellUser_Table_t * pShellUsers,
    const WshShell_Char_t * pcLogin,
    const WshShell_Char_t * pcPass
) 
```



Searches the given user table for a user whose login and password match the provided credentials. Comparison is done using the `WshShellUser_CheckCredentials` function.




**Parameters:**


* `pShellUsers` Pointer to the user table. 
* `pcLogin` Pointer to the login string. 
* `pcPass` Pointer to the password string.



**Returns:**

Pointer to the matching user object if found; NULL otherwise.




**Note:**

Returns NULL if any input pointer is NULL or if the user table is empty. 





        

<hr>



### function WshShellUser\_GetUserByIndex 

_Retrieve a user by index._ 
```C++
const WshShellUser_t * WshShellUser_GetUserByIndex (
    WshShellUser_Table_t * pShellUsers,
    WshShell_Size_t idx
) 
```





**Parameters:**


* `pShellUsers` Pointer to the user table. 
* `idx` Index of the user.



**Returns:**

Pointer to the user object, or NULL if invalid. 





        

<hr>



### function WshShellUser\_GetUsersNum 

_Get the number of users registered in the shell._ 
```C++
WshShell_Size_t WshShellUser_GetUsersNum (
    WshShellUser_Table_t * pShellUsers
) 
```





**Parameters:**


* `pShellUsers` Pointer to the user table.



**Returns:**

Number of users, or 0 if uninitialized or NULL. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_user.c`

