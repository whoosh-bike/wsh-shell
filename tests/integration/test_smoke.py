"""Smoke tests: binary starts, authenticates, and responds."""

from wsh_shell_adapter import WshShellAdapter

from .conftest import ShellSession


def test_auth_and_version(adapter: WshShellAdapter) -> None:
    result = adapter.execute("wsh")
    assert result.ok
    assert "Ver:" in result.text
    assert "root" in result.text


def test_ping_responds_pong(session: ShellSession) -> None:
    result = session.run("wsh --ping")
    assert result.ok
    assert "pong" in result.text.lower()


def test_list_commands_non_empty(session: ShellSession) -> None:
    result = session.run("wsh --exec")
    assert result.ok
    assert result.text.strip()
