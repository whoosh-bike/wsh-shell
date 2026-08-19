# Persistent Login Session

A device that reboots in the middle of a service session — watchdog, firmware
crash, a deliberate `reset` from the shell itself — normally drops the operator
back to the login prompt. `WSH_SHELL_SESSION` lets the operator ask the shell to
keep the current login valid across a bounded number of reboots:

```text
root@device > wsh --keep 3
Login kept across 3 reboot(s); auto-logout blocked
```

After the next reboot the shell comes up already logged in as `root`, with two
reboots left in the budget.

> [!WARNING]
> A kept session is a password-free login. It is a convenience for manufacturing
> and field service, not a security feature: anyone with access to the port
> inherits the session until its budget runs out or someone logs out.

---

## How it works

The shell stores a small descriptor — an integrity hash, the remaining reboot
budget and the index of the logged-in user in the user table:

```c
typedef struct {
    WshShell_U32_t Hash;        /* over the fields below */
    WshShell_U32_t RebootsLeft;
    WshShell_U32_t UserIdx;
} WshShellSession_t;
```

The budget is a plain reboot counter, so **no RTC or wall clock is required**.
Every restore spends one reboot; when the budget reaches zero the descriptor stops
validating and the shell asks for a password again.

The hash covers every field after itself, so a descriptor that was never written —
uninitialised RAM after a cold boot, a torn write — fails validation and is
discarded instead of granting a stray login.

A session is dropped when:

- the budget is exhausted;
- the user logs out (`wsh --deauth`, or any `WshShell_DeAuth()` call);
- `wsh --keep 0` is issued;
- the descriptor fails its hash check.

---

## Storage

Like the command history, the shell stays storage-agnostic: it never touches the
backing memory directly, only the read/write handlers the integrator installs.

A **no-init RAM region** is the natural home — it survives a warm reboot and is
lost when power is actually removed, which is exactly the lifetime a "keep me
logged in across a reboot" request should have. Flash or EEPROM work too, at the
cost of write cycles and of surviving a power cycle.

```c
/* Linker-placed section that startup code must NOT zero-initialise. */
static WshShellSession_t Shell_SessionStorage __attribute__((section(".noinit")));

static WshShellSession_t Shell_SessionRead(void) {
    return Shell_SessionStorage;
}

static void Shell_SessionWrite(WshShellSession_t session) {
    memcpy((void*)&Shell_SessionStorage, (void*)&session, sizeof(WshShellSession_t));
}
```

Wire it up at start-up, **after** the user table is attached — the restore needs
it to resolve the stored user index:

```c
WshShellUser_Attach(&(Shell.Users), Shell_UserTable, WSH_SHELL_ARR_LEN(Shell_UserTable), NULL);
WshShellSession_Init(&(Shell.SessionIO), Shell_SessionRead, Shell_SessionWrite);

if (!WshShell_SessionRestore(&Shell)) {
    /* No armed session — the shell stays at the login prompt as usual. */
}
```

`WshShellSession_Init()` also normalises the store: a descriptor that does not
validate is cleared right away, so random RAM contents can never be restored.

On success `WshShell_SessionRestore()` sets the current user, regenerates the PS1
and fires the `Auth` callback — the same state a password login would produce.

---

## Blocking the inactivity auto-logout

Many integrations log the user out after some idle time. While a session is armed
the operator explicitly asked to stay logged in, so ask the shell before dropping
them:

```c
static void Shell_InactivityTick(void) {
    if (WshShell_SessionIsKeepActive(&Shell))
        return;

    if (++IdleSeconds > SHELL_IDLE_TIMEOUT_S)
        WshShell_DeAuth(&Shell, "inactivity");
}
```

`WshShell_SessionRebootsLeft()` returns the remaining budget (0 when nothing is
armed) if the application wants to show it somewhere of its own.

---

## Command reference

| Command        | Effect                                                           |
| -------------- | ---------------------------------------------------------------- |
| `wsh --keep N` | Keep this login across the next `N` reboots; blocks auto-logout  |
| `wsh -k N`     | Same, short flag                                                 |
| `wsh --keep 0` | Clear an armed session                                           |
| `wsh`          | Prints `Session keep: N reboot(s) left` while a session is armed |
| `wsh --deauth` | Logs out and clears the session                                  |

The flag requires write access (`WSH_SHELL_OPT_ACCESS_WRITE`), so read-only users
cannot arm a session.

---

## API

| Function                         | Purpose                                                |
| -------------------------------- | ------------------------------------------------------ |
| `WshShellSession_Init()`         | Install storage handlers, discard an invalid store     |
| `WshShell_SessionArm()`          | Arm (or, with 0, clear) a session for the current user |
| `WshShell_SessionRestore()`      | Restore an armed login at start-up; spends one reboot  |
| `WshShell_SessionIsKeepActive()` | Whether an armed session with remaining budget exists  |
| `WshShell_SessionRebootsLeft()`  | Remaining reboot budget, or 0                          |

With `WSH_SHELL_SESSION 0` the whole feature compiles out: every function above
keeps its signature but becomes a stub returning `false` / `0`, so integrator code
does not need its own `#if` guards.

---

## Trying it on the PC example

The PC example has no no-init RAM, so it takes an optional file that plays the same
role — restarting the process with the same path models a reboot:

```bash
./example/build/example --session /tmp/wsh-session.bin -l root -p 1234
# wsh --keep 2
# ^C, then restart without credentials:
./example/build/example --session /tmp/wsh-session.bin
```

The second run comes up logged in as `root`. Without `--session` the store is
plain process RAM and nothing survives the restart.
