"""Tests for the prompt-wait hint budget (WSH_SHELL_PROMPT_WAIT_HINT_RETRIES).

A prompt-wait is modal — nothing typed reaches the command line — so the shell
explains itself with a hint. It repeats that hint only a few times and then goes
quiet, while the bell keeps answering every refused keystroke.
"""

import re
import time
from pathlib import Path
from typing import Iterator

import pytest

from wsh_shell_adapter import PtyProcessTransport

from .conftest import _EXAMPLE_BINARY, _REPO_ROOT, _skip_if_no_binary, spawn_adapter

_ANSI_RE = re.compile(r"\x1b(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
_HINT = "Press <Enter> to continue..."
_BELL = "\x07"


def _hint_retries() -> int:
    """The budget the example was built with (its config is generated from the default)."""
    cfg = (_REPO_ROOT / "src" / "wsh_shell_cfg_def.h").read_text()
    match = re.search(r"^#define\s+WSH_SHELL_PROMPT_WAIT_HINT_RETRIES\s+(\d+)", cfg, re.MULTILINE)
    assert match, "WSH_SHELL_PROMPT_WAIT_HINT_RETRIES not found in the default config"
    return int(match.group(1))


class RawDevice:
    """Direct PTY access — the adapter is prompt-driven and would answer the wait itself."""

    def __init__(self, transport: PtyProcessTransport) -> None:
        self._t = transport

    def read(self, timeout_s: float = 0.5) -> str:
        buf = b""
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            chunk = self._t.read(4096)
            if chunk:
                buf += chunk
            else:
                time.sleep(0.02)
        return buf.decode("utf-8", errors="replace")

    def send(self, data: bytes) -> None:
        self._t.write(data)
        time.sleep(0.15)

    def type_junk(self, count: int) -> str:
        """Send *count* keystrokes no prompt-wait handler accepts."""
        for _ in range(count):
            self._t.write(b"x")
            time.sleep(0.03)
        return self.read()


@pytest.fixture
def raw_device() -> Iterator[RawDevice]:
    _skip_if_no_binary()
    transport = PtyProcessTransport([str(_EXAMPLE_BINARY)], cwd=str(_REPO_ROOT))
    transport.open()
    device = RawDevice(transport)
    device.read()  # banner
    yield device
    transport.close()


class TestPromptWaitHint:
    def test_hint_stops_after_the_budget(self, raw_device: RawDevice) -> None:
        retries = _hint_retries()
        text = _ANSI_RE.sub("", raw_device.type_junk(retries + 4))
        assert text.count(_HINT) == retries

    def test_bell_answers_every_refused_key(self, raw_device: RawDevice) -> None:
        presses = _hint_retries() + 4
        text = raw_device.type_junk(presses)
        assert text.count(_BELL) == presses

    def test_enter_still_ends_a_quiet_wait(self, raw_device: RawDevice) -> None:
        raw_device.type_junk(_hint_retries() + 2)
        raw_device.send(b"\r")
        assert "Login:" in _ANSI_RE.sub("", raw_device.read())

    def test_budget_is_fresh_for_each_wait(self, raw_device: RawDevice) -> None:
        retries = _hint_retries()
        raw_device.type_junk(retries + 2)

        # Log in, then log out: the logout attaches a new wait, which must start
        # with a full hint budget rather than the exhausted one.
        for line in (b"\r", b"root\r", b"1234\r"):
            raw_device.send(line)
        raw_device.read()
        raw_device.send(b"wsh --deauth\r")
        raw_device.read()

        text = _ANSI_RE.sub("", raw_device.type_junk(retries + 2))
        assert text.count(_HINT) == retries


def test_ctrl_c_escapes_a_wait_without_a_hint(raw_device: RawDevice) -> None:
    # Ctrl+C always breaks out of a prompt-wait, and does it silently: it is not
    # a refused keystroke, so it must not spend hint budget.
    raw_device.send(b"\x03")
    assert _HINT not in _ANSI_RE.sub("", raw_device.read())

    # The shell survived the cancel and the wait is gone: Enter now reaches the
    # auth handler instead of being answered by the wait.
    raw_device.send(b"\r")
    assert "Login:" in _ANSI_RE.sub("", raw_device.read())
