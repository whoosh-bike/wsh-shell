/**
 * @file wsh_shell_io.h
 * @brief Terminal input/output definitions and interaction buffer API for WshShell.
 *
 * This header provides:
 * - Common control characters (e.g. backspace, delete, tab).
 * - ANSI escape sequences for terminal control (cursor movement, screen clearing, text style).
 * - Colorized print macros for various log levels (system/info/warning/error).
 * - PS1 and newline formatting helpers.
 * - I/O buffer structure and utility functions for handling user input.
 *
 * These utilities support consistent terminal behavior and shell interaction flow.
 *
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2024
 */

#ifndef __WSH_SHELL_IO_H
#define __WSH_SHELL_IO_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_str.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name Control characters */
///@{
#define WSH_SHELL_SYM_BACKSPACE '\b' /**< Backspace character */
#define WSH_SHELL_SYM_DELETE    0x7f /**< Delete (DEL) character */
#define WSH_SHELL_SYM_TAB       '\t' /**< Tab character */
#define WSH_SHELL_SYM_EXIT      0x04 /**< Ctrl + D (EOF signal) */
#define WSH_SHELL_SYM_SOUND     0x07 /**< Bell character (alert) */
///@}

/** @name Formatting and PS1 */
///@{
#define WSH_SHELL_CHAR_CR  '\r'   /**< Carriage return */
#define WSH_SHELL_CHAR_LF  '\n'   /**< Line feed */
#define WSH_SHELL_END_LINE "\r\n" /**< End-of-line string */
///@}

/**
 * @name Escape sequence start
 * Use `_CHAR` for character comparisons, `_STR` for formatting and printing.
 */
///@{
#define WSH_SHELL_ESC_SEQ_START_CHAR '\033' /**< Escape character (0x1B) */
#define WSH_SHELL_ESC_SEQ_START_STR  "\033" /**< Escape character as string */
///@}

/** @name Cursor movement and ANSI escape codes */
///@{
#define WSH_SHELL_ESC_SAVE_CURSOR           WSH_SHELL_ESC_SEQ_START_STR "7"
#define WSH_SHELL_ESC_RESTORE_CURSOR        WSH_SHELL_ESC_SEQ_START_STR "8"
#define WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS WSH_SHELL_ESC_SEQ_START_STR "[0K"
#define WSH_SHELL_ESC_ARROW_RIGHT           WSH_SHELL_ESC_SEQ_START_STR "[C"
#define WSH_SHELL_ESC_ARROW_LEFT            WSH_SHELL_ESC_SEQ_START_STR "[D"
#define WSH_SHELL_ESC_ARROW_UP              WSH_SHELL_ESC_SEQ_START_STR "[A"
#define WSH_SHELL_ESC_ARROW_DOWN            WSH_SHELL_ESC_SEQ_START_STR "[B"
///@}

/** @name Style reset and screen clearing */
///@{
#define WSH_SHELL_ESC_RESET_STYLE WSH_SHELL_ESC_SEQ_START_STR "[0m"
#define WSH_SHELL_ECS_CLR_SCREEN \
    WSH_SHELL_ESC_SEQ_START_STR "[1;1H" WSH_SHELL_ESC_SEQ_START_STR "[2J"
///@}

/** @name Font styles */
///@{
#define WSH_SHELL_ECS_SET_MODE_BOLD     WSH_SHELL_ESC_SEQ_START_STR "[1m"
#define WSH_SHELL_ECS_RESET_MODE_BOLD   WSH_SHELL_ESC_SEQ_START_STR "[22m"
#define WSH_SHELL_ECS_SET_MODE_ITALIC   WSH_SHELL_ESC_SEQ_START_STR "[3m"
#define WSH_SHELL_ECS_RESET_MODE_ITALIC WSH_SHELL_ESC_SEQ_START_STR "[23m"
///@}

/** @name ANSI text colors */
///@{
#define WSH_SHELL_COLOR_BLACK  WSH_SHELL_ESC_SEQ_START_STR "[30m"
#define WSH_SHELL_COLOR_RED    WSH_SHELL_ESC_SEQ_START_STR "[31m"
#define WSH_SHELL_COLOR_GREEN  WSH_SHELL_ESC_SEQ_START_STR "[32m"
#define WSH_SHELL_COLOR_YELLOW WSH_SHELL_ESC_SEQ_START_STR "[33m"
#define WSH_SHELL_COLOR_BLUE   WSH_SHELL_ESC_SEQ_START_STR "[34m"
#define WSH_SHELL_COLOR_PURPLE WSH_SHELL_ESC_SEQ_START_STR "[35m"
#define WSH_SHELL_COLOR_CYAN   WSH_SHELL_ESC_SEQ_START_STR "[36m"
#define WSH_SHELL_COLOR_WHITE  WSH_SHELL_ESC_SEQ_START_STR "[37m"
///@}

/** @name some predefined colors */
///@{
#define WSH_SHELL_COLOR_INFO  WSH_SHELL_COLOR_WHITE
#define WSH_SHELL_COLOR_SYS   WSH_SHELL_COLOR_CYAN
#define WSH_SHELL_COLOR_OK    WSH_SHELL_COLOR_GREEN
#define WSH_SHELL_COLOR_WARN  WSH_SHELL_COLOR_YELLOW
#define WSH_SHELL_COLOR_ERROR WSH_SHELL_COLOR_RED
///@}

/** @name Print macros (log levels) */
///@{
#if WSH_SHELL_PRINT_SYS_ENABLE
    #define WSH_SHELL_PRINT_SYS(_f_, ...) \
        WSH_SHELL_PRINT(WSH_SHELL_COLOR_SYS _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__)
#else
    #define WSH_SHELL_PRINT_SYS(_f_, ...)
#endif

#if WSH_SHELL_PRINT_INFO_ENABLE
    #define WSH_SHELL_PRINT_INFO(_f_, ...) \
        WSH_SHELL_PRINT(WSH_SHELL_COLOR_INFO _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__)
#else
    #define WSH_SHELL_PRINT_INFO(_f_, ...)
#endif

#if WSH_SHELL_PRINT_WARN_ENABLE
    #define WSH_SHELL_PRINT_WARN(_f_, ...) \
        WSH_SHELL_PRINT(WSH_SHELL_COLOR_WARN _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__)
#else
    #define WSH_SHELL_PRINT_WARN(_f_, ...)
#endif

#if WSH_SHELL_PRINT_ERR_ENABLE
    #define WSH_SHELL_PRINT_ERR(_f_, ...) \
        WSH_SHELL_PRINT(WSH_SHELL_COLOR_ERROR _f_ WSH_SHELL_ESC_RESET_STYLE, ##__VA_ARGS__)
#else
    #define WSH_SHELL_PRINT_ERR(_f_, ...)
#endif

///@}

/**
 * @enum WSH_SHELL_PRINT_t
 * @brief Shell print levels.
 *
 * Defines the available message levels for shell output.
 * Used to select which type of message (system, info, warning, error, ...)
 * should be printed when calling ::WSH_SHELL_PRINT_LEVEL().
 */
typedef enum {
    WSH_SHELL_PRINT_SYS,  /**< System-level messages (e.g., initialization info). */
    WSH_SHELL_PRINT_INFO, /**< Informational messages (normal operation). */
    WSH_SHELL_PRINT_WARN, /**< Warning messages about potential issues. */
    WSH_SHELL_PRINT_ERR,  /**< Error messages indicating failures. */
} WSH_SHELL_PRINT_t;

/**
 * @def WSH_SHELL_PRINT_LEVEL(_level_, _f_, ...)
 * @brief Generic macro for printing messages with a specific level.
 *
 * Each specific print macro (e.g. ::WSH_SHELL_PRINT_ERR) can be disabled
 * at compile time using corresponding flags (`WSH_SHELL_PRINT_*_ENABLE`).
 *
 * @param _level_  Print level (one of ::WSH_SHELL_PRINT_t).
 * @param _f_      Format string (compatible with `printf`).
 * @param ...      Additional arguments for the format string.
 *
 * @code
 * // Example usage:
 * WSH_SHELL_PRINT_LEVEL(WSH_SHELL_PRINT_INFO, "Initialization complete\n");
 * WSH_SHELL_PRINT_LEVEL(WSH_SHELL_PRINT_ERR, "Failed to open file: %s\n", filename);
 * @endcode
 */
#define WSH_SHELL_PRINT_LEVEL(_level_, _f_, ...)          \
    do {                                                  \
        switch (_level_) {                                \
            case WSH_SHELL_PRINT_SYS:                     \
                WSH_SHELL_PRINT_SYS(_f_, ##__VA_ARGS__);  \
                break;                                    \
            case WSH_SHELL_PRINT_INFO:                    \
                WSH_SHELL_PRINT_INFO(_f_, ##__VA_ARGS__); \
                break;                                    \
            case WSH_SHELL_PRINT_WARN:                    \
                WSH_SHELL_PRINT_WARN(_f_, ##__VA_ARGS__); \
                break;                                    \
            case WSH_SHELL_PRINT_ERR:                     \
                WSH_SHELL_PRINT_ERR(_f_, ##__VA_ARGS__);  \
                break;                                    \
            default:                                      \
                break;                                    \
        }                                                 \
    } while (0)

/**
 * @brief Structure representing the current user input interaction state.
 */
typedef struct {
    WshShell_Char_t Buff[WSH_SHELL_INTR_BUFF_LEN]; /**< Input buffer for interactive line. */
    WshShell_Size_t Len;                           /**< Current length of the buffer. */
    WshShell_Size_t CursorPos;                     /**< Current cursor position in the buffer. */
} WshShellIO_CommandLine_t;

/**
 * @brief Clears the interaction buffer.
 *
 * Resets buffer contents, cursor position, and line length.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 */
void WshShellIO_ClearInterBuff(WshShellIO_CommandLine_t* pCommandLine);

/**
 * @brief Writes a character into the interaction buffer at the cursor position.
 *
 * Increments cursor and line length accordingly.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 * @param symbol    Character to write.
 */
void WshShellIO_WriteToInterBuff(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t symbol);

/**
 * @brief Prints the current interaction buffer to the terminal.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 */
void WshShellIO_PrintInterBuff(WshShellIO_CommandLine_t* pCommandLine);

/**
 * @brief Refreshes the terminal display using the interaction buffer.
 *
 * Clears the current line and reprints buffer contents.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 */
void WshShellIO_RefreshConsoleFromInterBuff(WshShellIO_CommandLine_t* pCommandLine);

/**
 * @brief Removes the symbol to the left of the cursor in the input buffer.
 *
 * This function handles backspace-like behavior:
 * - If the cursor is not at the beginning of the line,
 *   it removes the symbol to the left of the cursor,
 *   shifts the rest of the line left,
 *   and updates both the buffer and terminal display.
 * - If the cursor is beyond the line length (unexpected),
 *   only moves the cursor one position left.
 *
 * Also updates the internal buffer length and uses ANSI escape sequences
 * to visually update the terminal.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 */
void WshShellIO_RemoveLeftSymbol(WshShellIO_CommandLine_t* pCommandLine);

/**
 * @brief Inserts a symbol at the current cursor position in the input buffer.
 *
 * This function handles character insertion into the interaction buffer:
 * - If the cursor is in the middle of the line (not at the end),
 *   it shifts existing characters to the right to make space for the new symbol.
 * - The character is then written into the buffer at the cursor position
 *   using `WshShellIO_WriteToInterBuff`.
 * - The terminal is updated to reflect the insertion, printing either the actual
 *   symbol or an asterisk (`*`) if `starsOrChars` is true (used for password masking).
 *
 * Cursor position and buffer length are updated internally.
 * Asserts that `pCommandLine` is valid and that `CursorPos >= 1` before printing.
 *
 * @param pCommandLine Pointer to the interaction buffer.
 * @param ch        The symbol to insert.
 * @param starsOrChars If true, prints `*` instead of the actual character.
 */
void WshShellIO_InsertSymbol(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t ch,
                             WshShell_Bool_t starsOrChars);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_IO_H */
