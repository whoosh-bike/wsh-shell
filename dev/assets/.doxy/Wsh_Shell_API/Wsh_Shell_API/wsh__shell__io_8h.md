

# File wsh\_shell\_io.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_io.h**](wsh__shell__io_8h.md)

[Go to the source code of this file](wsh__shell__io_8h_source.md)

_Terminal input/output definitions and interaction buffer API for WshShell._ [More...](#detailed-description)

* `#include "wsh_shell_cfg.h"`
* `#include "wsh_shell_str.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) <br>_Structure representing the current user input interaction state._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**WSH\_SHELL\_PRINT\_t**](#enum-wsh_shell_print_t)  <br>_Shell print levels._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellIO\_ClearInterBuff**](#function-wshshellio_clearinterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Clears the interaction buffer._  |
|  void | [**WshShellIO\_InsertSymbol**](#function-wshshellio_insertsymbol) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, WshShell\_Char\_t ch, WshShell\_Bool\_t starsOrChars) <br>_Inserts a symbol at the current cursor position in the input buffer._  |
|  void | [**WshShellIO\_PrintInterBuff**](#function-wshshellio_printinterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Prints the current interaction buffer to the terminal._  |
|  void | [**WshShellIO\_RefreshConsoleFromInterBuff**](#function-wshshellio_refreshconsolefrominterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Refreshes the terminal display using the interaction buffer._  |
|  void | [**WshShellIO\_RemoveLeftSymbol**](#function-wshshellio_removeleftsymbol) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Removes the symbol to the left of the cursor in the input buffer._  |
|  void | [**WshShellIO\_WriteToInterBuff**](#function-wshshellio_writetointerbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, WshShell\_Char\_t symbol) <br>_Writes a character into the interaction buffer at the cursor position._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WSH\_SHELL\_CHAR\_CR**](wsh__shell__io_8h.md#define-wsh_shell_char_cr)  `'\r'`<br> |
| define  | [**WSH\_SHELL\_CHAR\_LF**](wsh__shell__io_8h.md#define-wsh_shell_char_lf)  `'\n'`<br> |
| define  | [**WSH\_SHELL\_COLOR\_BLACK**](wsh__shell__io_8h.md#define-wsh_shell_color_black)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[30m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_BLUE**](wsh__shell__io_8h.md#define-wsh_shell_color_blue)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[34m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_CYAN**](wsh__shell__io_8h.md#define-wsh_shell_color_cyan)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[36m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_ERROR**](wsh__shell__io_8h.md#define-wsh_shell_color_error)  `WSH\_SHELL\_COLOR\_RED`<br> |
| define  | [**WSH\_SHELL\_COLOR\_GREEN**](wsh__shell__io_8h.md#define-wsh_shell_color_green)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[32m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_INFO**](wsh__shell__io_8h.md#define-wsh_shell_color_info)  `WSH\_SHELL\_COLOR\_WHITE`<br> |
| define  | [**WSH\_SHELL\_COLOR\_OK**](wsh__shell__io_8h.md#define-wsh_shell_color_ok)  `WSH\_SHELL\_COLOR\_GREEN`<br> |
| define  | [**WSH\_SHELL\_COLOR\_PURPLE**](wsh__shell__io_8h.md#define-wsh_shell_color_purple)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[35m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_RED**](wsh__shell__io_8h.md#define-wsh_shell_color_red)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[31m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_SYS**](wsh__shell__io_8h.md#define-wsh_shell_color_sys)  `WSH\_SHELL\_COLOR\_CYAN`<br> |
| define  | [**WSH\_SHELL\_COLOR\_WARN**](wsh__shell__io_8h.md#define-wsh_shell_color_warn)  `WSH\_SHELL\_COLOR\_YELLOW`<br> |
| define  | [**WSH\_SHELL\_COLOR\_WHITE**](wsh__shell__io_8h.md#define-wsh_shell_color_white)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[37m"`<br> |
| define  | [**WSH\_SHELL\_COLOR\_YELLOW**](wsh__shell__io_8h.md#define-wsh_shell_color_yellow)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[33m"`<br> |
| define  | [**WSH\_SHELL\_ECS\_CLR\_SCREEN**](wsh__shell__io_8h.md#define-wsh_shell_ecs_clr_screen)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[1;1H" WSH\_SHELL\_ESC\_SEQ\_START\_STR "[2J"`<br> |
| define  | [**WSH\_SHELL\_ECS\_RESET\_MODE\_BOLD**](wsh__shell__io_8h.md#define-wsh_shell_ecs_reset_mode_bold)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[22m"`<br> |
| define  | [**WSH\_SHELL\_ECS\_RESET\_MODE\_ITALIC**](wsh__shell__io_8h.md#define-wsh_shell_ecs_reset_mode_italic)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[23m"`<br> |
| define  | [**WSH\_SHELL\_ECS\_SET\_MODE\_BOLD**](wsh__shell__io_8h.md#define-wsh_shell_ecs_set_mode_bold)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[1m"`<br> |
| define  | [**WSH\_SHELL\_ECS\_SET\_MODE\_ITALIC**](wsh__shell__io_8h.md#define-wsh_shell_ecs_set_mode_italic)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[3m"`<br> |
| define  | [**WSH\_SHELL\_END\_LINE**](wsh__shell__io_8h.md#define-wsh_shell_end_line)  `"\r\n"`<br> |
| define  | [**WSH\_SHELL\_ESC\_ARROW\_DOWN**](wsh__shell__io_8h.md#define-wsh_shell_esc_arrow_down)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[B"`<br> |
| define  | [**WSH\_SHELL\_ESC\_ARROW\_LEFT**](wsh__shell__io_8h.md#define-wsh_shell_esc_arrow_left)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[D"`<br> |
| define  | [**WSH\_SHELL\_ESC\_ARROW\_RIGHT**](wsh__shell__io_8h.md#define-wsh_shell_esc_arrow_right)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[C"`<br> |
| define  | [**WSH\_SHELL\_ESC\_ARROW\_UP**](wsh__shell__io_8h.md#define-wsh_shell_esc_arrow_up)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[A"`<br> |
| define  | [**WSH\_SHELL\_ESC\_CLEAR\_RIGHT\_FROM\_CURS**](wsh__shell__io_8h.md#define-wsh_shell_esc_clear_right_from_curs)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[0K"`<br> |
| define  | [**WSH\_SHELL\_ESC\_RESET\_STYLE**](wsh__shell__io_8h.md#define-wsh_shell_esc_reset_style)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "[0m"`<br> |
| define  | [**WSH\_SHELL\_ESC\_RESTORE\_CURSOR**](wsh__shell__io_8h.md#define-wsh_shell_esc_restore_cursor)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "8"`<br> |
| define  | [**WSH\_SHELL\_ESC\_SAVE\_CURSOR**](wsh__shell__io_8h.md#define-wsh_shell_esc_save_cursor)  `[**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str) "7"`<br> |
| define  | [**WSH\_SHELL\_ESC\_SEQ\_START\_CHAR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_char)  `'\033'`<br> |
| define  | [**WSH\_SHELL\_ESC\_SEQ\_START\_STR**](wsh__shell__io_8h.md#define-wsh_shell_esc_seq_start_str)  `"\033"`<br> |
| define  | [**WSH\_SHELL\_PRINT\_ERR**](wsh__shell__io_8h.md#define-wsh_shell_print_err) (\_f\_, ...) <br> |
| define  | [**WSH\_SHELL\_PRINT\_INFO**](wsh__shell__io_8h.md#define-wsh_shell_print_info) (\_f\_, ...) <br> |
| define  | [**WSH\_SHELL\_PRINT\_LEVEL**](wsh__shell__io_8h.md#define-wsh_shell_print_level) (\_level\_, \_f\_, ...) `/* multi line expression */`<br>_Generic macro for printing messages with a specific level._  |
| define  | [**WSH\_SHELL\_PRINT\_SYS**](wsh__shell__io_8h.md#define-wsh_shell_print_sys) (\_f\_, ...) <br> |
| define  | [**WSH\_SHELL\_PRINT\_WARN**](wsh__shell__io_8h.md#define-wsh_shell_print_warn) (\_f\_, ...) <br> |
| define  | [**WSH\_SHELL\_SYM\_BACKSPACE**](wsh__shell__io_8h.md#define-wsh_shell_sym_backspace)  `'\b'`<br> |
| define  | [**WSH\_SHELL\_SYM\_DELETE**](wsh__shell__io_8h.md#define-wsh_shell_sym_delete)  `0x7f`<br> |
| define  | [**WSH\_SHELL\_SYM\_EXIT**](wsh__shell__io_8h.md#define-wsh_shell_sym_exit)  `0x04`<br> |
| define  | [**WSH\_SHELL\_SYM\_SOUND**](wsh__shell__io_8h.md#define-wsh_shell_sym_sound)  `0x07`<br> |
| define  | [**WSH\_SHELL\_SYM\_TAB**](wsh__shell__io_8h.md#define-wsh_shell_sym_tab)  `'\t'`<br> |

## Detailed Description


This header provides:
* Common control characters (e.g. backspace, delete, tab).
* ANSI escape sequences for terminal control (cursor movement, screen clearing, text style).
* Colorized print macros for various log levels (system/info/warning/error).
* PS1 and newline formatting helpers.
* I/O buffer structure and utility functions for handling user input.




These utilities support consistent terminal behavior and shell interaction flow.




**Author:**

Whoosh Embedded Team 




**Copyright:**

Copyright (c) 2024 





    
## Public Types Documentation




### enum WSH\_SHELL\_PRINT\_t 

_Shell print levels._ 
```C++
enum WSH_SHELL_PRINT_t {
    WSH_SHELL_PRINT_SYS,
    WSH_SHELL_PRINT_INFO,
    WSH_SHELL_PRINT_WARN,
    WSH_SHELL_PRINT_ERR
};
```



Defines the available message levels for shell output. Used to select which type of message (system, info, warning, error, ...) should be printed when calling [**WSH\_SHELL\_PRINT\_LEVEL()**](wsh__shell__io_8h.md#define-wsh_shell_print_level). 


        

<hr>
## Public Functions Documentation




### function WshShellIO\_ClearInterBuff 

_Clears the interaction buffer._ 
```C++
void WshShellIO_ClearInterBuff (
    WshShellIO_CommandLine_t * pCommandLine
) 
```



Resets buffer contents, cursor position, and line length.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 




        

<hr>



### function WshShellIO\_InsertSymbol 

_Inserts a symbol at the current cursor position in the input buffer._ 
```C++
void WshShellIO_InsertSymbol (
    WshShellIO_CommandLine_t * pCommandLine,
    WshShell_Char_t ch,
    WshShell_Bool_t starsOrChars
) 
```



This function handles character insertion into the interaction buffer:
* If the cursor is in the middle of the line (not at the end), it shifts existing characters to the right to make space for the new symbol.
* The character is then written into the buffer at the cursor position using `WshShellIO_WriteToInterBuff`.
* The terminal is updated to reflect the insertion, printing either the actual symbol or an asterisk (`*`) if `starsOrChars` is true (used for password masking).




Cursor position and buffer length are updated internally. Asserts that `pCommandLine` is valid and that `CursorPos >= 1` before printing.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 
* `ch` The symbol to insert. 
* `starsOrChars` If true, prints `*` instead of the actual character. 




        

<hr>



### function WshShellIO\_PrintInterBuff 

_Prints the current interaction buffer to the terminal._ 
```C++
void WshShellIO_PrintInterBuff (
    WshShellIO_CommandLine_t * pCommandLine
) 
```





**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 




        

<hr>



### function WshShellIO\_RefreshConsoleFromInterBuff 

_Refreshes the terminal display using the interaction buffer._ 
```C++
void WshShellIO_RefreshConsoleFromInterBuff (
    WshShellIO_CommandLine_t * pCommandLine
) 
```



Clears the current line and reprints buffer contents.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 




        

<hr>



### function WshShellIO\_RemoveLeftSymbol 

_Removes the symbol to the left of the cursor in the input buffer._ 
```C++
void WshShellIO_RemoveLeftSymbol (
    WshShellIO_CommandLine_t * pCommandLine
) 
```



This function handles backspace-like behavior:
* If the cursor is not at the beginning of the line, it removes the symbol to the left of the cursor, shifts the rest of the line left, and updates both the buffer and terminal display.
* If the cursor is beyond the line length (unexpected), only moves the cursor one position left.




Also updates the internal buffer length and uses ANSI escape sequences to visually update the terminal.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 




        

<hr>



### function WshShellIO\_WriteToInterBuff 

_Writes a character into the interaction buffer at the cursor position._ 
```C++
void WshShellIO_WriteToInterBuff (
    WshShellIO_CommandLine_t * pCommandLine,
    WshShell_Char_t symbol
) 
```



Increments cursor and line length accordingly.




**Parameters:**


* `pCommandLine` Pointer to the interaction buffer. 
* `symbol` Character to write. 




        

<hr>
## Macro Definition Documentation





### define WSH\_SHELL\_CHAR\_CR 

```C++
#define WSH_SHELL_CHAR_CR `'\r'`
```



Carriage return 


        

<hr>



### define WSH\_SHELL\_CHAR\_LF 

```C++
#define WSH_SHELL_CHAR_LF `'\n'`
```



Line feed 


        

<hr>



### define WSH\_SHELL\_COLOR\_BLACK 

```C++
#define WSH_SHELL_COLOR_BLACK `WSH_SHELL_ESC_SEQ_START_STR "[30m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_BLUE 

```C++
#define WSH_SHELL_COLOR_BLUE `WSH_SHELL_ESC_SEQ_START_STR "[34m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_CYAN 

```C++
#define WSH_SHELL_COLOR_CYAN `WSH_SHELL_ESC_SEQ_START_STR "[36m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_ERROR 

```C++
#define WSH_SHELL_COLOR_ERROR `WSH_SHELL_COLOR_RED`
```




<hr>



### define WSH\_SHELL\_COLOR\_GREEN 

```C++
#define WSH_SHELL_COLOR_GREEN `WSH_SHELL_ESC_SEQ_START_STR "[32m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_INFO 

```C++
#define WSH_SHELL_COLOR_INFO `WSH_SHELL_COLOR_WHITE`
```




<hr>



### define WSH\_SHELL\_COLOR\_OK 

```C++
#define WSH_SHELL_COLOR_OK `WSH_SHELL_COLOR_GREEN`
```




<hr>



### define WSH\_SHELL\_COLOR\_PURPLE 

```C++
#define WSH_SHELL_COLOR_PURPLE `WSH_SHELL_ESC_SEQ_START_STR "[35m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_RED 

```C++
#define WSH_SHELL_COLOR_RED `WSH_SHELL_ESC_SEQ_START_STR "[31m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_SYS 

```C++
#define WSH_SHELL_COLOR_SYS `WSH_SHELL_COLOR_CYAN`
```




<hr>



### define WSH\_SHELL\_COLOR\_WARN 

```C++
#define WSH_SHELL_COLOR_WARN `WSH_SHELL_COLOR_YELLOW`
```




<hr>



### define WSH\_SHELL\_COLOR\_WHITE 

```C++
#define WSH_SHELL_COLOR_WHITE `WSH_SHELL_ESC_SEQ_START_STR "[37m"`
```




<hr>



### define WSH\_SHELL\_COLOR\_YELLOW 

```C++
#define WSH_SHELL_COLOR_YELLOW `WSH_SHELL_ESC_SEQ_START_STR "[33m"`
```




<hr>



### define WSH\_SHELL\_ECS\_CLR\_SCREEN 

```C++
#define WSH_SHELL_ECS_CLR_SCREEN `WSH_SHELL_ESC_SEQ_START_STR "[1;1H" WSH_SHELL_ESC_SEQ_START_STR "[2J"`
```




<hr>



### define WSH\_SHELL\_ECS\_RESET\_MODE\_BOLD 

```C++
#define WSH_SHELL_ECS_RESET_MODE_BOLD `WSH_SHELL_ESC_SEQ_START_STR "[22m"`
```




<hr>



### define WSH\_SHELL\_ECS\_RESET\_MODE\_ITALIC 

```C++
#define WSH_SHELL_ECS_RESET_MODE_ITALIC `WSH_SHELL_ESC_SEQ_START_STR "[23m"`
```




<hr>



### define WSH\_SHELL\_ECS\_SET\_MODE\_BOLD 

```C++
#define WSH_SHELL_ECS_SET_MODE_BOLD `WSH_SHELL_ESC_SEQ_START_STR "[1m"`
```




<hr>



### define WSH\_SHELL\_ECS\_SET\_MODE\_ITALIC 

```C++
#define WSH_SHELL_ECS_SET_MODE_ITALIC `WSH_SHELL_ESC_SEQ_START_STR "[3m"`
```




<hr>



### define WSH\_SHELL\_END\_LINE 

```C++
#define WSH_SHELL_END_LINE `"\r\n"`
```



End-of-line string 


        

<hr>



### define WSH\_SHELL\_ESC\_ARROW\_DOWN 

```C++
#define WSH_SHELL_ESC_ARROW_DOWN `WSH_SHELL_ESC_SEQ_START_STR "[B"`
```




<hr>



### define WSH\_SHELL\_ESC\_ARROW\_LEFT 

```C++
#define WSH_SHELL_ESC_ARROW_LEFT `WSH_SHELL_ESC_SEQ_START_STR "[D"`
```




<hr>



### define WSH\_SHELL\_ESC\_ARROW\_RIGHT 

```C++
#define WSH_SHELL_ESC_ARROW_RIGHT `WSH_SHELL_ESC_SEQ_START_STR "[C"`
```




<hr>



### define WSH\_SHELL\_ESC\_ARROW\_UP 

```C++
#define WSH_SHELL_ESC_ARROW_UP `WSH_SHELL_ESC_SEQ_START_STR "[A"`
```




<hr>



### define WSH\_SHELL\_ESC\_CLEAR\_RIGHT\_FROM\_CURS 

```C++
#define WSH_SHELL_ESC_CLEAR_RIGHT_FROM_CURS `WSH_SHELL_ESC_SEQ_START_STR "[0K"`
```




<hr>



### define WSH\_SHELL\_ESC\_RESET\_STYLE 

```C++
#define WSH_SHELL_ESC_RESET_STYLE `WSH_SHELL_ESC_SEQ_START_STR "[0m"`
```




<hr>



### define WSH\_SHELL\_ESC\_RESTORE\_CURSOR 

```C++
#define WSH_SHELL_ESC_RESTORE_CURSOR `WSH_SHELL_ESC_SEQ_START_STR "8"`
```




<hr>



### define WSH\_SHELL\_ESC\_SAVE\_CURSOR 

```C++
#define WSH_SHELL_ESC_SAVE_CURSOR `WSH_SHELL_ESC_SEQ_START_STR "7"`
```




<hr>



### define WSH\_SHELL\_ESC\_SEQ\_START\_CHAR 

```C++
#define WSH_SHELL_ESC_SEQ_START_CHAR `'\033'`
```



Escape character (0x1B) 


        

<hr>



### define WSH\_SHELL\_ESC\_SEQ\_START\_STR 

```C++
#define WSH_SHELL_ESC_SEQ_START_STR `"\033"`
```



Escape character as string 


        

<hr>



### define WSH\_SHELL\_PRINT\_ERR 

```C++
#define WSH_SHELL_PRINT_ERR (
    _f_,
    ...
) 
```




<hr>



### define WSH\_SHELL\_PRINT\_INFO 

```C++
#define WSH_SHELL_PRINT_INFO (
    _f_,
    ...
) 
```




<hr>



### define WSH\_SHELL\_PRINT\_LEVEL 

_Generic macro for printing messages with a specific level._ 
```C++
#define WSH_SHELL_PRINT_LEVEL (
    _level_,
    _f_,
    ...
) `/* multi line expression */`
```



Each specific print macro (e.g. WSH\_SHELL\_PRINT\_ERR) can be disabled at compile time using corresponding flags (`WSH_SHELL_PRINT_*_ENABLE`).




**Parameters:**


* `<em>level</em>` Print level (one of [**WSH\_SHELL\_PRINT\_t**](wsh__shell__io_8h.md#enum-wsh_shell_print_t)). 
* `<em>f</em>` Format string (compatible with `printf`). 
* `...` Additional arguments for the format string.


```C++
// Example usage:
WSH_SHELL_PRINT_LEVEL(WSH_SHELL_PRINT_INFO, "Initialization complete\n");
WSH_SHELL_PRINT_LEVEL(WSH_SHELL_PRINT_ERR, "Failed to open file: %s\n", filename);
```
 


        

<hr>



### define WSH\_SHELL\_PRINT\_SYS 

```C++
#define WSH_SHELL_PRINT_SYS (
    _f_,
    ...
) 
```




<hr>



### define WSH\_SHELL\_PRINT\_WARN 

```C++
#define WSH_SHELL_PRINT_WARN (
    _f_,
    ...
) 
```




<hr>



### define WSH\_SHELL\_SYM\_BACKSPACE 

```C++
#define WSH_SHELL_SYM_BACKSPACE `'\b'`
```



Backspace character 


        

<hr>



### define WSH\_SHELL\_SYM\_DELETE 

```C++
#define WSH_SHELL_SYM_DELETE `0x7f`
```



Delete (DEL) character 


        

<hr>



### define WSH\_SHELL\_SYM\_EXIT 

```C++
#define WSH_SHELL_SYM_EXIT `0x04`
```



Ctrl + D (EOF signal) 


        

<hr>



### define WSH\_SHELL\_SYM\_SOUND 

```C++
#define WSH_SHELL_SYM_SOUND `0x07`
```



Bell character (alert) 


        

<hr>



### define WSH\_SHELL\_SYM\_TAB 

```C++
#define WSH_SHELL_SYM_TAB `'\t'`
```



Tab character 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_io.h`

