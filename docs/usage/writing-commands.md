# Writing Commands

This page covers the standard pattern for defining a shell command — option table, enum, handler, and descriptor — and explains how option parsing works, including error detection via `ParseError`.

---

## Option Table (X-Macro Pattern)

Define options with an X-macro table so the enum and the `WshShellOption_t` array stay in sync automatically.

```c
/* clang-format off */
#define MY_CMD_OPT_TABLE() \
    X_CMD_ENTRY(MY_CMD_OPT_DEF,    WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_ANY, "Default action")) \
    X_CMD_ENTRY(MY_CMD_OPT_HELP,   WSH_SHELL_OPT_HELP()) \
    X_CMD_ENTRY(MY_CMD_OPT_VERBOSE, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, \
                                        "-v", "--verbose", "Enable verbose output")) \
    X_CMD_ENTRY(MY_CMD_OPT_NAME,   WSH_SHELL_OPT_STR(WSH_SHELL_OPT_ACCESS_WRITE, \
                                        "-n", "--name", "Set target name")) \
    X_CMD_ENTRY(MY_CMD_OPT_END_ID, WSH_SHELL_OPT_END())
/* clang-format on */

typedef enum {
#define X_CMD_ENTRY(en, m) en,
    MY_CMD_OPT_TABLE() MY_CMD_OPT_ENUM_SIZE
#undef X_CMD_ENTRY
} MY_CMD_OPT_t;

#define X_CMD_ENTRY(en, m) {en, m},
static const WshShellOption_t MyCmdOpts[] = {MY_CMD_OPT_TABLE()};
#undef X_CMD_ENTRY
```

Rules:

- Always start with `WSH_SHELL_OPT_NO` (the no-argument default) and end with `WSH_SHELL_OPT_END`.
- Short and long flag strings must be unique within the table — duplicates trigger `WSH_SHELL_ASSERT` on `WshShellCmd_Attach`.
- `ENUM_SIZE` sentinel at the end lets you range-check IDs if needed.

---

## Handler

```c
static WSH_SHELL_RET_STATE_t MyCmdHandler(const WshShellCmd_t* pcCmd,
                                           WshShell_Size_t argc,
                                           const WshShell_Char_t* pArgv[],
                                           void* pShellCtx) {
    if (!pcCmd || !pShellCtx || (argc > 0 && !pArgv))
        return WSH_SHELL_RET_STATE_ERR_PARAM;

    WshShell_t* pShell       = (WshShell_t*)pShellCtx;
    WshShell_Bool_t verbose  = false;
    const WshShell_Char_t* name = NULL;

    for (WshShell_Size_t tokenPos = 0; tokenPos < argc;) {
        WshShellOption_Ctx_t optCtx =
            WshShellCmd_ParseOpt(pcCmd, argc, pArgv, pShell->CurrUser->Rights, &tokenPos);

        if (!optCtx.Option) {
            if (optCtx.ParseError)          /* unknown flag encountered */
                return WSH_SHELL_RET_STATE_ERR_PARAM;
            break;                          /* end of options */
        }

        switch (optCtx.Option->ID) {
            case MY_CMD_OPT_HELP:
                WshShellCmd_PrintOptionsOverview(pcCmd);
                return WSH_SHELL_RET_STATE_SUCCESS;

            case MY_CMD_OPT_VERBOSE:
                verbose = true;
                break;

            case MY_CMD_OPT_NAME:
                /* TokenPos is the flag token; value is the next token */
                if (optCtx.TokenPos + 1 < argc)
                    name = pArgv[optCtx.TokenPos + 1];
                break;

            case MY_CMD_OPT_DEF:
            default:
                break;
        }
    }

    /* ... do the actual work using verbose / name ... */

    return WSH_SHELL_RET_STATE_SUCCESS;
}
```

---

## Command Descriptor

```c
static const WshShellCmd_t MyCmdDef = {
    .Groups  = WSH_SHELL_CMD_GROUP_ALL,
    .Name    = "mycmd",
    .Descr   = "One-line description shown in wsh -x",
    .Options = MyCmdOpts,
    .OptNum  = WSH_SHELL_ARR_LEN(MyCmdOpts),
    .Handler = MyCmdHandler,
};
```

Register it at init time:

```c
WshShellCmd_Attach(&shell, &MyCmdDef);
```

---

## ParseError — Detecting Unknown Flags

`WshShellCmd_ParseOpt` returns an `optCtx` with `.Option == NULL` in two situations:

| Situation | `optCtx.ParseError` | Meaning |
|---|---|---|
| No more tokens to process | `false` | Normal end — exit the loop with `break` |
| Unknown flag token encountered | `true` | The flag was not in the table — `[WARN]` already printed |

Checking `ParseError` is what separates **strict** handlers (reject bad flags) from **lenient** ones (ignore them). The standard pattern is strict:

```c
if (!optCtx.Option) {
    if (optCtx.ParseError)
        return WSH_SHELL_RET_STATE_ERR_PARAM;
    break;
}
```

If you intentionally want lenient behavior — for example, a parent command that ignores unknown tokens it passes down to subcommands — just omit the `ParseError` check and always `break`.

---

## Option Types Reference

| Macro | Type constant | Arguments | Example |
|---|---|---|---|
| `WSH_SHELL_OPT_NO` | `WSH_SHELL_OPTION_NO` | 0 | default action when no flag given |
| `WSH_SHELL_OPT_HELP` | `WSH_SHELL_OPTION_HELP` | 0 | `-h / --help` |
| `WSH_SHELL_OPT_INTERACT` | `WSH_SHELL_OPTION_INTERACT` | 0 | `-i / --interactive` |
| `WSH_SHELL_OPT_WO_PARAM` | `WSH_SHELL_OPTION_WO_PARAM` | 0 | flag without value (`--verbose`) |
| `WSH_SHELL_OPT_STR` | `WSH_SHELL_OPTION_STR` | 1 | flag with string value (`--name foo`) |
| `WSH_SHELL_OPT_INT` | `WSH_SHELL_OPTION_INT` | 1 | flag with integer value — decimal, `0x` hex, or `0` octal (`--count 5`, `--mask 0xFF`) |
| `WSH_SHELL_OPT_FLOAT` | `WSH_SHELL_OPTION_FLOAT` | 1 | flag with float value (`--rate 1.5`) |
| `WSH_SHELL_OPT_MULTI_ARG` | `WSH_SHELL_OPTION_MULTI_ARG` | N | flag consuming N tokens |
| `WSH_SHELL_OPT_WAITS_INPUT` | `WSH_SHELL_OPTION_WAITS_INPUT` | 0 | handler waits for subsequent input |
| `WSH_SHELL_OPT_END` | `WSH_SHELL_OPTION_END` | — | terminator (required, always last) |

For single-argument options (`STR`, `INT`, `FLOAT`), the argument token is at `pArgv[optCtx.TokenPos]` after `ParseOpt` returns and has already incremented past it.

---

## Access Control

Each option carries an access mask. `WshShellCmd_ParseOpt` silently skips options the current user lacks rights for — pass `pShell->CurrUser->Rights` as the `access` argument. See [Permission Rules](permission-rules.md) for the full bitmask reference and group-based command access.

---

## Adding Subcommands

Once a command has its own handler and options defined, it can be a parent by adding `.SubCmds` and `.SubCmdNum`:

```c
static const WshShellCmd_t* const MySubCmds[] = {
    &MySubCmdA,
    &MySubCmdB,
};

static const WshShellCmd_t MyCmdDef = {
    /* ... */
    .SubCmds   = MySubCmds,
    .SubCmdNum = WSH_SHELL_ARR_LEN(MySubCmds),
};
```

See [Subcommands](subcommands.md) for the full dispatch and autocomplete details.
