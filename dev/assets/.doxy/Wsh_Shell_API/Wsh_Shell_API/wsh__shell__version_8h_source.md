

# File wsh\_shell\_version.h

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_version.h**](wsh__shell__version_8h.md)

[Go to the documentation of this file](wsh__shell__version_8h.md)


```C++
#ifndef __WSH_SHELL_VERSION_H
#define __WSH_SHELL_VERSION_H

#define WSH_SHELL_VERSION_MAJOR 2
#define WSH_SHELL_VERSION_MINOR 4

#define _WSH_SHELL_STR_HELPER(x) #x
#define _WSH_SHELL_STR(x)        _WSH_SHELL_STR_HELPER(x)
#define WSH_SHELL_VERSION_STR \
    _WSH_SHELL_STR(WSH_SHELL_VERSION_MAJOR) "." _WSH_SHELL_STR(WSH_SHELL_VERSION_MINOR)

#endif /* __WSH_SHELL_VERSION_H */
```


