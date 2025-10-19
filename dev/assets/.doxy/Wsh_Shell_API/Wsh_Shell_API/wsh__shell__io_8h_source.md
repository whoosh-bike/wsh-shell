

# File wsh\_shell\_io.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_io.h**](wsh__shell__io_8h.md)

[Go to the documentation of this file](wsh__shell__io_8h.md)


```C++

#ifndef __WSH_SHELL_IO_H
#define __WSH_SHELL_IO_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_str.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WSH_SHELL_SYM_BACKSPACE '\b' 
#define WSH_SHELL_SYM_DELETE    0x7f 
#define WSH_SHELL_SYM_TAB       '\t' 
#define WSH_SHELL_SYM_EXIT      0x04 
#define WSH_SHELL_SYM_SOUND     0x07 

#define WSH_SHELL_CHAR_CR  '\r'   
#define WSH_SHELL_CHAR_LF  '\n'   
#define WSH_SHELL_END_LINE "\r\n" 

#define WSH_SHELL_ESC_SEQ_START_CHAR '\033' 
#define WSH_SHELL_ESC_SEQ_START_STR  "\033" 

#define WSH_SHELL_ESC_SAVE_CURSOR           WSH_SHELL_ESC_SEQ_START_STR "7"
#define WSH_SHELL_ESC_RESTORE_CURSOR        WSH_SHELL_ESC_SEQ_START_STR "8"
#define WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS WSH_SHELL_ESC_SEQ_START_STR "[0K"
#define WSH_SHELL_ESC_ARROW_RIGHT           WSH_SHELL_ESC_SEQ_START_STR "[C"
#define WSH_SHELL_ESC_ARROW_LEFT            WSH_SHELL_ESC_SEQ_START_STR "[D"
#define WSH_SHELL_ESC_ARROW_UP              WSH_SHELL_ESC_SEQ_START_STR "[A"
#define WSH_SHELL_ESC_ARROW_DOWN            WSH_SHELL_ESC_SEQ_START_STR "[B"

#define WSH_SHELL_ESC_RESET_STYLE WSH_SHELL_ESC_SEQ_START_STR "[0m"
#define WSH_SHELL_ECS_CLR_SCREEN \
    WSH_SHELL_ESC_SEQ_START_STR "[1;1H" WSH_SHELL_ESC_SEQ_START_STR "[2J"

#define WSH_SHELL_ECS_SET_MODE_BOLD     WSH_SHELL_ESC_SEQ_START_STR "[1m"
#define WSH_SHELL_ECS_RESET_MODE_BOLD   WSH_SHELL_ESC_SEQ_START_STR "[22m"
#define WSH_SHELL_ECS_SET_MODE_ITALIC   WSH_SHELL_ESC_SEQ_START_STR "[3m"
#define WSH_SHELL_ECS_RESET_MODE_ITALIC WSH_SHELL_ESC_SEQ_START_STR "[23m"

#define WSH_SHELL_COLOR_BLACK  WSH_SHELL_ESC_SEQ_START_STR "[30m"
#define WSH_SHELL_COLOR_RED    WSH_SHELL_ESC_SEQ_START_STR "[31m"
#define WSH_SHELL_COLOR_GREEN  WSH_SHELL_ESC_SEQ_START_STR "[32m"
#define WSH_SHELL_COLOR_YELLOW WSH_SHELL_ESC_SEQ_START_STR "[33m"
#define WSH_SHELL_COLOR_BLUE   WSH_SHELL_ESC_SEQ_START_STR "[34m"
#define WSH_SHELL_COLOR_PURPLE WSH_SHELL_ESC_SEQ_START_STR "[35m"
#define WSH_SHELL_COLOR_CYAN   WSH_SHELL_ESC_SEQ_START_STR "[36m"
#define WSH_SHELL_COLOR_WHITE  WSH_SHELL_ESC_SEQ_START_STR "[37m"

#define WSH_SHELL_COLOR_INFO  WSH_SHELL_COLOR_WHITE
#define WSH_SHELL_COLOR_SYS   WSH_SHELL_COLOR_CYAN
#define WSH_SHELL_COLOR_OK    WSH_SHELL_COLOR_GREEN
#define WSH_SHELL_COLOR_WARN  WSH_SHELL_COLOR_YELLOW
#define WSH_SHELL_COLOR_ERROR WSH_SHELL_COLOR_RED

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


typedef enum {
    WSH_SHELL_PRINT_SYS,  
    WSH_SHELL_PRINT_INFO, 
    WSH_SHELL_PRINT_WARN, 
    WSH_SHELL_PRINT_ERR,  
} WSH_SHELL_PRINT_t;

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

typedef struct {
    WshShell_Char_t Buff[WSH_SHELL_INTR_BUFF_LEN]; 
    WshShell_Size_t Len;                           
    WshShell_Size_t CursorPos;                     
} WshShellIO_CommandLine_t;

void WshShellIO_ClearInterBuff(WshShellIO_CommandLine_t* pCommandLine);

void WshShellIO_WriteToInterBuff(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t symbol);

void WshShellIO_PrintInterBuff(WshShellIO_CommandLine_t* pCommandLine);

void WshShellIO_RefreshConsoleFromInterBuff(WshShellIO_CommandLine_t* pCommandLine);

void WshShellIO_RemoveLeftSymbol(WshShellIO_CommandLine_t* pCommandLine);

void WshShellIO_InsertSymbol(WshShellIO_CommandLine_t* pCommandLine, WshShell_Char_t ch,
                             WshShell_Bool_t starsOrChars);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_IO_H */
```


