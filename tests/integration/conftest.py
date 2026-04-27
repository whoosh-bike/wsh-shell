"""Shared fixtures and ShellSession helper for integration tests.

Binary must be built before running:
    make example

Fixture scopes:
  adapter  — module-scoped: one PTY process per test file, synced once.
  session  — function-scoped: reset (Ctrl+C + drain) before and after each test.
"""

import re
import time
from pathlib import Path
from typing import Iterator

import pytest

from wsh_shell_adapter import AdapterConfig, PtyProcessTransport, WshShellAdapter
from wsh_shell_adapter.adapter import CommandResult

_REPO_ROOT = Path(__file__).resolve().parents[2]
_EXAMPLE_BINARY = _REPO_ROOT / "example" / "build" / "example"

_ANSI_RE = re.compile(rb"\x1b(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
_PROMPT_RE = re.compile(rb"[^\r\n]+@[^\r\n]+\s*>\s*$", re.MULTILINE)


# ── ShellSession ─────────────────────────────────────────────────────────────


class ShellSession:
    """High-level wrapper around the PTY transport for keyboard-level tests.

    run(cmd)       — execute via adapter (prompt-aware, reliable)
    type(text)     — send characters without Enter
    press(key)     — send named key sequence (up / down / left / right /
                     home / end / backspace / delete / tab / ctrl_c / ctrl_d)
    press(key, n)  — press n times
    enter()        — send \\r and read until the shell prompt; returns raw bytes
    read_raw(t)    — collect raw PTY output for t seconds
    drain(t)       — discard output for t seconds
    reset()        — send Ctrl+C + drain; ensures the shell is at a normal prompt
    """

    KEYS: dict[str, bytes] = {
        "up":        b"\x1b[A",
        "down":      b"\x1b[B",
        "right":     b"\x1b[C",
        "left":      b"\x1b[D",
        "home":      b"\x01",      # Ctrl+A  (moves to line start)
        "end":       b"\x05",      # Ctrl+E  (moves to line end)
        "delete":    b"\x1b[3~",   # forward-delete
        "backspace": b"\x7f",
        "tab":       b"\x09",
        "enter":     b"\r",
        "ctrl_c":    b"\x03",
        "ctrl_d":    b"\x04",
    }

    def __init__(self, adapter: WshShellAdapter) -> None:
        self._adapter = adapter
        self._t = adapter.transport

    # ── high-level ───────────────────────────────────────────────────────────

    def run(self, cmd: str) -> CommandResult:
        """Execute a command via the adapter and return its CommandResult."""
        return self._adapter.execute(cmd)

    # ── keyboard simulation ──────────────────────────────────────────────────

    def type(self, text: str) -> None:
        """Type characters (no Enter)."""
        self._t.write(text.encode("utf-8"))
        time.sleep(0.05)

    def press(self, key: str, count: int = 1) -> None:
        """Press a named key one or more times."""
        seq = self.KEYS[key]
        for _ in range(count):
            self._t.write(seq)
            time.sleep(0.03)

    def enter(self) -> bytes:
        """Press Enter and block until the shell prompt appears.

        Returns the raw PTY bytes (including echo and ANSI codes).
        """
        self._t.write(b"\r")
        return self._read_until_prompt(timeout_s=3.0)

    # ── raw I/O ──────────────────────────────────────────────────────────────

    def read_raw(self, timeout_s: float = 0.3) -> bytes:
        """Collect whatever the PTY produces during *timeout_s* seconds."""
        buf = b""
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            chunk = self._t.read(4096)
            if chunk:
                buf += chunk
            else:
                time.sleep(0.02)
        return buf

    def drain(self, duration_s: float = 0.15) -> None:
        """Discard all pending PTY output for *duration_s* seconds."""
        deadline = time.monotonic() + duration_s
        while time.monotonic() < deadline:
            if not self._t.read(4096):
                time.sleep(0.02)

    def reset(self) -> None:
        """Exit interactive mode (if active) and return to a normal prompt."""
        self._t.write(b"\x03")
        time.sleep(0.1)
        self.drain(0.2)

    # ── internals ────────────────────────────────────────────────────────────

    def _read_until_prompt(self, timeout_s: float) -> bytes:
        buf = b""
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            chunk = self._t.read(4096)
            if chunk:
                buf += chunk
                if _PROMPT_RE.search(_ANSI_RE.sub(b"", buf)):
                    break
            else:
                time.sleep(0.02)
        return buf


# ── Fixtures ─────────────────────────────────────────────────────────────────


def _skip_if_no_binary() -> None:
    if not _EXAMPLE_BINARY.exists():
        pytest.skip("example binary not found — run `make example` first")


@pytest.fixture(scope="module")
def adapter() -> Iterator[WshShellAdapter]:
    """Module-scoped PTY adapter: starts once, syncs once, tears down after all tests."""
    _skip_if_no_binary()
    transport = PtyProcessTransport([str(_EXAMPLE_BINARY)], cwd=str(_REPO_ROOT))
    adp = WshShellAdapter(
        config=AdapterConfig(
            login="root",
            password="1234",
            retries=0,
            sync_timeout_s=10.0,
            command_timeout_s=3.0,
            auto_recover=False,
        ),
        transport=transport,
    )
    adp.sync()
    yield adp
    adp.close()


@pytest.fixture
def session(adapter: WshShellAdapter) -> Iterator[ShellSession]:
    """Function-scoped ShellSession: resets shell state before and after each test."""
    s = ShellSession(adapter)
    s.reset()
    yield s
    s.reset()
