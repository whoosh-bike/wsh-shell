

# File wsh\_shell\_io.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_io.c**](wsh__shell__io_8c.md)

[Go to the source code of this file](wsh__shell__io_8c_source.md)



* `#include "wsh_shell_io.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const WshShell\_Char\_t \* | [**WshShellIO\_InsertStringTemplate**](#variable-wshshellio_insertstringtemplate)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellIO\_ClearInterBuff**](#function-wshshellio_clearinterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Clears the interaction buffer._  |
|  void | [**WshShellIO\_InsertSymbol**](#function-wshshellio_insertsymbol) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, WshShell\_Char\_t ch, WshShell\_Bool\_t starsOrChars) <br>_Inserts a symbol at the current cursor position in the input buffer._  |
|  void | [**WshShellIO\_PrintInterBuff**](#function-wshshellio_printinterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Prints the current interaction buffer to the terminal._  |
|  void | [**WshShellIO\_RefreshConsoleFromInterBuff**](#function-wshshellio_refreshconsolefrominterbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Refreshes the terminal display using the interaction buffer._  |
|  void | [**WshShellIO\_RemoveLeftSymbol**](#function-wshshellio_removeleftsymbol) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br>_Removes the symbol to the left of the cursor in the input buffer._  |
|  void | [**WshShellIO\_WriteToInterBuff**](#function-wshshellio_writetointerbuff) ([**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, WshShell\_Char\_t symbol) <br>_Writes a character into the interaction buffer at the cursor position._  |




























## Public Static Attributes Documentation




### variable WshShellIO\_InsertStringTemplate 

```C++
const WshShell_Char_t* WshShellIO_InsertStringTemplate;
```




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

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_io.c`

