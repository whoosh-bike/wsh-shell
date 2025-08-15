

# Struct WshShell\_t



[**ClassList**](annotated.md) **>** [**WshShell\_t**](structWshShell__t.md)



_Main shell structure containing state, configuration, user context, and subsystems._ 

* `#include <wsh_shell.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellIO\_CommandLine\_t**](structWshShellIO__CommandLine__t.md) | [**CommandLine**](#variable-commandline)  <br> |
|  [**WshShellCmd\_Table\_t**](structWshShellCmd__Table__t.md) | [**Commands**](#variable-commands)  <br> |
|  const [**WshShellUser\_t**](structWshShellUser__t.md) \* | [**CurrUser**](#variable-curruser)  <br> |
|  WshShell\_Char\_t | [**DeviceName**](#variable-devicename)  <br> |
|  [**WshShellEsc\_Storage\_t**](structWshShellEsc__Storage__t.md) | [**EscStorage**](#variable-escstorage)  <br> |
|  [**WshShell\_ExtCallbacks\_t**](structWshShell__ExtCallbacks__t.md) | [**ExtCallbacks**](#variable-extcallbacks)  <br> |
|  [**WshShellHistory\_IO\_t**](structWshShellHistory__IO__t.md) | [**HistoryIO**](#variable-historyio)  <br> |
|  [**WshShell\_Interact\_t**](structWshShell__Interact__t.md) | [**Interact**](#variable-interact)  <br> |
|  WshShell\_Char\_t | [**PS1**](#variable-ps1)  <br> |
|  WshShell\_Char\_t | [**PrevSym**](#variable-prevsym)  <br> |
|  [**WshShellPromptWait\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_t) | [**PromptWait**](#variable-promptwait)  <br> |
|  [**WshShell\_AuthContext\_t**](structWshShell__AuthContext__t.md) | [**TmpAuth**](#variable-tmpauth)  <br> |
|  [**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md) | [**Users**](#variable-users)  <br> |
|  WshShell\_Char\_t \* | [**Version**](#variable-version)  <br> |












































## Public Attributes Documentation




### variable CommandLine 

```C++
WshShellIO_CommandLine_t WshShell_t::CommandLine;
```



Terminal input/output interface. 


        

<hr>



### variable Commands 

```C++
WshShellCmd_Table_t WshShell_t::Commands;
```



Registered command table. 


        

<hr>



### variable CurrUser 

```C++
const WshShellUser_t* WshShell_t::CurrUser;
```



Currently authenticated user. 


        

<hr>



### variable DeviceName 

```C++
WshShell_Char_t WshShell_t::DeviceName[WSH_SHELL_DEV_NAME_LEN];
```



Device name (used in PS1 and more). 


        

<hr>



### variable EscStorage 

```C++
WshShellEsc_Storage_t WshShell_t::EscStorage;
```



Escape sequence state storage. 


        

<hr>



### variable ExtCallbacks 

```C++
WshShell_ExtCallbacks_t WshShell_t::ExtCallbacks;
```



Optional external auth callbacks. 


        

<hr>



### variable HistoryIO 

```C++
WshShellHistory_IO_t WshShell_t::HistoryIO;
```



Command history buffer and ops. 


        

<hr>



### variable Interact 

```C++
WshShell_Interact_t WshShell_t::Interact;
```



Interactive command interface. 


        

<hr>



### variable PS1 

```C++
WshShell_Char_t WshShell_t::PS1[WSH_SHELL_PS1_MAX_LEN];
```



Cached PS1 string. 


        

<hr>



### variable PrevSym 

```C++
WshShell_Char_t WshShell_t::PrevSym;
```



Previous symbol inserted in. 


        

<hr>



### variable PromptWait 

```C++
WshShellPromptWait_t WshShell_t::PromptWait;
```




<hr>



### variable TmpAuth 

```C++
WshShell_AuthContext_t WshShell_t::TmpAuth;
```



Temporary auth input storage. 


        

<hr>



### variable Users 

```C++
WshShellUser_Table_t WshShell_t::Users;
```



Table of available users. 


        

<hr>



### variable Version 

```C++
WshShell_Char_t* WshShell_t::Version;
```



Version string. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell.h`

