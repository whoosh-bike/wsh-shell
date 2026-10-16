# Shell State Line

A host tool that talks to the shell over a serial link has to know what the
shell is doing before it writes anything. Guessing — sending Enter and watching
what comes back, then probing with `wsh --ping` — is not harmless:

- **Enter ends a prompt-wait.** A device that streams data while the shell
  waits (a module sniffer, a live log) stops the stream on the first Enter.
  A host that "just re-syncs" after a reconnect kills exactly what the
  operator asked to keep running.
- **Enter at a login prompt starts a login.** A host that reconnects to a
  shell whose session was [restored after a reboot](session.md) has nothing
  to log into; probing only adds noise.
- **`wsh --ping` needs a logged-in prompt**, so it can't tell a host anything
  until that host has already done the risky part.

With `WSH_SHELL_STATE_LINE` the shell says what state it is in, and a host
reads that instead of probing.

---

## The line

One compact JSON object on a line of its own:

```json
{"evt":"shell","state":"busy","user":"admin","busy":"sniff:cell","keepLeft":9}
```

| Field      | Value                                                                                   |
| ---------- | --------------------------------------------------------------------------------------- |
| `evt`      | Always `"shell"`. Never `cmd`, and there is no `result`: it is not a command reply.     |
| `state`    | `login`, `ready` or `busy` — see below.                                                 |
| `user`     | Logged-in user, or `null`.                                                              |
| `busy`     | While `busy`: the tag the application gave the wait (e.g. `"sniff:cell"`), or `null`.    |
| `keepLeft` | Reboots a [kept session](session.md) still survives; `0` when none is armed.            |

### States

| `state` | Meaning                                                    | What a host does                                          |
| ------- | ---------------------------------------------------------- | --------------------------------------------------------- |
| `login` | Nobody is logged in (including the "Press <Enter>" gate)   | Log in: Enter, login, password                            |
| `ready` | Logged in, prompt free                                     | Nothing — send commands. No probe needed                  |
| `busy`  | A prompt-wait holds the input; Enter or Ctrl+C would end it | Write nothing, unless the operator explicitly asks to end it |

`busy` takes precedence: a stream started before anyone logged in (a sniffer
kept across a reboot, say) is still `busy` — the untagged login gate alone is
`login`.

---

## When it is printed

By itself, **only when something a host acts on changes** — the state, the
user or the wait's tag:

- on login, and on a session restore at start-up;
- on logout (`wsh --deauth`, Ctrl+D, the inactivity timer, `WshShell_DeAuth()`);
- when a prompt-wait starts or ends.

Not at `WshShell_Init()`: right after it an application typically restores a
session or starts a wait, and a host acting on a `login` that is about to
change would only do harm. A host that missed the lines asks.

### Asking: ENQ

Send **ENQ** — byte `0x05` (Ctrl+E), the ASCII *Enquiry* — and the shell
prints the line, changed or not. ENQ is taken before anything else looks at
the input, so it is safe in every state:

| Shell state                  | ENQ                                                                   |
| ---------------------------- | --------------------------------------------------------------------- |
| Login gate / login / password | Never enters the login or password                                   |
| Prompt, command half-typed   | Never enters the line; the command still runs as typed                |
| Prompt-wait (stream, Y/N)    | Doesn't end the wait, and doesn't trigger its hint or bell            |
| Kept session, idle           | Doesn't count as activity: polling can't hold a session open forever |

A shell built without the line (or older firmware) treats `0x05` like any
other non-printable key: it beeps and ignores it, so asking is safe there too —
the host just gets no answer and falls back to the old sync.

---

## Firmware side

The line is on by default; turn it off in `wsh_shell_cfg.h`:

```c
#define WSH_SHELL_STATE_LINE 0
```

The shell tracks login and prompt-waits itself — the only thing it can't know
is *what* a wait is for. Say it when attaching the wait:

```c
WshShellPromptWait_AttachTagged(&(pShell->PromptWait), Sniff_WaitHandler, NULL, "sniff:cell");
```

The tag must outlive the wait (a string literal is fine). Plain
`WshShellPromptWait_Attach()` is the same with no tag: the host then sees
`"busy":null`.

`WshShell_StatePrint()` prints the line on demand, should an application ever
need to (ENQ calls it).

The PC example's `stream` command holds the input with a tagged wait, the way
a sniffer would:

```text
root@example > stream
{"evt":"shell","state":"busy","user":"root","busy":"stream","keepLeft":0}
Press <Enter> or Ctrl+C to stop
```

---

## Host side

The [Python adapter](python-adapter.md) does all of this in `sync()`:

1. Sends ENQ and waits up to `state_query_timeout_s` for the line.
2. `ready` → synced. Nothing else is written: no Enter, no `wsh --ping`.
3. `busy` → adapter state `busy`; nothing else is written. `execute()` then
   ends the wait with Enter before running the command — an explicit command
   outranks the stream.
4. `login` → the usual login. The shell announces `ready` once logged in,
   which completes sync — again without `wsh --ping`.
5. No answer → older firmware: the old flow, Enter and `ping_command_and_response`.

`adapter.shell_state` holds the last line seen, `adapter.query_state()` asks
on demand. A host that reads the port itself (a live terminal) feeds the text
to `ShellStateWatch` and gets each new line as it completes:

```python
from wsh_shell_adapter import ShellStateWatch

watch = ShellStateWatch()
for chunk in stream:
    state = watch.feed(adapter.clean_text(chunk))
    if state and state["state"] == "login":
        ...  # the shell logged us out
```

### `wsh --ping`

Still there, and still answered — by older hosts and by people at a terminal.
A host using the state line only needs it for firmware without the line.
