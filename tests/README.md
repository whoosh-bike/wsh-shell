# Tests

This directory contains automated tests for the Python adapter (`wsh_shell_adapter`).

## Test files

- `test_wsh_shell_adapter.py`:
  - `test_sync_and_execute_wsh` checks sync/login flow and basic command execution (`wsh`)
  - `test_execute_json_output_to_dict` checks JSON output parsing into `CommandResult.data`
  - `test_fragmented_write_command` checks command handling when transport splits outgoing writes
  - `test_retry_after_timeout` checks retry behavior after first command timeout
  - `test_raises_on_empty_command` checks validation error for empty command
  - `test_integration_with_example_binary_pty` runs integration against `./example/build/example` via PTY
- `test_wsh_shell_config.py`:
  - checks TOML config parsing (`adapter` and `local_shell` sections)
  - checks that local helper uses loaded config values
- `conftest.py`:
  - adds repository root to `sys.path` for local imports during test runs

## Prerequisites

Run from repository root:

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install -r wsh_shell_adapter/requirements.txt
pip install -r tests/requirements.txt
```

## Run tests

Run all tests (with verbose output):

```bash
pytest -q tests -vv
```

Run only unit tests (without integration):

```bash
pytest -q tests -k "not integration"
```

Run only integration test:

```bash
make src/wsh_shell_cfg.h
make example
pytest -q tests -k integration
```

Run a single test:

```bash
pytest -q tests -k test_retry_after_timeout
```

## Notes

- Integration test uses `PtyProcessTransport` and requires `./example/build/example`.
- In a clean checkout, generate `src/wsh_shell_cfg.h` before building example binary.
- If the example binary is missing, integration test is skipped automatically.
- Add `-vv` to any pytest command for more verbose output.
