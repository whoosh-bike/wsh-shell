# Tests

Automated tests for the Python adapter (`wsh_shell_adapter`) and for the shell
itself, driven through the example binary over a PTY.

## Layout

- `tests/unit/` — no binary required, the device is simulated in memory
    - `test_adapter.py`:
        - sync/login flow, command execution, JSON output parsing
        - fragmented transport writes, retry after timeout, empty-command validation
        - prompt detection with async log lines arriving on the same link, and with the
          prompt redrawn twice — the response must end at the *last* prompt
        - non-zero exit when `WSH_SHELL_ASSERT` fires
    - `test_config.py` — TOML config parsing (`adapter` and `local_shell` sections)
- `tests/integration/` — drives `./example/build/example` through `PtyProcessTransport`
    - `conftest.py` — `ShellSession` keyboard-level helper, `spawn_adapter()` factory,
      module-scoped `adapter` and function-scoped `session` fixtures
    - `test_smoke.py` — the binary starts, authenticates and answers
    - `test_history.py` — command history behaviour
    - `test_features.py` — hex dump, subcommand tree, enum options
    - `test_session.py` — `wsh --keep`: arming, restore across reboots, budget
      exhaustion, clearing, logout, corrupted store
    - `test_promptwait.py` — prompt-wait hint budget, per-wait reset, silent Ctrl+C escape

## Prerequisites

Run from repository root:

```bash
python3 -m venv .venv
. .venv/bin/activate
pip install -r wsh_shell_adapter/requirements.txt
pip install -r tests/requirements.txt
```

## Run tests

```bash
pytest -q tests                       # everything
pytest -q tests/unit                  # no binary needed
pytest -q tests/integration           # needs ./example/build/example
pytest -q tests -k test_retry_after_timeout
```

Build the example binary first for the integration suite:

```bash
make example
```

## Notes

- Integration tests skip automatically when the example binary is missing.
- `make example` regenerates `example/wsh_shell_cfg.h` from `src/wsh_shell_cfg_def.h`,
  so integration tests always run against the default feature set. Tests that depend
  on a config value (e.g. `WSH_SHELL_PROMPT_WAIT_HINT_RETRIES`) read it from that
  template rather than hard-coding it.
- `test_session.py` gives the example a `--session <file>` store: restarting the
  process with the same path models a device reboot with its no-init RAM intact.
- Add `-vv` to any pytest command for more verbose output.
