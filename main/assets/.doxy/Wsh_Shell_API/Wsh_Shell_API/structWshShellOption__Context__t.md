

# Struct WshShellOption\_Context\_t



[**ClassList**](annotated.md) **>** [**WshShellOption\_Context\_t**](structWshShellOption__Context__t.md)



_Option usage context during parsing._ [More...](#detailed-description)

* `#include <wsh_shell_option.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  const [**WshShellOption\_t**](structWshShellOption__t.md) \* | [**Option**](#variable-option)  <br> |
|  WshShell\_Size\_t | [**TokenPos**](#variable-tokenpos)  <br> |












































## Detailed Description


Used internally to reference which token matched which option. 


    
## Public Attributes Documentation




### variable Option 

```C++
const WshShellOption_t* WshShellOption_Context_t::Option;
```



Pointer to matched option. 


        

<hr>



### variable TokenPos 

```C++
WshShell_Size_t WshShellOption_Context_t::TokenPos;
```



Position of the matching token in command line. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_option.h`

