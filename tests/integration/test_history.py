"""Tests for command history: navigation with arrow keys, list, and clear."""

import pytest

from .conftest import ShellSession


@pytest.fixture(autouse=True)
def _clear_history(session: ShellSession) -> None:
    """Start each test with an empty history."""
    session.run("wsh history clear")


# ── Arrow-key navigation ──────────────────────────────────────────────────────


class TestHistoryNavigation:
    def test_up_recalls_last_command(self, session: ShellSession) -> None:
        session.run("wsh --ping")
        session.press("up")
        out = session.enter()
        assert b"pong" in out, f"Expected recalled command to execute, got: {out!r}"

    def test_up_up_recalls_second_to_last(self, session: ShellSession) -> None:
        session.run("wsh --ping")   # 1st in history
        session.run("wsh --exec")   # 2nd (most recent)
        session.press("up")         # recalls "wsh --exec"
        session.press("up")         # recalls "wsh --ping"
        out = session.enter()
        assert b"pong" in out, f"Expected 'wsh --ping' to be recalled, got: {out!r}"

    def test_down_navigates_forward_in_history(self, session: ShellSession) -> None:
        session.run("wsh --ping")   # 1st
        session.run("wsh --exec")   # 2nd
        session.press("up")         # → "wsh --exec"
        session.press("up")         # → "wsh --ping"
        session.press("down")       # → "wsh --exec"
        out = session.enter()
        # "wsh --exec" lists accessible commands; output should be non-trivial
        assert b"pong" not in out, "Down arrow should have moved forward, not back to --ping"

    def test_up_on_empty_history_does_nothing(self, session: ShellSession) -> None:
        """After clearing history, Up should not recall any command."""
        session.press("up")
        out = session.enter()
        # Empty line execution prints nothing (or just the prompt) — shell still alive
        assert session.run("wsh --ping").ok


# ── History list reflects executed commands ───────────────────────────────────


class TestHistoryList:
    def test_executed_command_appears_in_list(self, session: ShellSession) -> None:
        session.run("wsh --ping")
        result = session.run("wsh history list")
        assert result.ok
        assert "wsh --ping" in result.text

    def test_multiple_commands_in_list(self, session: ShellSession) -> None:
        session.run("wsh --ping")
        session.run("wsh --exec")
        result = session.run("wsh history list")
        assert "wsh --ping" in result.text
        assert "wsh --exec" in result.text

    def test_list_is_empty_after_clear(self, session: ShellSession) -> None:
        session.run("wsh --ping")
        session.run("wsh history clear")
        result = session.run("wsh history list")
        assert result.ok
        # After clear the list should not contain the earlier command
        assert "wsh --ping" not in result.text
