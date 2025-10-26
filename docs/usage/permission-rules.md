# Groups, Permissions and Access Rights

## Command Flags Overview

In WshShell, each command option has a set of access rights (access flags) that define what actions a user is allowed to perform with it.

> ⚠️ Access rights are assigned not to commands themselves, but to the options within commands.

Each option can be assigned one or more of the following masks:

```c
#define WSH_SHELL_OPT_ACCESS_NO      0x00 
#define WSH_SHELL_OPT_ACCESS_READ    0x01
#define WSH_SHELL_OPT_ACCESS_WRITE   0x02
#define WSH_SHELL_OPT_ACCESS_EXECUTE 0x04
#define WSH_SHELL_OPT_ACCESS_ADMIN   0x08
#define WSH_SHELL_OPT_ACCESS_ANY \
    (WSH_SHELL_OPT_ACCESS_READ | \
     WSH_SHELL_OPT_ACCESS_WRITE | \
     WSH_SHELL_OPT_ACCESS_EXECUTE)
```

| Flag                           | Bitmask | Description                                |
| ------------------------------ | ------- | ------------------------------------------ |
| `WSH_SHELL_OPT_ACCESS_NO`      | `0x00`  | No access at all                           |
| `WSH_SHELL_OPT_ACCESS_READ`    | `0x01`  | Allows reading or retrieving information   |
| `WSH_SHELL_OPT_ACCESS_WRITE`   | `0x02`  | Allows modification or reset operations    |
| `WSH_SHELL_OPT_ACCESS_EXECUTE` | `0x04`  | Allows performing actions or running logic |
| `WSH_SHELL_OPT_ACCESS_ADMIN`   | `0x08`  | Restricted to admin users                  |
| `WSH_SHELL_OPT_ACCESS_ANY`     | `0x07`  | Full read/write/execute access             |

### Flags Access Rights Usage

Each option in the shell defines its access rights via a macro entry:

```c
#define CMD_FS_OPT_TABLE() \
X_CMD_ENTRY(CMD_FS_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X_CMD_ENTRY(CMD_FS_OPT_DISK_NUM, WSH_SHELL_OPT_INT(WSH_SHELL_OPT_ACCESS_WRITE, "-d", "--disc", "Set disk root path number [0..n]")) \
X_CMD_ENTRY(CMD_FS_OPT_SCAN, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-s", "--scan", "Scan files")) \
X_CMD_ENTRY(CMD_FS_OPT_MSD, WSH_SHELL_OPT_INT(WSH_SHELL_OPT_ACCESS_EXECUTE, "-m", "--msd", "Run MSD for specified amount of minutes [1..n]")) \
X_CMD_ENTRY(CMD_FS_OPT_INFO, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ANY, "-i", "--info", "Show information about storage")) \
X_CMD_ENTRY(CMD_FS_OPT_TEST_SPEED, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_ADMIN, "-t", "--testspeed", "Fast speed test")) \
X_CMD_ENTRY(CMD_FS_OPT_END, WSH_SHELL_OPT_END())
```

Users can list all available options and their assigned access flags:

```shell
blue-pill@admin > def -h
Default command for configuring and querying the shell interface
Options overview:
  Short   Long             Type       Access Descr
  --      ---              EMPTY      rwx-   Print basic info about shell instance
  -h      --help           HELP       rwx-   Show command help information
  -i      --interactive    INTERACT   rwx-   Run command in interactive mode
  -x      --exec           WO_PARAM   r---   Get info about accessible commands
  -u      --user           WO_PARAM   r---   Get info about users
  -c      --cls            WO_PARAM   --x-   Clear screen
  -r      --histrst        WO_PARAM   -w--   Reset history storage
  -p      --histprint      WO_PARAM   r---   Print history storage
  -d      --deauth         WO_PARAM   rwx-   DeAuth and destroy history
  -s      --str            STR        --x-   Set string
  -n      --int            INT        --x-   Set int
  -f      --flt            FLOAT      ---A   Set float
```

> ⚠️ WSH_SHELL_OPT_HELP flag always has WSH_SHELL_OPT_ACCESS_ANY flag and accessible for each user

### Example

Access denied due to access rights mismatch

- User: has `WSH_SHELL_OPT_ACCESS_ANY` rights
- Command option `-f`: has `WSH_SHELL_OPT_ACCESS_ADMIN` permissions

```c
blue-pill@user > def -f 1.0
No access rights for option "-f" (---A) and user (rwx-)
Command execution: ERR_EMPTY
```

## Groups Overview

Groups define logical associations between users and commands and serve as the primary layer of access control.

Access in the shell is managed through bitmask-based command groups. Each user and each command can belong to one or more groups. When a command is executed, the shell checks if there is any intersection between the user's group set and the command's group set.

### Group Definitions

Groups should be defined as a bitmask-based enumeration in `wsh_shell_cfg.h` file. Each group represents a logical access level or subsystem scope.

```c
#define WSH_SHELL_CMD_GROUP_LIST                    
X_ENTRY(WSH_SHELL_CMD_GROUP_HARDWARE, 0x01) \  
X_ENTRY(WSH_SHELL_CMD_GROUP_TESTS, 0x02) \
X_ENTRY(WSH_SHELL_CMD_GROUP_USER, 0x04)

#define X_ENTRY(name, value) name = value,
typedef enum { WSH_SHELL_CMD_GROUP_LIST } WSH_SHELL_CMD_GROUP_t;
#undef X_ENTRY

#define WSH_SHELL_CMD_GROUP_ALL ((WshShell_Size_t)(~0U))
```

| Group Enum Name                | Bitmask  |
|--------------------------------|----------|
| `WSH_SHELL_CMD_GROUP_HARDWARE` | `0x01`   |
| `WSH_SHELL_CMD_GROUP_TESTS`    | `0x02`   |
| `WSH_SHELL_CMD_GROUP_USER`     | `0x04`   |
| `WSH_SHELL_CMD_GROUP_ALL`      | `~0U`    |

Groups can be combined using bitwise OR (`|`) to form composite access sets.

### Group Usage

Commands are assigned to one or more groups. During execution, the shell checks whether the current user’s group set intersects with the command’s group mask:

```c
const WshShellCmd_t Shell_DebugLogCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_USER,
    .Name    = "log",
    .Descr   = "Enable system log/debug output",
    .Options = DebugOptArr,
    .OptNum  = CMD_DEBUG_LOG_OPT_ENUM_SIZE,
    .Handler = shell_cmd_debug_log,
};
```

Users are associated with their respective groups, each user record includes group membership:

```c
static const WshShellUser_t Shell_UserTable[] = {
    {
        .Login  = "admin",
        .Salt   = "a0523cb065ee08c1",
        .Hash   = "0632cee0",  //1234
        .Groups = WSH_SHELL_CMD_GROUP_ALL,
        .Rights = WSH_SHELL_OPT_ACCESS_ADMIN,
    },
    {
        .Login  = "user",
        .Salt   = "aa89fee902ab12e4",
        .Hash   = "ff69cdab",  //qwer
        .Groups = WSH_SHELL_CMD_GROUP_USER,
        .Rights = WSH_SHELL_OPT_ACCESS_ANY,
    },
};
```

### Example

Access denied due to group mismatch

- User: belongs to WSH_SHELL_CMD_GROUP_USER
- Command: belongs to WSH_SHELL_CMD_GROUP_HARDWARE

```shell
blue-pill@user > led -h
Access denied: no group intersection for command "led" and user "user"!
```
