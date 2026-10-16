

# Struct WshShellPromptWait



[**ClassList**](annotated.md) **>** [**WshShellPromptWait**](structWshShellPromptWait.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**Ctx**](#variable-ctx)  <br> |
|  [**WshShellPromptWait\_Handler\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_handler_t) | [**Handler**](#variable-handler)  <br> |
|  [**WshShellPromptWait\_Notify\_t**](wsh__shell__promptwait_8h.md#typedef-wshshellpromptwait_notify_t) | [**Notify**](#variable-notify)  <br> |
|  void \* | [**NotifyOwner**](#variable-notifyowner)  <br> |
|  WshShell\_Size\_t | [**RejectedNum**](#variable-rejectednum)  <br> |
|  const WshShell\_Char\_t \* | [**Tag**](#variable-tag)  <br> |












































## Public Attributes Documentation




### variable Ctx 

```C++
void* WshShellPromptWait::Ctx;
```




<hr>



### variable Handler 

```C++
WshShellPromptWait_Handler_t WshShellPromptWait::Handler;
```




<hr>



### variable Notify 

```C++
WshShellPromptWait_Notify_t WshShellPromptWait::Notify;
```



Set by the shell, survives Flush/Attach. 


        

<hr>



### variable NotifyOwner 

```C++
void* WshShellPromptWait::NotifyOwner;
```




<hr>



### variable RejectedNum 

```C++
WshShell_Size_t WshShellPromptWait::RejectedNum;
```



Keystrokes refused by the handler since attach. 


        

<hr>



### variable Tag 

```C++
const WshShell_Char_t* WshShellPromptWait::Tag;
```



What the shell is busy with, e.g. "sniff:cell"; NULL if not said. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/wsh_shell_promptwait.h`

