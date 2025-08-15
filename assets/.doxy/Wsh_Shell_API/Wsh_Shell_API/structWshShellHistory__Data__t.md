

# Struct WshShellHistory\_Data\_t



[**ClassList**](annotated.md) **>** [**WshShellHistory\_Data\_t**](structWshShellHistory__Data__t.md)



_Internal structure for storing command history buffer._ [More...](#detailed-description)

* `#include <wsh_shell_history.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  WshShell\_Size\_t | [**HeadIdx**](#variable-headidx)  <br> |
|  WshShell\_Size\_t | [**LastSavedCmdIdx**](#variable-lastsavedcmdidx)  <br> |
|  WshShell\_Bool\_t | [**LimitIsReached**](#variable-limitisreached)  <br> |
|  [**WSH\_SHELL\_HIST\_CMD\_DIR\_t**](wsh__shell__history_8h.md#enum-wsh_shell_hist_cmd_dir_t) | [**PrevDir**](#variable-prevdir)  <br> |
|  WshShell\_Char\_t | [**StorageBuff**](#variable-storagebuff)  <br> |
|  WshShell\_Size\_t | [**TailIdx**](#variable-tailidx)  <br> |












































## Detailed Description


This structure contains the command history as a raw character buffer and related metadata for navigation and storage management. 


    
## Public Attributes Documentation




### variable HeadIdx 

```C++
WshShell_Size_t WshShellHistory_Data_t::HeadIdx;
```



Index of the buffer start. 


        

<hr>



### variable LastSavedCmdIdx 

```C++
WshShell_Size_t WshShellHistory_Data_t::LastSavedCmdIdx;
```



Index of the last saved command. 


        

<hr>



### variable LimitIsReached 

```C++
WshShell_Bool_t WshShellHistory_Data_t::LimitIsReached;
```



Flag indicating buffer search boundary reached. 


        

<hr>



### variable PrevDir 

```C++
WSH_SHELL_HIST_CMD_DIR_t WshShellHistory_Data_t::PrevDir;
```



Last direction of history navigation. 


        

<hr>



### variable StorageBuff 

```C++
WshShell_Char_t WshShellHistory_Data_t::StorageBuff[WSH_SHELL_HISTORY_BUFF_SIZE];
```



Circular buffer for commands. 


        

<hr>



### variable TailIdx 

```C++
WshShell_Size_t WshShellHistory_Data_t::TailIdx;
```



Index of the buffer end. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_history.h`

