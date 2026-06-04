# wsh_shell_adapter

Python adapter for working with serial devices over:

- serial/UART (`pyserial`)
- local process via PTY (`PtyProcessTransport`)

Two adapter classes are provided depending on the device type:

- `WshShellAdapter` — for devices running `wsh-shell`: text protocol with prompts, sync, command parsing
- `RawSerialAdapter` — for devices with binary protocols: raw byte read/write, no shell logic

Both share the same transport layer (`SerialTransport`) and port auto-discovery by VID/PID,
so all serial access goes through a single layer without importing `pyserial` directly.

## Features

- connect by fixed `port` or auto-discover by `vid/pid`
- synchronize/authenticate with shell (`Press <Enter>` -> `Login` -> `Password` -> prompt)
- optionally verify sync by `ping_command_and_response`
- execute commands with timeout and retry logic
- parse command output into structured `dict` (`json` or `key: value`)
- recover on failures with optional fallback ping
- raw binary read/write for non-shell devices (`RawSerialAdapter`)

## Quick Start

From repository root:

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install -r wsh_shell_adapter/requirements.txt
```

## WshShellAdapter

For devices running `wsh-shell` — text protocol with prompt sync, command execution, and output parsing.

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

## RawSerialAdapter

For devices that speak a binary protocol (no shell, no prompts), use `RawSerialAdapter`
instead of importing `pyserial` directly. Does not use a TOML config file — configuration
is passed directly as `AdapterConfig`.

Minimal usage:

```python
from wsh_shell_adapter import AdapterConfig, RawSerialAdapter

adapter = RawSerialAdapter(AdapterConfig(
    vid=0x0483,
    pid=0x5740,
    baudrate=115200,
))
adapter.connect()

adapter.reset_input_buffer()
adapter.write(bytes([0x5A, 0xA5, 0x00, 0x20, 0x22, 0xA0, 0x00]))

while adapter.in_waiting > 0:
    byte = adapter.read(1)

adapter.disconnect()
```

`AdapterConfig` fields used by `RawSerialAdapter`: `port`, `baudrate`, `vid`, `pid`.

Available methods and properties:

- `connect()` — open the serial port; auto-discovers by VID/PID if `port` is not set
- `disconnect()` — close the serial port
- `write(data: bytes) -> int` — write raw bytes; returns bytes written
- `read(size: int) -> bytes` — read up to `size` bytes
- `reset_input_buffer()` — discard bytes waiting in the receive buffer
- `in_waiting: int` — number of bytes available to read
- `is_open: bool` — whether the port is currently open
- `port: str | None` — active port name, or `None` if not connected
- `status() -> dict` — `{"connected": bool, "port": str | None}`

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

Detailed test notes: [../tests/README.md](../tests/README.md)
