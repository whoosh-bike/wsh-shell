"""Unit tests for the shell state line in the adapter (docs/usage/shell-state.md).

StatefulShellTransport answers ENQ (0x05) with a state line and prints one on
login, like a shell built with WSH_SHELL_STATE_LINE. Everything the adapter
writes is recorded, so the tests can assert what it did NOT send.
"""

import json

import pytest

from wsh_shell_adapter import (
    AdapterConfig,
    ShellStateWatch,
    WshShellAdapter,
    find_shell_states,
    parse_shell_state,
)
from wsh_shell_adapter.adapter import STATE_BUSY, STATE_SYNCED

from .test_adapter import FakeShellTransport

ENQ = b"\x05"


def state_line(state: str, user=None, busy=None, keep=0) -> str:
    return "\r\n" + json.dumps(
        {"evt": "shell", "state": state, "user": user, "busy": busy, "keepLeft": keep},
        separators=(",", ":"),
    ) + "\r\n"


class StatefulShellTransport(FakeShellTransport):
    """mode: "login" (fresh boot), "ready" (session restored), "busy" (sniffer kept)."""

    def __init__(self, mode: str = "login") -> None:
        super().__init__()
        self.mode = mode
        self.written = bytearray()
        if mode in ("ready", "busy"):
            self._state = "synced"

    def open(self) -> None:
        self._is_open = True
        if self.mode == "login":
            self._enqueue("\x07Press <Enter> to log in...\r\n")
        elif self.mode == "busy":
            self._enqueue("[sniff] AT+CSQ\r\n")

    def write(self, data: bytes) -> int:
        self.written.extend(data)
        for byte in data:
            if byte == ENQ[0]:
                self._enqueue(self._current_state())
                continue
            if self.mode == "busy":
                if byte in (ord("\r"), ord("\n")):
                    self.mode = "ready"
                    self._enqueue("Sniffer stopped\r\n" + state_line("ready", "root"))
                continue
            super().write(bytes([byte]))
        return len(data)

    def _current_state(self) -> str:
        if self.mode == "busy":
            return state_line("busy", "root", "sniff:cell", 9)
        if self._state == "synced":
            return state_line("ready", "root")
        return state_line("login")

    def _process_line(self, line: str) -> None:
        was_synced = self._state == "synced"
        super()._process_line(line)
        if not was_synced and self._state == "synced":
            # The shell announces a login by itself.
            self._enqueue(state_line("ready", "root"))


def _adapter(transport: StatefulShellTransport, **kwargs) -> WshShellAdapter:
    return WshShellAdapter(
        config=AdapterConfig(login="root", password="1234", retries=0, **kwargs),
        transport=transport,
    )


# ── Parsing ───────────────────────────────────────────────────────────────────


class TestParsing:
    def test_watch_reports_a_split_line_once(self) -> None:
        watch = ShellStateWatch()
        line = state_line("busy", "root", "sniff:cell", 9)
        assert watch.feed("noise" + line[:20]) is None
        state = watch.feed(line[20:] + "more noise\r\n")
        assert state is not None and state["busy"] == "sniff:cell"
        assert watch.feed("still streaming\r\n") is None

    def test_command_reply_is_not_a_state_line(self) -> None:
        assert find_shell_states('{"cmd":"log","sniff":"cell","result":"SUCCESS"}') == []
        assert parse_shell_state('{"evt":"shell","state":"sleeping"}') is None


# ── sync() ────────────────────────────────────────────────────────────────────


class TestSync:
    def test_ready_shell_gets_nothing_but_enq(self) -> None:
        transport = StatefulShellTransport("ready")
        adapter = _adapter(transport)
        adapter.sync()
        assert adapter.state == STATE_SYNCED
        assert bytes(transport.written) == ENQ  # no Enter, no login, no ping
        assert adapter.state_line_supported is True

    def test_busy_shell_is_left_alone(self) -> None:
        transport = StatefulShellTransport("busy")
        adapter = _adapter(transport)
        adapter.sync()
        assert adapter.state == STATE_BUSY
        assert bytes(transport.written) == ENQ
        assert transport.mode == "busy"  # the sniffer still runs
        assert adapter.shell_state["busy"] == "sniff:cell"

    def test_login_ends_on_the_announced_ready_without_ping(self) -> None:
        transport = StatefulShellTransport("login")
        adapter = _adapter(transport)
        adapter.sync()
        assert adapter.state == STATE_SYNCED
        assert b"wsh --ping" not in transport.written
        assert b"root\r" in transport.written

    def test_old_firmware_falls_back_to_ping(self) -> None:
        transport = FakeShellTransport()  # never answers ENQ
        adapter = WshShellAdapter(
            config=AdapterConfig(login="root", password="1234", retries=0, state_query_timeout_s=0.2,
                                 ping_command_and_response=("wsh --ping", "pong")),
            transport=transport,
        )
        adapter.sync()
        assert adapter.state == STATE_SYNCED
        assert adapter.state_line_supported is False

    def test_state_query_can_be_switched_off(self) -> None:
        transport = StatefulShellTransport("ready")
        adapter = _adapter(transport, state_query=False)
        adapter.sync()
        assert ENQ not in transport.written


# ── execute() ─────────────────────────────────────────────────────────────────


class TestExecuteWhileBusy:
    def test_explicit_command_ends_the_wait_first(self) -> None:
        transport = StatefulShellTransport("busy")
        adapter = _adapter(transport)
        adapter.sync()
        result = adapter.execute("wsh")
        assert result.ok and "Ver:" in result.text
        assert transport.mode == "ready"  # the operator's command stopped it
        assert adapter.state == STATE_SYNCED


@pytest.mark.parametrize("mode", ["ready", "busy"])
def test_query_state_is_side_effect_free(mode: str) -> None:
    transport = StatefulShellTransport(mode)
    adapter = _adapter(transport)
    adapter.connect()
    for _ in range(3):
        assert adapter.query_state()["state"] == mode
    assert transport.mode == mode
