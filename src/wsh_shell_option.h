/**
 * @file wsh_shell_option.h
 * @brief Option object definition and API's
 * 
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_OPTIONS_H
#define __WSH_SHELL_OPTIONS_H

#include "wsh_shell_types.h"

/**
 * @defgroup ShellOption Option
 * @brief Option object definition and API's
 *
 * Option is the most basic concept and object in all shell interface.
 * Each option object describes one string token called **FLAG**.
 * Each flag exists in two variants:
 * - *Long flag* - starts with \"--\"
 * - *Short flag* - starts with \"-\"
 * @{
 */

/**
 * @name Constans
 * Macros constants for option objects.
 */
/**@{ */

/**
 * @def WSH_SHELL_OPTION_UNDEF_ID
 * @brief Undefined option identificator.
 */
#define WSH_SHELL_OPTION_UNDEF_ID ((WshShell_Size_t)(~0UL))

/**@} */

/**
 * @defgroup OptionCreationShortcuts Option creation shortcuts
 * @brief Macroses for fast option objects creation
 * @{
 */

/**
 * @def WSH_SHELL_OPT_NO(ab)
 * @brief Define an option that will be called when command is called only by it's name. Example "help".
 * @param[in] ab: Option access bits.
 */
#define WSH_SHELL_OPT_NO(ab) WSH_SHELL_OPTION_NO, (ab), 0, NULL, NULL, NULL

/**
 * @def WSH_SHELL_OPT_WAITS_INPUT(ab)
 * @brief Define an option which waits input without any additional flags. Example "print hello my pretty shell".
 * @param[in] ab: Option access bits.
 */
#define WSH_SHELL_OPT_WAITS_INPUT(ab) WSH_SHELL_OPTION_WAITS_INPUT, (ab), 0, NULL, NULL, NULL

#if WSH_SHELL_OPT_HELP_ENABLE
    /**
 * @def WSH_SHELL_OPT_HELP
 * Define an option which will print command and other options description. Example "yourCmd --help".
 */
    #define WSH_SHELL_OPT_HELP() \
        WSH_SHELL_OPTION_HELP, WSH_SHELL_ACCESS_ANY, 0, "-h", "--help", "Show command description"

    /**
 * @def WSH_SHELL_OPT_INTERACT
 * Define an option for running command in interactive mode. Example "yourCmd --interactive".
 */
    #define WSH_SHELL_OPT_INTERACT() \
        WSH_SHELL_OPTION_INTERACT, WSH_SHELL_ACCESS_ANY, 0, "-i", "--interactive", "Run command in interactive mode"

    /**
 * @def WSH_SHELL_OPT_WO_PARAM(ab, sn, ln, h)
 * @brief Define an option which can be called without any additional parameters. Example "help --all".
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_WO_PARAM(ab, sn, ln, h) WSH_SHELL_OPTION_WO_PARAM, (ab), 0, (sn), (ln), (h)

    /**
 * @def WSH_SHELL_OPT_MULTI_ARG(ab, an, sn, ln, h)
 * @brief Define an option which can be called with multiple different parameters. Example "calc --sum 5 10".
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_MULTI_ARG(ab, an, sn, ln, h) WSH_SHELL_OPTION_MULTI_ARG, (ab), (an), (sn), (ln), (h)

    /**
 * @def WSH_SHELL_OPT_STR(ab, sn, ln, h)
 * @brief Define an option which expects string parameter after flag. Example "write --string string to be written"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_STR(ab, sn, ln, h) WSH_SHELL_OPTION_STR, (ab), 1, (sn), (ln), (h)

    /**
 * @def WSH_SHELL_OPT_INT(ab, sn, ln, h)
 * @brief Define an option which expects integer parameter after flag. Example "calc --root 4"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_INT(ab, sn, ln, h) WSH_SHELL_OPTION_INT, (ab), 1, (sn), (ln), (h)

    /**
 * @def WSH_SHELL_OPT_FLOAT(ab, sn, ln, h)
 * @brief Define an option which expects float parameter after flag. Example "calc --round 4.5"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_FLOAT(ab, sn, ln, h) WSH_SHELL_OPTION_FLOAT, (ab), 1, (sn), (ln), (h)

#else /* WSH_SHELL_OPT_HELP_ENABLE */
    /**
 * @def WSH_SHELL_OPT_HELP
 * Define an option which will print command and other options description. Example "yourCmd --help".
 */
    #define WSH_SHELL_OPT_HELP()                       WSH_SHELL_OPTION_HELP, WSH_SHELL_ACCESS_ANY, 0, "-h", "--help", ""

    /**
 * @def WSH_SHELL_OPT_INTERACT
 * Define an option for running command in interactive mode. Example "yourCmd --interactive".
 */
    #define WSH_SHELL_OPT_INTERACT()                   WSH_SHELL_OPTION_INTERACT, WSH_SHELL_ACCESS_ANY, 0, "-i", "--interactive", ""

    /**
 * @def WSH_SHELL_OPT_WO_PARAM(ab, sn, ln, h)
 * @brief Define an option which can be called without any additional parameters. Example "help --all".
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_WO_PARAM(ab, sn, ln, h)      WSH_SHELL_OPTION_WO_PARAM, (ab), 0, (sn), (ln), ""

    /**
 * @def WSH_SHELL_OPT_MULTI_ARG(ab, an, sn, ln, h)
 * @brief Define an option which can be called with multiple different parameters. Example "calc --sum 5 10".
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_MULTI_ARG(ab, an, sn, ln, h) WSH_SHELL_OPTION_MULTI_ARG, (ab), (an), (sn), (ln), ""

    /**
 * @def WSH_SHELL_OPT_STR(ab, sn, ln, h)
 * @brief Define an option which expects string parameter after flag. Example "write --string string to be written"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_STR(ab, sn, ln, h)           WSH_SHELL_OPTION_STR, (ab), 1, (sn), (ln), ""

    /**
 * @def WSH_SHELL_OPT_INT(ab, sn, ln, h)
 * @brief Define an option which expects integer parameter after flag. Example "calc --root 4"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_INT(ab, sn, ln, h)           WSH_SHELL_OPTION_INT, (ab), 1, (sn), (ln), ""

    /**
 * @def WSH_SHELL_OPT_FLOAT(ab, sn, ln, h)
 * @brief Define an option which expects float parameter after flag. Example "calc --round 4.5"
 * @param[in] ab: Option access bits.
 */
    #define WSH_SHELL_OPT_FLOAT(ab, sn, ln, h)         WSH_SHELL_OPTION_FLOAT, (ab), 1, (sn), (ln), ""
#endif /* WSH_SHELL_OPT_HELP_ENABLE */

/**
 * @def WSH_SHELL_OPT_END
 * Define an option which indicates option array ending.
 */
#define WSH_SHELL_OPT_END() WSH_SHELL_OPTION_END, WSH_SHELL_ACCESS_ANY, 0, NULL, NULL, NULL

/**
 * @}
 */

/**
 * @enum WSH_SHELL_OPTION_TYPE_t
 * @brief All available option types for shell to parse.
 */
typedef enum {
    WSH_SHELL_OPTION_NO = 0,      /**< Calling a command without any options. */
    WSH_SHELL_OPTION_HELP,        /**< Option for showing command help. */
    WSH_SHELL_OPTION_INTERACT,    /**< Option for command call in interactive mode. */
    WSH_SHELL_OPTION_WO_PARAM,    /**< Calling a command with option flag only. */
    WSH_SHELL_OPTION_MULTI_ARG,   /**< Calling a command with multiple arguments. */
    WSH_SHELL_OPTION_WAITS_INPUT, /**< Calling a command with input params only. */
    WSH_SHELL_OPTION_STR,         /**< Calling a command with a string parameter. */
    WSH_SHELL_OPTION_INT,         /**< Calling a command with integer parameter. */
    WSH_SHELL_OPTION_FLOAT,       /**< Calling a command with float parameter. */
    WSH_SHELL_OPTION_END,         /**< Option that defines end of options array. */
    WSH_SHELL_OPTION_ENUM_SIZE    /**< Size of enumeration for iteration. */
} WSH_SHELL_OPTION_TYPE_t;

/**
 * @struct WshShellOption_t shell_option.h "shell_option.h"
 * @brief Structure for WshShellOption_t object representing.
 * 
 * The most common and preffered usage of option object is creating an array using X macroses.
 * For this object user code should handle dynamic and static memory allocation by itself.
 * See the "tests/test_basic.c" and *Option creation shortcuts* documentation section for more information.
 */
typedef struct {
    WshShell_Size_t ID;           /**< Option identificator */
    WSH_SHELL_OPTION_TYPE_t Type; /**< Option type. */
    WshShell_Size_t AccessBits;   /**< Access rights for option. */
    WshShell_Size_t ArgNum;       /**< Number of arguments that option requires. */
    const char* pShortName;       /**< Option short name. */
    const char* pLongName;        /**< Option long name. */
    const char* pHelp;            /**< Option description. */
} WshShellOption_t;

/**
 * @}
 */

#endif /* __WSH_SHELL_OPTIONS_H */
