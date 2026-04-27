"""Tests for v3.2 features: hex dump, subcommands, enum options.

Note: Ctrl+C behaviour (WSH_SHELL_SYM_CANCEL) is not tested here because the
host example keeps ISIG enabled in its termios setup. With ISIG on the tty
discipline converts 0x03 to SIGINT before the byte ever reaches the shell
process, so raw-transport injection of ^C does not work reliably over PTY.
"""

import re

from .conftest import ShellSession

_HEXDUMP_LINE_RE = re.compile(r"^[0-9a-f]{8}  .+\|[^\|]+\|$")


# ── Hex dump ──────────────────────────────────────────────────────────────────


class TestHexDump:
    def test_dump_produces_hexdump_lines(self, session: ShellSession) -> None:
        result = session.run("dump")
        assert result.ok
        lines = result.text.splitlines()
        assert any(_HEXDUMP_LINE_RE.match(l.strip()) for l in lines), (
            f"No hexdump lines found in:\n{result.text}"
        )

    def test_dump_lines_have_ascii_section(self, session: ShellSession) -> None:
        result = session.run("dump")
        hex_lines = [l.strip() for l in result.text.splitlines() if re.match(r"^[0-9a-f]{8}\s", l.strip())]
        assert hex_lines
        assert all("|" in l for l in hex_lines)

    def test_dump_offset_starts_at_zero(self, session: ShellSession) -> None:
        result = session.run("dump")
        hex_lines = [l.strip() for l in result.text.splitlines() if re.match(r"^[0-9a-f]{8}\s", l.strip())]
        assert hex_lines
        assert hex_lines[0].startswith("00000000")


# ── Subcommand tree ───────────────────────────────────────────────────────────


class TestSubcommands:
    def test_wsh_user_list(self, session: ShellSession) -> None:
        result = session.run("wsh user list")
        assert result.ok
        assert result.text.strip()

    def test_wsh_user_whoami_shows_current_user(self, session: ShellSession) -> None:
        result = session.run("wsh user whoami")
        assert result.ok
        assert "root" in result.text.lower()

    def test_wsh_history_list(self, session: ShellSession) -> None:
        assert session.run("wsh history list").ok

    def test_wsh_history_clear(self, session: ShellSession) -> None:
        assert session.run("wsh history clear").ok

    def test_unknown_subcommand_emits_warn(self, session: ShellSession) -> None:
        result = session.run("wsh user nosuchsubcmd")
        assert "[WARN]" in result.text
        assert "nosuchsubcmd" in result.text


# ── Enum option type ──────────────────────────────────────────────────────────


class TestEnumOptions:
    def test_format_table_accepted(self, session: ShellSession) -> None:
        assert session.run("wsh user list --format table").ok

    def test_format_short_accepted(self, session: ShellSession) -> None:
        assert session.run("wsh user list --format short").ok

    def test_format_unknown_emits_warn(self, session: ShellSession) -> None:
        result = session.run("wsh user list --format nosuchformat")
        assert "[WARN]" in result.text
        assert "nosuchformat" in result.text
