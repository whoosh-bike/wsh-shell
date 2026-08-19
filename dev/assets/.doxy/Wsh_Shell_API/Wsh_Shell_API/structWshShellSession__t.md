

# Struct WshShellSession\_t



[**ClassList**](annotated.md) **>** [**WshShellSession\_t**](structWshShellSession__t.md)



_Persisted session descriptor with an integrity hash._ [More...](#detailed-description)

* `#include <wsh_shell_session.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_U32\_t | [**Hash**](#variable-hash)  <br> |
|  WshShell\_U32\_t | [**RebootsLeft**](#variable-rebootsleft)  <br> |
|  WshShell\_U32\_t | [**UserIdx**](#variable-useridx)  <br> |












































## Detailed Description


Hash covers every field after itself, so uninitialised RAM (or a torn write) reads back as invalid. A valid descriptor with RebootsLeft &gt; 0 means an active "keep me logged in" request. 


    
## Public Attributes Documentation




### variable Hash 

```C++
WshShell_U32_t WshShellSession_t::Hash;
```



Hash over the fields below. 


        

<hr>



### variable RebootsLeft 

```C++
WshShell_U32_t WshShellSession_t::RebootsLeft;
```



Reboots the login may still survive. 


        

<hr>



### variable UserIdx 

```C++
WshShell_U32_t WshShellSession_t::UserIdx;
```



Index into the shell user table. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_session.h`

