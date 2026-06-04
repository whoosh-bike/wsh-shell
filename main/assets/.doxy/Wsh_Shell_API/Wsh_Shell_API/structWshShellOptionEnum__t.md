

# Struct WshShellOptionEnum\_t



[**ClassList**](annotated.md) **>** [**WshShellOptionEnum\_t**](structWshShellOptionEnum__t.md)



_Describes the closed set of string values for an ENUM option._ 

* `#include <wsh_shell_option.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Size\_t | [**Count**](#variable-count)  <br> |
|  const WshShell\_Char\_t \*const  \* | [**Values**](#variable-values)  <br> |












































## Public Attributes Documentation




### variable Count 

```C++
WshShell_Size_t WshShellOptionEnum_t::Count;
```



Number of entries in Values[]. 


        

<hr>



### variable Values 

```C++
const WshShell_Char_t* const* WshShellOptionEnum_t::Values;
```



NULL-free array of allowed value strings. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_option.h`

