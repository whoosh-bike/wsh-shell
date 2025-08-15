

# Struct WshShellOption\_t



[**ClassList**](annotated.md) **>** [**WshShellOption\_t**](structWshShellOption__t.md)



_Represents a shell command-line option._ [More...](#detailed-description)

* `#include <wsh_shell_option.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Size\_t | [**Access**](#variable-access)  <br> |
|  WshShell\_Size\_t | [**ArgNum**](#variable-argnum)  <br> |
|  const WshShell\_Char\_t \* | [**Descr**](#variable-descr)  <br> |
|  WshShell\_Size\_t | [**ID**](#variable-id)  <br> |
|  const WshShell\_Char\_t \* | [**LongName**](#variable-longname)  <br> |
|  const WshShell\_Char\_t \* | [**ShortName**](#variable-shortname)  <br> |
|  [**WSH\_SHELL\_OPTION\_TYPE\_t**](wsh__shell__option_8h.md#enum-wsh_shell_option_type_t) | [**Type**](#variable-type)  <br> |












































## Detailed Description


This structure describes a single command-line option used in a shell command. It contains type, access level, required arguments, and optional short/long flags and description. 


    
## Public Attributes Documentation




### variable Access 

```C++
WshShell_Size_t WshShellOption_t::Access;
```



Access level required to use this option. 


        

<hr>



### variable ArgNum 

```C++
WshShell_Size_t WshShellOption_t::ArgNum;
```



Number of arguments expected by this option. 


        

<hr>



### variable Descr 

```C++
const WshShell_Char_t* WshShellOption_t::Descr;
```



Optional help description. 


        

<hr>



### variable ID 

```C++
WshShell_Size_t WshShellOption_t::ID;
```



Option identifier (for internal use). 


        

<hr>



### variable LongName 

```C++
const WshShell_Char_t* WshShellOption_t::LongName;
```



Long flag (e.g. `"--verbose"`) 


        

<hr>



### variable ShortName 

```C++
const WshShell_Char_t* WshShellOption_t::ShortName;
```



Short flag (e.g. `"-v"`) 


        

<hr>



### variable Type 

```C++
WSH_SHELL_OPTION_TYPE_t WshShellOption_t::Type;
```



Option type. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_option.h`

