"""Tests for the shell state line (WSH_SHELL_STATE_LINE, docs/usage/shell-state.md).

The shell reports {"evt":"shell","state":"login|ready|busy",...} by itself on
every change and in answer to ENQ (0x05). Driven over a raw PTY: the adapter
would answer prompts on its own and hide what the device actually printed.
"""

import json
import re
import time
from pathlib import Path
from typing import Iterator, Optional

import pytest

from wsh_shell_adapter import PtyProcessTransport

from .conftest import _EXAMPLE_BINARY, _REPO_ROOT, _skip_if_no_binary

_ANSI_RE = re.compile(r"\x1b(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
_STATE_RE = re.compile(r'\{"evt":"shell"[^\r\n{}]*\}')
ENQ = b"\x05"


class RawDevice:
    def __init__(self, args: Optional[list[str]] = None) -> None:
        _skip_if_no_binary()
        self._t = PtyProcessTransport([str(_EXAMPLE_BINARY), *(args or [])], cwd=str(_REPO_ROOT))
        self._t.open()

    def read(self, timeout_s: float = 0.4) -> str:
        buf = b""
        deadline = time.monotonic() + timeout_s
        while time.monotonic() < deadline:
            chunk = self._t.read(4096)
            if chunk:
                buf += chunk
            else:
                time.sleep(0.02)
        return _ANSI_RE.sub("", buf.decode("utf-8", errors="replace"))

    def send(self, data: bytes, settle_s: float = 0.4) -> str:
        self._t.write(data)
        return self.read(settle_s)

    def login(self) -> str:
        self.send(b"\r")
        self.send(b"root\r")
        return self.send(b"1234\r")

    def close(self) -> None:
        self._t.close()


def states(text: str) -> list[dict]:
    return [json.loads(m.group(0)) for m in _STATE_RE.finditer(text)]


@pytest.fixture
def device() -> Iterator[RawDevice]:
    dev = RawDevice()
    dev.read()  # banner
    yield dev
    dev.close()


class TestStateLine:
    def test_boot_prints_no_line_enq_answers_login(self, device: RawDevice) -> None:
        # Nothing on its own at boot: the application may still restore a session.
        assert states(device.read(0.2)) == []
        [state] = states(device.send(ENQ))
        assert state == {"evt": "shell", "state": "login", "user": None, "busy": None, "keepLeft": 0}

    def test_enq_never_reaches_the_login_prompt(self, device: RawDevice) -> None:
        device.send(b"\r")
        device.send(b"ro")
        device.send(ENQ)
        out = device.send(b"ot\r") + device.send(b"1234\r")
        assert [s["state"] for s in states(out)] == ["ready"]

    def test_login_and_logout_are_announced(self, device: RawDevice) -> None:
        [ready] = states(device.login())
        assert ready["state"] == "ready" and ready["user"] == "root"
        [login] = states(device.send(b"\x04"))  # Ctrl+D logs out
        assert login["state"] == "login" and login["user"] is None

    def test_enq_keeps_a_half_typed_command(self, device: RawDevice) -> None:
        device.login()
        device.send(b"du")
        assert states(device.send(ENQ))[0]["state"] == "ready"
        out = device.send(b"mp\r")
        assert "not found" not in out.lower()
        assert "0x" in out or "00000000" in out  # the hex dump ran

    def test_stream_is_busy_and_survives_enq(self, device: RawDevice) -> None:
        device.login()
        [busy] = states(device.send(b"stream\r"))
        assert busy["state"] == "busy" and busy["busy"] == "stream" and busy["user"] == "root"

        out = device.send(ENQ)
        assert states(out)[0]["busy"] == "stream"
        assert "Stream stopped" not in out and "\x07" not in out  # no stop, no hint, no bell

        out = device.send(b"\r")
        assert "Stream stopped" in out
        assert states(out)[-1]["state"] == "ready"

    def test_repeated_enq_always_answers(self, device: RawDevice) -> None:
        device.login()
        out = device.send(ENQ) + device.send(ENQ)
        assert [s["state"] for s in states(out)] == ["ready", "ready"]


class TestStateLineSession:
    def test_restored_session_reports_ready_with_reboots_left(self, tmp_path: Path) -> None:
        store = tmp_path / "session.bin"
        dev = RawDevice(["--session", str(store)])
        try:
            dev.read()
            dev.login()
            dev.send(b"wsh --keep 3\r")
        finally:
            dev.close()

        dev = RawDevice(["--session", str(store)])
        try:
            boot = dev.read()
            # The restore is announced at boot, already counting this reboot.
            assert states(boot)[-1] == {
                "evt": "shell", "state": "ready", "user": "root", "busy": None, "keepLeft": 2,
            }
            assert states(dev.send(ENQ))[0]["state"] == "ready"
        finally:
            dev.close()
