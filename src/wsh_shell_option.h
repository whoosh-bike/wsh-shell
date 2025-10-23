/**
 * @file wsh_shell_option.h
 * @brief Definition of shell command-line option object and creation macros.
 * 
 * This file provides structures and macros for defining and managing
 * command-line options in the WSH shell framework.
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_OPTIONS_H
#define __WSH_SHELL_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh_shell_types.h"

#define WSH_SHELL_OPT_ACCESS_NO      0x00
#define WSH_SHELL_OPT_ACCESS_READ    0x01
#define WSH_SHELL_OPT_ACCESS_WRITE   0x02
#define WSH_SHELL_OPT_ACCESS_EXECUTE 0x04
#define WSH_SHELL_OPT_ACCESS_ADMIN   0x08
#define WSH_SHELL_OPT_ACCESS_ANY \
    (WSH_SHELL_OPT_ACCESS_READ | WSH_SHELL_OPT_ACCESS_WRITE | WSH_SHELL_OPT_ACCESS_EXECUTE)

#if WSH_SHELL_PRINT_OPT_HELP_ENABLE
    #define WSH_SHELL_OPT_DESCR(descr) descr
#else
    #define WSH_SHELL_OPT_DESCR(descr) ""
#endif

/**
 * @brief Define a special option that matches the command name only (no flags).
 * @param[in] acc Access rights mask.
 */
#define WSH_SHELL_OPT_NO(acc, descr) \
    WSH_SHELL_OPTION_NO, (acc), 0, "--", "---", WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Define an option that triggers when input is provided with no flags.
 * @param[in] acc Access rights mask.
 */
#define WSH_SHELL_OPT_WAITS_INPUT(acc) WSH_SHELL_OPTION_WAITS_INPUT, (acc), 0, NULL, NULL, NULL

/**
 * @brief Define a built-in help option (e.g. `"--help"` or `"-h"`).
 */
#define WSH_SHELL_OPT_HELP()                                            \
    WSH_SHELL_OPTION_HELP, WSH_SHELL_OPT_ACCESS_ANY, 0, "-h", "--help", \
        WSH_SHELL_OPT_DESCR("Show command help information")

/**
 * @brief Define an option for entering interactive mode.
 */
#define WSH_SHELL_OPT_INTERACT(acc)                             \
    WSH_SHELL_OPTION_INTERACT, (acc), 0, "-i", "--interactive", \
        WSH_SHELL_OPT_DESCR("Run command in interactive mode")

/**
 * @brief Define an option that requires no arguments.
 * @param[in] acc Access rights mask.
 * @param[in] short Short flag (e.g. `"-a"`).
 * @param[in] long Long flag (e.g. `"--all"`).
 * @param[in] descr Help description.
 */
#define WSH_SHELL_OPT_WO_PARAM(acc, short, long, descr) \
    WSH_SHELL_OPTION_WO_PARAM, (acc), 0, (short), (long), WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Define an option that accepts multiple arguments.
 * @param[in] acc Access rights mask.
 * @param[in] argnum Number of arguments.
 * @param[in] short Short flag.
 * @param[in] long Long flag.
 * @param[in] descr Help description.
 */
#define WSH_SHELL_OPT_MULTI_ARG(acc, argnum, short, long, descr) \
    WSH_SHELL_OPTION_MULTI_ARG, (acc), (argnum), (short), (long), WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Define a string argument option.
 * @param[in] acc Access rights mask.
 * @param[in] short Short flag.
 * @param[in] long Long flag.
 * @param[in] descr Help description.
 */
#define WSH_SHELL_OPT_STR(acc, short, long, descr) \
    WSH_SHELL_OPTION_STR, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Define an integer argument option.
 * @param[in] acc Access rights mask.
 * @param[in] short Short flag.
 * @param[in] long Long flag.
 * @param[in] descr Help description.
 */
#define WSH_SHELL_OPT_INT(acc, short, long, descr) \
    WSH_SHELL_OPTION_INT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Define a float argument option.
 * @param[in] acc Access rights mask.
 * @param[in] short Short flag.
 * @param[in] long Long flag.
 * @param[in] descr Help description.
 */
#define WSH_SHELL_OPT_FLOAT(acc, short, long, descr) \
    WSH_SHELL_OPTION_FLOAT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

/**
 * @brief Marks the end of an option array.
 */
#define WSH_SHELL_OPT_END() WSH_SHELL_OPTION_END, WSH_SHELL_OPT_ACCESS_ANY, 0, NULL, NULL, NULL

/**
 * @brief Internal macro: full list of option types.
 */
#define WSH_SHELL_OPTION_TYPES_TABLE()                   \
    X_ENTRY(WSH_SHELL_OPTION_NO, "EMPTY")                \
    X_ENTRY(WSH_SHELL_OPTION_HELP, "HELP")               \
    X_ENTRY(WSH_SHELL_OPTION_INTERACT, "INTERACT")       \
    X_ENTRY(WSH_SHELL_OPTION_WO_PARAM, "WO_PARAM")       \
    X_ENTRY(WSH_SHELL_OPTION_MULTI_ARG, "MULTI_ARG")     \
    X_ENTRY(WSH_SHELL_OPTION_WAITS_INPUT, "WAITS_INPUT") \
    X_ENTRY(WSH_SHELL_OPTION_STR, "STR")                 \
    X_ENTRY(WSH_SHELL_OPTION_INT, "INT")                 \
    X_ENTRY(WSH_SHELL_OPTION_FLOAT, "FLOAT")             \
    X_ENTRY(WSH_SHELL_OPTION_END, "END")

/**
 * @enum WSH_SHELL_OPTION_TYPE_t
 * @brief Enumeration of all option types used by the shell.
 */
typedef enum {
#define X_ENTRY(id, str) id,
    WSH_SHELL_OPTION_TYPES_TABLE() WSH_SHELL_OPTION_ENUM_SIZE
#undef X_ENTRY
} WSH_SHELL_OPTION_TYPE_t;

/**
 * @brief Human-readable names of option types.
 */
static const WshShell_Char_t* WshShell_OptionTypeNames[] = {
#define X_ENTRY(id, str) str,
    WSH_SHELL_OPTION_TYPES_TABLE()
#undef X_ENTRY
};

/**
 * @brief Return the string name of an option type.
 *
 * @param[in] optType The option type.
 * @return Option type name as string.
 */
static inline const WshShell_Char_t* WshShell_OptTypeStr_Get(WSH_SHELL_OPTION_TYPE_t optType) {
    if (optType >= WSH_SHELL_OPTION_ENUM_SIZE)
        optType = WSH_SHELL_OPTION_NO;

    return WshShell_OptionTypeNames[optType];
}

/**
 * @struct WshShellOption_t
 * @brief Represents a shell command-line option.
 *
 * This structure describes a single command-line option used in a shell command.
 * It contains type, access level, required arguments, and optional short/long flags and description.
 */
typedef struct {
    WshShell_Size_t ID;               /**< Option identifier (for internal use). */
    WSH_SHELL_OPTION_TYPE_t Type;     /**< Option type. */
    WshShell_Size_t Access;           /**< Access level required to use this option. */
    WshShell_Size_t ArgNum;           /**< Number of arguments expected by this option. */
    const WshShell_Char_t* ShortName; /**< Short flag (e.g. `"-v"`) */
    const WshShell_Char_t* LongName;  /**< Long flag (e.g. `"--verbose"`) */
    const WshShell_Char_t* Descr;     /**< Optional help description. */
} WshShellOption_t;

/**
 * @struct WshShellOption_Ctx_t
 * @brief Option usage context during parsing.
 *
 * Used internally to reference which token matched which option.
 */
typedef struct {
    const WshShellOption_t* Option; /**< Pointer to matched option. */
    WshShell_Size_t TokenPos;       /**< Position of the matching token in command line. */
} WshShellOption_Ctx_t;

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_OPTIONS_H */
