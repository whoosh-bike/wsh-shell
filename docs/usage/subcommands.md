# Subcommand Trees

Subcommands allow organizing related operations under a single parent command, similar to how tools like `git <branch>` or `aws ec2 describe-instances` structure their CLI surface. Each subcommand is a full `WshShellCmd_t` with its own options, handler, groups, and (optionally) further subcommands.

---

## Enabling Subcommands

Subcommands are gated by a config flag so projects that don't need them pay no RAM/flash cost.

```c
/* wsh_shell_cfg.h (or wsh_shell_cfg_def.h) */
#define WSH_SHELL_SUBCOMMANDS           1
#define WSH_SHELL_SUBCOMMANDS_MAX_DEPTH 4
```

- `WSH_SHELL_SUBCOMMANDS` — master switch. When `0`, the `SubCmds` / `SubCmdNum` fields of `WshShellCmd_t` are compiled out and the dispatcher treats commands as flat.
- `WSH_SHELL_SUBCOMMANDS_MAX_DEPTH` — hard cap on how deep the shell descends. Protects against accidental loops in user-supplied tables and bounds stack/iteration cost.

---

## Declaring a Subcommand Tree

Every node — root, parent, leaf — is a full `WshShellCmd_t`. Each level can declare its own options, handler, access groups, and further children. Options are **not** inherited from the parent; each node parses only its own flag table. This keeps parsing predictable and handlers focused.

Each node uses the standard X-macro option table + handler pattern — see [Writing Commands](writing-commands.md) for the full boilerplate. The only subcommand-specific addition is wiring children into the parent descriptor:

```c
/* Leaf — defined exactly like any other command (see writing-commands.md) */
static const WshShellCmd_t UserListCmd = {
    .Name    = "list",
    .Descr   = "List all registered users",
    .Groups  = WSH_SHELL_CMD_GROUP_ADMIN,
    .Options = UserListOpts,
    .OptNum  = WSH_SHELL_ARR_LEN(UserListOpts),
    .Handler = shell_cmd_user_list,
    /* no SubCmds — this is a leaf */
};

/* Parent — same structure, plus SubCmds/SubCmdNum */
static const WshShellCmd_t* const UserSubCmds[] = {
    &UserListCmd,
    &UserWhoamiCmd,
};

const WshShellCmd_t UserCmd = {
    .Name      = "user",
    .Descr     = "User table and session queries",
    .Groups    = WSH_SHELL_CMD_GROUP_ADMIN,
    .Options   = UserOpts,
    .OptNum    = WSH_SHELL_ARR_LEN(UserOpts),
    .Handler   = shell_cmd_user,
    .SubCmds   = UserSubCmds,       /* children array */
    .SubCmdNum = WSH_SHELL_ARR_LEN(UserSubCmds),
};
```

Subcommand tables are fully static — no allocation, no registration calls. The built-in `wsh user` demo in [`src/wsh_shell_cmd_def.c`](https://github.com/whoosh-bike/wsh-shell/blob/main/src/wsh_shell_cmd_def.c) is a complete working example with flags at every level (`wsh user --count`, `wsh user list --short`, `wsh user whoami --name --groups`).

---

## Dispatch Flow

Given an input line like `user whoami --verbose`:

1. The shell locates the top-level command (`user`).
2. If the next non-flag token matches a subcommand name, the dispatcher descends into it and shifts `argv` so the subcommand handler sees its own name as `argv[0]`.
3. Descent stops when the next token starts with `-` (flag) or no subcommand match is found.
4. Group-based access control is re-checked at every level. A user without the required groups on a subcommand gets a clear access-denied error instead of silently falling through.
5. The leaf command's handler is invoked.

If the user types `user bogus`, the shell prints `Unknown subcommand: bogus` and stops — the parent handler is not called with a garbage argument.

---

## Help Output

`WshShellCmd_PrintOptionsOverview` (used by `-h` / `--help`) lists subcommands after the options table. The overview is produced automatically — each parent command advertises its children for free:

```text
> user -h
User table and session queries
Subcommands:
  Name             Descr
  list             List all registered users
  whoami           Print current session user info
```

If a command has no options, the "Options overview" section is suppressed to keep output clean.

---

## Autocomplete

Tab completion walks the subcommand tree end-to-end:

- `user <TAB>` → shows `list` and `whoami`.
- `user l<TAB>` → completes to `user list`.
- `user list <TAB>` → completes the leaf command's flags.

Multi-candidate listings and longest-common-prefix expansion work identically to top-level commands.

---

## Validation

At registration time (`WshShellCmd_Attach`), the shell recursively validates the tree:

- Duplicate subcommand names within the same parent trigger `WSH_SHELL_ASSERT`.
- Each subcommand's own options are validated (duplicate short/long flags).
- NULL entries in a `SubCmds` array are reported with the parent name for easy debugging.

---

## Design Notes

- Subcommand lookup is a simple linear scan — cheap for embedded-typical N (< 10 children). No hashing, no allocation.
- The parent handler is still called when the user runs the parent with no subcommand (e.g., `user` alone). This is a convenient hook for printing an overview.
- Subcommands do **not** inherit their parent's options; each level has its own option table. This keeps parsing predictable and handlers focused.
