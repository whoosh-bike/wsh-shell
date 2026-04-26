# Python Adapter

Python adapter for working with `wsh-shell` over:

- serial/UART (`pyserial`)
- local process via PTY (`PtyProcessTransport`)

## Features

- connect by fixed `port` or auto-discover by `vid/pid`
- synchronize/authenticate with shell (`Press <Enter>` -> `Login` -> `Password` -> prompt)
- optionally verify sync by `ping_command_and_response`
- execute commands with timeout and retry logic
- parse command output into structured `dict` (`json` or `key: value`)
- recover on failures with optional fallback ping

## Quick Start

From repository root:

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install -r wsh_shell_adapter/requirements.txt
```

Minimal usage:

```python
from wsh_shell_adapter import AdapterConfig, WshShellAdapter

cfg = AdapterConfig(
    vid=0x0483,
    pid=0x5740,
    baudrate=115200,
    login="root",
    password="1234",
)

adapter = WshShellAdapter(config=cfg)
adapter.sync()
result = adapter.execute("wsh")
print(result.text)
print(result.data)
adapter.close()
```

## Config File (TOML)

Default path: `wsh_shell_adapter/config.toml`

`[adapter]` maps to `AdapterConfig` fields.  
`[local_shell]` is used by `run_local_shell.py`.

Example:

```toml
[adapter]
port = ""
vid = 0x0483
pid = 0x5740
baudrate = 115200
login = "root"
password = "1234"
newline = "\r"
command_timeout_s = 3.0
retries = 1
auto_recover = true
ping_command_and_response = ["wsh --ping", "pong"]
prompt_regex = "^(?:[^\\r\\n]+@[^\\r\\n]+\\s*>|wsh-shell>)\\s*$"

[local_shell]
binary = "example/build/example"
cwd = "."
```

If `ping_command_and_response` is set, `sync()` sends the command after prompt detection and
expects the configured response as the full cleaned output or as one full output line before marking
the adapter as synced.
If `ping_command_and_response` is missing or `[]`, `sync()` sends a fixed probe command and treats
any non-empty command output as proof that the shell is alive.
`auto_recover` controls whether the adapter tries soft/hard recovery after command failures.

Load config directly:

```python
from wsh_shell_adapter import load_adapter_config_from_toml

cfg = load_adapter_config_from_toml("wsh_shell_adapter/config.toml")
```

Resolve port selection in application code without touching `pyserial` directly:

```python
from wsh_shell_adapter import (
    format_serial_port,
    list_serial_ports,
    prompt_for_serial_port,
    resolve_serial_port,
)

ports = list_serial_ports()
for item in ports:
    print(format_serial_port(item))

selected_port = resolve_serial_port(ports=ports, chooser=prompt_for_serial_port)
```

## Helper Scripts

### Local PTY helper

```bash
python -m wsh_shell_adapter.run_local_shell
```

Before first run in a clean checkout, build example binary:

```bash
make src/wsh_shell_cfg.h
make example
```

### USB helper

```bash
python -m wsh_shell_adapter.run_usb_shell
```

It loads `[adapter]` settings from `wsh_shell_adapter/config.toml` and starts interactive REPL.

## Tests

Install test dependencies:

```bash
pip install -r tests/requirements.txt
```

Run all adapter tests:

```bash
pytest -q tests -vv
```
