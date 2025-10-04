

# File wsh\_shell\_cfg\_def.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_cfg\_def.h**](wsh__shell__cfg__def_8h.md)

[Go to the source code of this file](wsh__shell__cfg__def_8h_source.md)



* `#include "wsh_shell_types.h"`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**WshShellRetState\_TranslateToProject**](#function-wshshellretstate_translatetoproject) (WSH\_SHELL\_RET\_STATE\_t state) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**RET\_STATE\_MAP\_TABLE**](wsh__shell__cfg__def_8h.md#define-ret_state_map_table) () `/* multi line expression */`<br> |
| define  | [**WSH\_SHELL\_ASSERT**](wsh__shell__cfg__def_8h.md#define-wsh_shell_assert) (exp) <br> |
| define  | [**WSH\_SHELL\_AUTOCOMPLETE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_autocomplete)  `1`<br> |
| define  | [**WSH\_SHELL\_AUTOCOMPLETE\_PAD\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_autocomplete_pad_len)  `32`<br> |
| define  | [**WSH\_SHELL\_AUTOCOMPLETE\_PAD\_SYM**](wsh__shell__cfg__def_8h.md#define-wsh_shell_autocomplete_pad_sym)  `'.'`<br> |
| define  | [**WSH\_SHELL\_CMD\_ARGS\_MAX\_NUM**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_args_max_num)  `16`<br> |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_ADMIN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_group_admin)  `0x01`<br> |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_ALL**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_group_all)  `((WshShell\_Size\_t)(~0U))`<br> |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_MAX\_COUNT**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_group_max_count)  `4`<br> |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_NONE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_group_none)  `0x00`<br> |
| define  | [**WSH\_SHELL\_CMD\_GROUP\_READER**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_group_reader)  `0x02`<br> |
| define  | [**WSH\_SHELL\_CMD\_NAME\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_name_len)  `16`<br> |
| define  | [**WSH\_SHELL\_CMD\_OPTIONS\_MAX\_NUM**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_options_max_num)  `16`<br> |
| define  | [**WSH\_SHELL\_CMD\_PRINT\_OPT\_OVERVIEW**](wsh__shell__cfg__def_8h.md#define-wsh_shell_cmd_print_opt_overview)  `1`<br> |
| define  | [**WSH\_SHELL\_DEF\_COMMAND**](wsh__shell__cfg__def_8h.md#define-wsh_shell_def_command)  `1`<br> |
| define  | [**WSH\_SHELL\_DEV\_NAME\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_dev_name_len)  `16`<br> |
| define  | [**WSH\_SHELL\_ESC\_BUFF\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_esc_buff_len)  `8`<br> |
| define  | [**WSH\_SHELL\_HEADER**](wsh__shell__cfg__def_8h.md#define-wsh_shell_header)  `"\                \_\_               \_\_         \_\_\_\_  \r\n\ \_      \_\_\_\_\_\_\_/ /\_        \_\_\_\_\_/ /\_  \_\_\_  / / /  \r\n\\| \| /\| / / \_\_\_/ \_\_ \\\_\_\_\_\_\_/ \_\_\_/ \_\_ \\/ \_ \\/ / /\r\n\\| \|/ \|/ (\_\_  ) / / /\_\_\_\_\_(\_\_  ) / / /  \_\_/ / /    \r\n\\|\_\_/\|\_\_/\_\_\_\_/\_/ /\_/     /\_\_\_\_/\_/ /\_/\\\_\_\_/\_/\_/    \r\n\\r\n"`<br> |
| define  | [**WSH\_SHELL\_HISTORY**](wsh__shell__cfg__def_8h.md#define-wsh_shell_history)  `1`<br> |
| define  | [**WSH\_SHELL\_HISTORY\_BUFF\_SIZE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_history_buff_size)  `256`<br> |
| define  | [**WSH\_SHELL\_INTERACTIVE\_MODE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_interactive_mode)  `1`<br> |
| define  | [**WSH\_SHELL\_INTR\_BUFF\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_intr_buff_len)  `64`<br> |
| define  | [**WSH\_SHELL\_LOGIN\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_login_len)  `16`<br> |
| define  | [**WSH\_SHELL\_MEMCMP**](wsh__shell__cfg__def_8h.md#define-wsh_shell_memcmp) (pD, pS, sz) `memcmp((pD), (pS), (sz))`<br> |
| define  | [**WSH\_SHELL\_MEMCPY**](wsh__shell__cfg__def_8h.md#define-wsh_shell_memcpy) (pD, pS, sz) `memcpy((pD), (pS), (sz))`<br> |
| define  | [**WSH\_SHELL\_MEMSET**](wsh__shell__cfg__def_8h.md#define-wsh_shell_memset) (pD, c, sz) `memset((pD), (c), (sz))`<br> |
| define  | [**WSH\_SHELL\_OPTION\_LONG\_NAME\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_option_long_name_len)  `16`<br> |
| define  | [**WSH\_SHELL\_OPTION\_SHORT\_NAME\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_option_short_name_len)  `2`<br> |
| define  | [**WSH\_SHELL\_PASS\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_pass_len)  `16`<br> |
| define  | [**WSH\_SHELL\_PRINT**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print) (\_f\_, ...) `/* multi line expression */`<br> |
| define  | [**WSH\_SHELL\_PRINT\_ERR\_ENABLE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print_err_enable)  `1`<br> |
| define  | [**WSH\_SHELL\_PRINT\_INFO\_ENABLE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print_info_enable)  `1`<br> |
| define  | [**WSH\_SHELL\_PRINT\_OPT\_HELP\_ENABLE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print_opt_help_enable)  `1`<br> |
| define  | [**WSH\_SHELL\_PRINT\_SYS\_ENABLE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print_sys_enable)  `1`<br> |
| define  | [**WSH\_SHELL\_PRINT\_WARN\_ENABLE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_print_warn_enable)  `1`<br> |
| define  | [**WSH\_SHELL\_PROMPT\_WAIT**](wsh__shell__cfg__def_8h.md#define-wsh_shell_prompt_wait)  `1`<br> |
| define  | [**WSH\_SHELL\_PS1\_CUSTOM**](wsh__shell__cfg__def_8h.md#define-wsh_shell_ps1_custom)  `1`<br> |
| define  | [**WSH\_SHELL\_PS1\_MAX\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_ps1_max_len)  `128`<br> |
| define  | [**WSH\_SHELL\_PS1\_TEMPLATE**](wsh__shell__cfg__def_8h.md#define-wsh_shell_ps1_template)  `"%r%b%c6%d%c7@%c5%u%c2%i %c7&gt; %r%c7"`<br> |
| define  | [**WSH\_SHELL\_SALT\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_salt_len)  `16`<br> |
| define  | [**WSH\_SHELL\_SALT\_PASS\_HASH\_LEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_salt_pass_hash_len)  `64`<br> |
| define  | [**WSH\_SHELL\_SNPRINTF**](wsh__shell__cfg__def_8h.md#define-wsh_shell_snprintf) (buf, size, ...) `snprintf((buf), (size), \_\_VA\_ARGS\_\_)`<br> |
| define  | [**WSH\_SHELL\_STRCMP**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strcmp) (pS1, pS2) `strcmp((pS1), (pS2))`<br> |
| define  | [**WSH\_SHELL\_STRCPY**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strcpy) (pD, pS) `strcpy((pD), (pS))`<br> |
| define  | [**WSH\_SHELL\_STRLEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strlen) (pS) `strlen((pS))`<br> |
| define  | [**WSH\_SHELL\_STRNCMP**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strncmp) (pS1, pS2, len) `strncmp((pS1), (pS2), (len))`<br> |
| define  | [**WSH\_SHELL\_STRNCPY**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strncpy) (pD, pS, sz) `strncpy((pD), (pS), (sz))`<br> |
| define  | [**WSH\_SHELL\_STRNLEN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strnlen) (pS, len) `strnlen((pS), (len))`<br> |
| define  | [**WSH\_SHELL\_STRTOF**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strtof) (pN, pE) `strtof((pN), (pE))`<br> |
| define  | [**WSH\_SHELL\_STRTOL**](wsh__shell__cfg__def_8h.md#define-wsh_shell_strtol) (pS, pE, radix) `strtol((pS), (pE), (radix))`<br> |
| define  | [**WSH\_SHELL\_TARGET\_OS**](wsh__shell__cfg__def_8h.md#define-wsh_shell_target_os)  `"Bare-metal"`<br> |
| define  | [**WSH\_SHELL\_USER\_ACCESS\_ADMIN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_user_access_admin)  `(WSH\_SHELL\_OPT\_ACCESS\_ANY)`<br> |
| define  | [**WSH\_SHELL\_USER\_ACCESS\_READER**](wsh__shell__cfg__def_8h.md#define-wsh_shell_user_access_reader)  `(WSH\_SHELL\_OPT\_ACCESS\_ANY)`<br> |
| define  | [**WSH\_SHELL\_USER\_GROUP\_ADMIN**](wsh__shell__cfg__def_8h.md#define-wsh_shell_user_group_admin)  `(WSH\_SHELL\_CMD\_GROUP\_ALL)`<br> |
| define  | [**WSH\_SHELL\_USER\_GROUP\_READER**](wsh__shell__cfg__def_8h.md#define-wsh_shell_user_group_reader)  `(WSH\_SHELL\_CMD\_GROUP\_READER)`<br> |
| define  | [**X\_MAP\_ENTRY**](wsh__shell__cfg__def_8h.md#define-x_map_entry) (proj, shell) `/* multi line expression */`<br> |

## Public Static Functions Documentation




### function WshShellRetState\_TranslateToProject 

```C++
static inline bool WshShellRetState_TranslateToProject (
    WSH_SHELL_RET_STATE_t state
) 
```




<hr>
## Macro Definition Documentation





### define RET\_STATE\_MAP\_TABLE 

```C++
#define RET_STATE_MAP_TABLE (
    
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_ASSERT 

```C++
#define WSH_SHELL_ASSERT (
    exp
) 
```




<hr>



### define WSH\_SHELL\_AUTOCOMPLETE 

```C++
#define WSH_SHELL_AUTOCOMPLETE `1`
```




<hr>



### define WSH\_SHELL\_AUTOCOMPLETE\_PAD\_LEN 

```C++
#define WSH_SHELL_AUTOCOMPLETE_PAD_LEN `32`
```




<hr>



### define WSH\_SHELL\_AUTOCOMPLETE\_PAD\_SYM 

```C++
#define WSH_SHELL_AUTOCOMPLETE_PAD_SYM `'.'`
```




<hr>



### define WSH\_SHELL\_CMD\_ARGS\_MAX\_NUM 

```C++
#define WSH_SHELL_CMD_ARGS_MAX_NUM `16`
```




<hr>



### define WSH\_SHELL\_CMD\_GROUP\_ADMIN 

```C++
#define WSH_SHELL_CMD_GROUP_ADMIN `0x01`
```




<hr>



### define WSH\_SHELL\_CMD\_GROUP\_ALL 

```C++
#define WSH_SHELL_CMD_GROUP_ALL `((WshShell_Size_t)(~0U))`
```




<hr>



### define WSH\_SHELL\_CMD\_GROUP\_MAX\_COUNT 

```C++
#define WSH_SHELL_CMD_GROUP_MAX_COUNT `4`
```




<hr>



### define WSH\_SHELL\_CMD\_GROUP\_NONE 

```C++
#define WSH_SHELL_CMD_GROUP_NONE `0x00`
```




<hr>



### define WSH\_SHELL\_CMD\_GROUP\_READER 

```C++
#define WSH_SHELL_CMD_GROUP_READER `0x02`
```




<hr>



### define WSH\_SHELL\_CMD\_NAME\_LEN 

```C++
#define WSH_SHELL_CMD_NAME_LEN `16`
```




<hr>



### define WSH\_SHELL\_CMD\_OPTIONS\_MAX\_NUM 

```C++
#define WSH_SHELL_CMD_OPTIONS_MAX_NUM `16`
```




<hr>



### define WSH\_SHELL\_CMD\_PRINT\_OPT\_OVERVIEW 

```C++
#define WSH_SHELL_CMD_PRINT_OPT_OVERVIEW `1`
```




<hr>



### define WSH\_SHELL\_DEF\_COMMAND 

```C++
#define WSH_SHELL_DEF_COMMAND `1`
```




<hr>



### define WSH\_SHELL\_DEV\_NAME\_LEN 

```C++
#define WSH_SHELL_DEV_NAME_LEN `16`
```




<hr>



### define WSH\_SHELL\_ESC\_BUFF\_LEN 

```C++
#define WSH_SHELL_ESC_BUFF_LEN `8`
```




<hr>



### define WSH\_SHELL\_HEADER 

```C++
#define WSH_SHELL_HEADER `"\                __               __         ____  \r\n\ _      _______/ /_        _____/ /_  ___  / / /  \r\n\| | /| / / ___/ __ \\______/ ___/ __ \\/ _ \\/ / /\r\n\| |/ |/ (__  ) / / /_____(__  ) / / /  __/ / /    \r\n\|__/|__/____/_/ /_/     /____/_/ /_/\\___/_/_/    \r\n\\r\n"`
```




<hr>



### define WSH\_SHELL\_HISTORY 

```C++
#define WSH_SHELL_HISTORY `1`
```




<hr>



### define WSH\_SHELL\_HISTORY\_BUFF\_SIZE 

```C++
#define WSH_SHELL_HISTORY_BUFF_SIZE `256`
```




<hr>



### define WSH\_SHELL\_INTERACTIVE\_MODE 

```C++
#define WSH_SHELL_INTERACTIVE_MODE `1`
```




<hr>



### define WSH\_SHELL\_INTR\_BUFF\_LEN 

```C++
#define WSH_SHELL_INTR_BUFF_LEN `64`
```




<hr>



### define WSH\_SHELL\_LOGIN\_LEN 

```C++
#define WSH_SHELL_LOGIN_LEN `16`
```




<hr>



### define WSH\_SHELL\_MEMCMP 

```C++
#define WSH_SHELL_MEMCMP (
    pD,
    pS,
    sz
) `memcmp((pD), (pS), (sz))`
```




<hr>



### define WSH\_SHELL\_MEMCPY 

```C++
#define WSH_SHELL_MEMCPY (
    pD,
    pS,
    sz
) `memcpy((pD), (pS), (sz))`
```




<hr>



### define WSH\_SHELL\_MEMSET 

```C++
#define WSH_SHELL_MEMSET (
    pD,
    c,
    sz
) `memset((pD), (c), (sz))`
```




<hr>



### define WSH\_SHELL\_OPTION\_LONG\_NAME\_LEN 

```C++
#define WSH_SHELL_OPTION_LONG_NAME_LEN `16`
```




<hr>



### define WSH\_SHELL\_OPTION\_SHORT\_NAME\_LEN 

```C++
#define WSH_SHELL_OPTION_SHORT_NAME_LEN `2`
```




<hr>



### define WSH\_SHELL\_PASS\_LEN 

```C++
#define WSH_SHELL_PASS_LEN `16`
```




<hr>



### define WSH\_SHELL\_PRINT 

```C++
#define WSH_SHELL_PRINT (
    _f_,
    ...
) `/* multi line expression */`
```




<hr>



### define WSH\_SHELL\_PRINT\_ERR\_ENABLE 

```C++
#define WSH_SHELL_PRINT_ERR_ENABLE `1`
```




<hr>



### define WSH\_SHELL\_PRINT\_INFO\_ENABLE 

```C++
#define WSH_SHELL_PRINT_INFO_ENABLE `1`
```




<hr>



### define WSH\_SHELL\_PRINT\_OPT\_HELP\_ENABLE 

```C++
#define WSH_SHELL_PRINT_OPT_HELP_ENABLE `1`
```




<hr>



### define WSH\_SHELL\_PRINT\_SYS\_ENABLE 

```C++
#define WSH_SHELL_PRINT_SYS_ENABLE `1`
```




<hr>



### define WSH\_SHELL\_PRINT\_WARN\_ENABLE 

```C++
#define WSH_SHELL_PRINT_WARN_ENABLE `1`
```




<hr>



### define WSH\_SHELL\_PROMPT\_WAIT 

```C++
#define WSH_SHELL_PROMPT_WAIT `1`
```




<hr>



### define WSH\_SHELL\_PS1\_CUSTOM 

```C++
#define WSH_SHELL_PS1_CUSTOM `1`
```




<hr>



### define WSH\_SHELL\_PS1\_MAX\_LEN 

```C++
#define WSH_SHELL_PS1_MAX_LEN `128`
```




<hr>



### define WSH\_SHELL\_PS1\_TEMPLATE 

```C++
#define WSH_SHELL_PS1_TEMPLATE `"%r%b%c6%d%c7@%c5%u%c2%i %c7> %r%c7"`
```




<hr>



### define WSH\_SHELL\_SALT\_LEN 

```C++
#define WSH_SHELL_SALT_LEN `16`
```




<hr>



### define WSH\_SHELL\_SALT\_PASS\_HASH\_LEN 

```C++
#define WSH_SHELL_SALT_PASS_HASH_LEN `64`
```




<hr>



### define WSH\_SHELL\_SNPRINTF 

```C++
#define WSH_SHELL_SNPRINTF (
    buf,
    size,
    ...
) `snprintf((buf), (size), __VA_ARGS__)`
```




<hr>



### define WSH\_SHELL\_STRCMP 

```C++
#define WSH_SHELL_STRCMP (
    pS1,
    pS2
) `strcmp((pS1), (pS2))`
```




<hr>



### define WSH\_SHELL\_STRCPY 

```C++
#define WSH_SHELL_STRCPY (
    pD,
    pS
) `strcpy((pD), (pS))`
```




<hr>



### define WSH\_SHELL\_STRLEN 

```C++
#define WSH_SHELL_STRLEN (
    pS
) `strlen((pS))`
```




<hr>



### define WSH\_SHELL\_STRNCMP 

```C++
#define WSH_SHELL_STRNCMP (
    pS1,
    pS2,
    len
) `strncmp((pS1), (pS2), (len))`
```




<hr>



### define WSH\_SHELL\_STRNCPY 

```C++
#define WSH_SHELL_STRNCPY (
    pD,
    pS,
    sz
) `strncpy((pD), (pS), (sz))`
```




<hr>



### define WSH\_SHELL\_STRNLEN 

```C++
#define WSH_SHELL_STRNLEN (
    pS,
    len
) `strnlen((pS), (len))`
```




<hr>



### define WSH\_SHELL\_STRTOF 

```C++
#define WSH_SHELL_STRTOF (
    pN,
    pE
) `strtof((pN), (pE))`
```




<hr>



### define WSH\_SHELL\_STRTOL 

```C++
#define WSH_SHELL_STRTOL (
    pS,
    pE,
    radix
) `strtol((pS), (pE), (radix))`
```




<hr>



### define WSH\_SHELL\_TARGET\_OS 

```C++
#define WSH_SHELL_TARGET_OS `"Bare-metal"`
```




<hr>



### define WSH\_SHELL\_USER\_ACCESS\_ADMIN 

```C++
#define WSH_SHELL_USER_ACCESS_ADMIN `(WSH_SHELL_OPT_ACCESS_ANY)`
```




<hr>



### define WSH\_SHELL\_USER\_ACCESS\_READER 

```C++
#define WSH_SHELL_USER_ACCESS_READER `(WSH_SHELL_OPT_ACCESS_ANY)`
```




<hr>



### define WSH\_SHELL\_USER\_GROUP\_ADMIN 

```C++
#define WSH_SHELL_USER_GROUP_ADMIN `(WSH_SHELL_CMD_GROUP_ALL)`
```




<hr>



### define WSH\_SHELL\_USER\_GROUP\_READER 

```C++
#define WSH_SHELL_USER_GROUP_READER `(WSH_SHELL_CMD_GROUP_READER)`
```




<hr>



### define X\_MAP\_ENTRY 

```C++
#define X_MAP_ENTRY (
    proj,
    shell
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_cfg_def.h`

