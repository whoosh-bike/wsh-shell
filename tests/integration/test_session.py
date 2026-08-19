"""Tests for WSH_SHELL_SESSION — `wsh --keep`, a login that survives reboots.

Each test drives real example processes: the `--session <file>` option makes the
example store its session descriptor in a file instead of plain RAM, so killing
the process and starting it again models a device reboot with the no-init RAM
region intact.
"""

import time
from pathlib import Path
from typing import Iterator

import pytest

from wsh_shell_adapter import SyncError, WshShellAdapter

from .conftest import spawn_adapter


@pytest.fixture
def store(tmp_path: Path) -> Path:
    """Path of the file standing in for the device's no-init RAM."""
    return tmp_path / "session.bin"


class Device:
    """A single device whose reboots share one session store."""

    def __init__(self, store: Path) -> None:
        self._store = store
        self._adapter: WshShellAdapter | None = None

    def boot(self, *, credentials: bool = True) -> WshShellAdapter:
        """(Re)start the example. Without credentials only a restored session can log in."""
        self.shutdown()
        self._adapter = spawn_adapter(
            ["--session", str(self._store)],
            login="root" if credentials else None,
            password="1234" if credentials else None,
            # No credentials to fall back on: fail fast instead of retrying Enter.
            sync_timeout_s=10.0 if credentials else 4.0,
        )
        return self._adapter

    def boot_expecting_login_prompt(self) -> None:
        with pytest.raises(SyncError):
            self.boot(credentials=False)

    def shutdown(self) -> None:
        if self._adapter is not None:
            self._adapter.close()
            self._adapter = None


@pytest.fixture
def device(store: Path) -> Iterator[Device]:
    dev = Device(store)
    yield dev
    dev.shutdown()


def _status(adapter: WshShellAdapter) -> str:
    result = adapter.execute("wsh")
    assert result.ok
    return result.text


# ── Arming and reporting ─────────────────────────────────────────────────────


class TestSessionArm:
    def test_keep_is_acknowledged(self, device: Device) -> None:
        result = device.boot().execute("wsh --keep 2")
        assert result.ok
        assert "2" in result.text

    def test_status_reports_remaining_reboots(self, device: Device) -> None:
        adapter = device.boot()
        adapter.execute("wsh --keep 3")
        assert "Session keep: 3 reboot(s) left" in _status(adapter)

    def test_status_is_quiet_without_a_session(self, device: Device) -> None:
        assert "Session keep" not in _status(device.boot())

    def test_short_flag_matches_long_flag(self, device: Device) -> None:
        adapter = device.boot()
        adapter.execute("wsh -k 2")
        assert "Session keep: 2 reboot(s) left" in _status(adapter)


# ── Surviving a reboot ───────────────────────────────────────────────────────


class TestSessionRestore:
    def test_login_survives_a_reboot(self, device: Device) -> None:
        device.boot().execute("wsh --keep 2")

        adapter = device.boot(credentials=False)
        assert "User: root" in _status(adapter)

    def test_restore_spends_one_reboot(self, device: Device) -> None:
        device.boot().execute("wsh --keep 2")

        adapter = device.boot(credentials=False)
        assert "Session keep: 1 reboot(s) left" in _status(adapter)

    def test_budget_runs_out(self, device: Device) -> None:
        device.boot().execute("wsh --keep 1")

        # The single budgeted reboot still restores...
        assert "User: root" in _status(device.boot(credentials=False))
        # ...but it was the last one.
        device.boot_expecting_login_prompt()

    def test_no_session_means_no_restore(self, device: Device) -> None:
        device.boot()
        device.boot_expecting_login_prompt()


# ── Clearing ─────────────────────────────────────────────────────────────────


class TestSessionClear:
    def test_keep_zero_clears_the_session(self, device: Device) -> None:
        adapter = device.boot()
        adapter.execute("wsh --keep 2")
        assert adapter.execute("wsh --keep 0").ok
        assert "Session keep" not in _status(adapter)

        device.boot_expecting_login_prompt()

    def test_logout_clears_the_session(self, device: Device) -> None:
        adapter = device.boot()
        adapter.execute("wsh --keep 2")
        # Not execute(): a logout leaves the shell at the login banner, so no
        # prompt ever comes back for the adapter to wait on.
        adapter.transport.write(b"wsh --deauth\r")
        time.sleep(0.3)

        device.boot_expecting_login_prompt()

    def test_corrupt_store_is_ignored(self, device: Device, store: Path) -> None:
        device.boot().execute("wsh --keep 2")
        device.shutdown()

        # Flip the payload without fixing up the hash — what uninitialised RAM
        # or a torn write looks like.
        raw = bytearray(store.read_bytes())
        raw[-1] ^= 0xFF
        store.write_bytes(bytes(raw))

        device.boot_expecting_login_prompt()
