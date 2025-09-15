

# Struct WshShellUser\_Table\_t



[**ClassList**](annotated.md) **>** [**WshShellUser\_Table\_t**](structWshShellUser__Table__t.md)



_Table of registered shell users._ [More...](#detailed-description)

* `#include <wsh_shell_user.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**WshShellUser\_HashFunc\_t**](wsh__shell__user_8h.md#typedef-wshshelluser_hashfunc_t) | [**Hash**](#variable-hash)  <br> |
|  const [**WshShellUser\_t**](structWshShellUser__t.md) \* | [**List**](#variable-list)  <br> |
|  WshShell\_Size\_t | [**Num**](#variable-num)  <br> |












































## Detailed Description


This structure contains a list of shell users and total count. 


    
## Public Attributes Documentation




### variable Hash 

```C++
WshShellUser_HashFunc_t WshShellUser_Table_t::Hash;
```



Hash func 


        

<hr>



### variable List 

```C++
const WshShellUser_t* WshShellUser_Table_t::List;
```



Pointer to an array of user objects. 


        

<hr>



### variable Num 

```C++
WshShell_Size_t WshShellUser_Table_t::Num;
```



Number of users in the list. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_user.h`

