

# File wsh\_shell\_option.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_option.h**](wsh__shell__option_8h.md)

[Go to the documentation of this file](wsh__shell__option_8h.md)


```C++

#ifndef __WSH_SHELL_OPTIONS_H
#define __WSH_SHELL_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh_shell_types.h"

#define WSH_SHELL_OPT_ACCESS_NONE    0x00
#define WSH_SHELL_OPT_ACCESS_READ    0x01
#define WSH_SHELL_OPT_ACCESS_WRITE   0x02
#define WSH_SHELL_OPT_ACCESS_EXECUTE 0x04
#define WSH_SHELL_OPT_ACCESS_ANY     ((WshShell_Size_t)(~0U))

#if WSH_SHELL_PRINT_OPT_HELP_ENABLE
    #define WSH_SHELL_OPT_DESCR(descr) descr
#else
    #define WSH_SHELL_OPT_DESCR(descr) ""
#endif

#define WSH_SHELL_OPT_NO(acc, descr) \
    WSH_SHELL_OPTION_NO, (acc), 0, "--", "---", WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_WAITS_INPUT(acc) WSH_SHELL_OPTION_WAITS_INPUT, (acc), 0, NULL, NULL, NULL

#define WSH_SHELL_OPT_HELP()                                            \
    WSH_SHELL_OPTION_HELP, WSH_SHELL_OPT_ACCESS_ANY, 0, "-h", "--help", \
        WSH_SHELL_OPT_DESCR("Show command help information")

#define WSH_SHELL_OPT_INTERACT()                                                   \
    WSH_SHELL_OPTION_INTERACT, WSH_SHELL_OPT_ACCESS_ANY, 0, "-i", "--interactive", \
        WSH_SHELL_OPT_DESCR("Run command in interactive mode")

#define WSH_SHELL_OPT_WO_PARAM(acc, short, long, descr) \
    WSH_SHELL_OPTION_WO_PARAM, (acc), 0, (short), (long), WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_MULTI_ARG(acc, argnum, short, long, descr) \
    WSH_SHELL_OPTION_MULTI_ARG, (acc), (argnum), (short), (long), WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_STR(acc, short, long, descr) \
    WSH_SHELL_OPTION_STR, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_INT(acc, short, long, descr) \
    WSH_SHELL_OPTION_INT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_FLOAT(acc, short, long, descr) \
    WSH_SHELL_OPTION_FLOAT, (acc), 1, (short), (long), WSH_SHELL_OPT_DESCR(descr)

#define WSH_SHELL_OPT_END() WSH_SHELL_OPTION_END, WSH_SHELL_OPT_ACCESS_ANY, 0, NULL, NULL, NULL

#define WSH_SHELL_OPTION_TYPES_TABLE()                   \
    X_ENTRY(WSH_SHELL_OPTION_NO, "EMPTY")                \
    X_ENTRY(WSH_SHELL_OPTION_HELP, "HELP")               \
    X_ENTRY(WSH_SHELL_OPTION_INTERACT, "INTERACT")       \
    X_ENTRY(WSH_SHELL_OPTION_WO_PARAM, "WO_PARAM")       \
    X_ENTRY(WSH_SHELL_OPTION_MULTI_ARG, "MULTI_ARG")     \
    X_ENTRY(WSH_SHELL_OPTION_WAITS_INPUT, "WAITS_INPUT") \
    X_ENTRY(WSH_SHELL_OPTION_STR, "STR")                 \
    X_ENTRY(WSH_SHELL_OPTION_INT, "INT")                 \
    X_ENTRY(WSH_SHELL_OPTION_FLOAT, "FLOAT")             \
    X_ENTRY(WSH_SHELL_OPTION_END, "END")

typedef enum {
#define X_ENTRY(id, str) id,
    WSH_SHELL_OPTION_TYPES_TABLE() WSH_SHELL_OPTION_ENUM_SIZE
#undef X_ENTRY
} WSH_SHELL_OPTION_TYPE_t;

static const WshShell_Char_t* WshShell_OptionTypeNames[] = {
#define X_ENTRY(id, str) str,
    WSH_SHELL_OPTION_TYPES_TABLE()
#undef X_ENTRY
};

static inline const WshShell_Char_t* WshShell_OptTypeStr_Get(WSH_SHELL_OPTION_TYPE_t optType) {
    if (optType >= WSH_SHELL_OPTION_ENUM_SIZE)
        optType = WSH_SHELL_OPTION_NO;

    return WshShell_OptionTypeNames[optType];
}

typedef struct {
    WshShell_Size_t ID;               
    WSH_SHELL_OPTION_TYPE_t Type;     
    WshShell_Size_t Access;           
    WshShell_Size_t ArgNum;           
    const WshShell_Char_t* ShortName; 
    const WshShell_Char_t* LongName;  
    const WshShell_Char_t* Descr;     
} WshShellOption_t;

typedef struct {
    const WshShellOption_t* Option; 
    WshShell_Size_t TokenPos;       
} WshShellOption_Context_t;

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_OPTIONS_H */
```


