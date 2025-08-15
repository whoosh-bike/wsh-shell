

# File wsh\_shell\_esc.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_esc.c**](wsh__shell__esc_8c.md)

[Go to the source code of this file](wsh__shell__esc_8c_source.md)



* `#include "wsh_shell_esc.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const [**WshShellEsc\_Action\_t**](structWshShellEsc__Action__t.md) | [**WshShellEsc\_SeqHandlers**](#variable-wshshellesc_seqhandlers)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellEsc\_Handler**](#function-wshshellesc_handler) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine, [**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage, const WshShell\_Char\_t symbol) <br>_Handles incoming escape sequence character._  |
|  WshShell\_Bool\_t | [**WshShellEsc\_IsSeqStarted**](#function-wshshellesc_isseqstarted) ([**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage) <br>_Checks if escape sequence is in progress._  |
|  void | [**WshShellEsc\_StartSeq**](#function-wshshellesc_startseq) ([**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage) <br>_Starts a new escape sequence._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**WshShellEsc\_ArrowDown**](#function-wshshellesc_arrowdown) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br> |
|  void | [**WshShellEsc\_ArrowLeft**](#function-wshshellesc_arrowleft) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br> |
|  void | [**WshShellEsc\_ArrowRight**](#function-wshshellesc_arrowright) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br> |
|  void | [**WshShellEsc\_ArrowUp**](#function-wshshellesc_arrowup) ([**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) \* pHistIO, [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) \* pCommandLine) <br> |
|  void | [**WshShellEsc\_ClearStorage**](#function-wshshellesc_clearstorage) ([**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) \* pEscStorage) <br> |


























## Public Static Attributes Documentation




### variable WshShellEsc\_SeqHandlers 

```C++
const WshShellEsc_Action_t WshShellEsc_SeqHandlers[];
```




<hr>
## Public Functions Documentation




### function WshShellEsc\_Handler 

_Handles incoming escape sequence character._ 
```C++
void WshShellEsc_Handler (
    WshShellHistory_IO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine,
    WshShellEsc_Storage_t * pEscStorage,
    const WshShell_Char_t symbol
) 
```



Matches current buffer against known ANSI sequences and triggers appropriate handler.




**Parameters:**


* `pHistIO` Pointer to shell history interface 
* `pCommandLine` Pointer to input interaction buffer 
* `pEscStorage` Pointer to escape sequence buffer 
* `symbol` Incoming character to append 




        

<hr>



### function WshShellEsc\_IsSeqStarted 

_Checks if escape sequence is in progress._ 
```C++
WshShell_Bool_t WshShellEsc_IsSeqStarted (
    WshShellEsc_Storage_t * pEscStorage
) 
```





**Parameters:**


* `pEscStorage` Pointer to escape sequence buffer 



**Returns:**

WshShell\_Bool\_t `true` if sequence has started 





        

<hr>



### function WshShellEsc\_StartSeq 

_Starts a new escape sequence._ 
```C++
void WshShellEsc_StartSeq (
    WshShellEsc_Storage_t * pEscStorage
) 
```



Initializes the escape buffer with ESC char.




**Parameters:**


* `pEscStorage` Pointer to escape sequence buffer 




        

<hr>
## Public Static Functions Documentation




### function WshShellEsc\_ArrowDown 

```C++
static void WshShellEsc_ArrowDown (
    WshShellHistory_IO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine
) 
```




<hr>



### function WshShellEsc\_ArrowLeft 

```C++
static void WshShellEsc_ArrowLeft (
    WshShellHistory_IO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine
) 
```




<hr>



### function WshShellEsc\_ArrowRight 

```C++
static void WshShellEsc_ArrowRight (
    WshShellHistory_IO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine
) 
```




<hr>



### function WshShellEsc\_ArrowUp 

```C++
static void WshShellEsc_ArrowUp (
    WshShellHistory_IO_t * pHistIO,
    WshShellIO_CommandLine_t * pCommandLine
) 
```




<hr>



### function WshShellEsc\_ClearStorage 

```C++
static void WshShellEsc_ClearStorage (
    WshShellEsc_Storage_t * pEscStorage
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_esc.c`

