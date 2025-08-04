/**
 * @file wsh_shell_cmd_def.h
 * @brief Functions for fast creating and registration of default shell commands.
 * @author Whoosh Embedded Team
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __WSH_SHELL_CMD_DEF_H
#define __WSH_SHELL_CMD_DEF_H

#include "wsh_shell_cmd.h"
#include "wsh_shell_types.h"

/* clang-format off */
#define WSH_SHELL_HELP_OPT_TABLE() \
X(WSH_SHELL_HELP_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X(WSH_SHELL_HELP_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_ACCESS_ANY)) \
X(WSH_SHELL_HELP_OPT_END, WSH_SHELL_OPT_END())

#define X(en, m) en,
typedef enum {
	WSH_SHELL_HELP_OPT_TABLE()
	WSH_SHELL_HELP_OPT_ENUM_SIZE
} WSH_SHELL_HELP_OPT_t;
#undef X

#define WSH_SHELL_CLS_OPT_TABLE() \
X(WSH_SHELL_CLS_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X(WSH_SHELL_CLS_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_ACCESS_ANY)) \
X(WSH_SHELL_CLS_OPT_END, WSH_SHELL_OPT_END())

#define X(en, m) en,
typedef enum {
	WSH_SHELL_CLS_OPT_TABLE()
	WSH_SHELL_CLS_OPT_ENUM_SIZE
} WSH_SHELL_CLS_OPT_t;
#undef X

#define WSH_SHELL_ECHO_OPT_TABLE() \
X(WSH_SHELL_ECHO_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X(WSH_SHELL_ECHO_OPT_DEF, WSH_SHELL_OPT_WAITS_INPUT(WSH_SHELL_ACCESS_ANY)) \
X(WSH_SHELL_ECHO_OPT_END, WSH_SHELL_OPT_END())

#define X(en, m) en,
typedef enum {
	WSH_SHELL_ECHO_OPT_TABLE()
	WSH_SHELL_ECHO_OPT_ENUM_SIZE
} WSH_SHELL_ECHO_OPT_t;
#undef X

#define WSH_SHELL_EXIT_OPT_TABLE() \
X(WSH_SHELL_EXIT_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X(WSH_SHELL_EXIT_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_ACCESS_ANY)) \
X(WSH_SHELL_EXIT_OPT_END, WSH_SHELL_OPT_END())

#define X(en, m) en,
typedef enum {
	WSH_SHELL_EXIT_OPT_TABLE()
	WSH_SHELL_EXIT_OPT_ENUM_SIZE
} WSH_SHELL_EXIT_OPT_t;
#undef X
/* clang-format on */

WSH_SHELL_RET_STATE_t WshShellCmdDef_Help_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv);
WSH_SHELL_RET_STATE_t WshShellCmdDef_Cls_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                    const char** pArgv);
WSH_SHELL_RET_STATE_t WshShellCmdDef_Echo_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv);
WSH_SHELL_RET_STATE_t WshShellCmdDef_Exit_Executable(const WshShellCmd_t* pCmd, WshShell_Size_t argc,
                                                     const char** pArgv);

#endif /* __WSH_SHELL_CMD_DEF_H */
