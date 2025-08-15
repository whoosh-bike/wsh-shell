

# Struct WshShellHistory\_t



[**ClassList**](annotated.md) **>** [**WshShellHistory\_t**](structWshShellHistory__t.md)



_Shell history structure with integrity check._ [More...](#detailed-description)

* `#include <wsh_shell_history.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellHistory\_Data\_t**](structWshShellHistory__Data__t.md) | [**Data**](#variable-data)  <br> |
|  WshShell\_U32\_t | [**Hash**](#variable-hash)  <br> |












































## Detailed Description


This structure wraps the command history data along with a hash (CRC) for validation. 


    
## Public Attributes Documentation




### variable Data 

```C++
WshShellHistory_Data_t WshShellHistory_t::Data;
```



Command history buffer. 


        

<hr>



### variable Hash 

```C++
WshShell_U32_t WshShellHistory_t::Hash;
```



Hash for integrity verification. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_history.h`

