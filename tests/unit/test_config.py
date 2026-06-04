from pathlib import Path
from typing import Optional

import pytest

from wsh_shell_adapter import AdapterConfig, load_adapter_config_from_toml
from wsh_shell_adapter import run_local_shell as local_shell


def test_adapter_config_from_toml(tmp_path: Path) -> None:
    config_path = tmp_path / "cfg.toml"
    config_path.write_text(
        """
[adapter]
login = "root"
password = "1234"
retries = 5
prompt_regex = '.*@.* >\\s*$'
""",
        encoding="utf-8",
    )

    cfg = load_adapter_config_from_toml(config_path)

    assert cfg.login == "root"
    assert cfg.password == "1234"
    assert cfg.retries == 5
    assert cfg.prompt_regex == ".*@.* >\\s*$"


def test_load_settings_from_toml(monkeypatch: pytest.MonkeyPatch, tmp_path: Path) -> None:
    config_path = tmp_path / "cfg.toml"
    config_path.write_text(
        """
[adapter]
baudrate = 9600
vid = 0x0483
pid = 0x5740
login = "admin"
password = "secret"
command_timeout_s = 4.5
retries = 3
auto_recover = false
prompt_regex = '.*@.* >\\s*$'
sync_enter_interval_s = 0.11
sync_idle_enter_interval_s = 0.55
sync_poll_sleep_s = 0.07
read_poll_sleep_s = 0.08
pre_command_drain_s = 0.09
recover_soft_sync_timeout_cap_s = 1.5
recover_soft_command_timeout_cap_s = 1.6
recover_hard_sync_timeout_cap_s = 2.5
debug = true

[local_shell]
binary = "/tmp/example-bin"
cwd = "/tmp"
""",
        encoding="utf-8",
    )
    monkeypatch.setattr(local_shell, "DEFAULT_CONFIG_PATH", config_path)

    adapter_cfg, binary, cwd = local_shell._load_settings()

    assert adapter_cfg.baudrate == 9600
    assert adapter_cfg.vid == 0x0483
    assert adapter_cfg.pid == 0x5740
    assert adapter_cfg.login == "admin"
    assert adapter_cfg.password == "secret"
    assert adapter_cfg.command_timeout_s == 4.5
    assert adapter_cfg.retries == 3
    assert adapter_cfg.auto_recover is False
    assert adapter_cfg.prompt_regex == ".*@.* >\\s*$"
    assert adapter_cfg.sync_enter_interval_s == 0.11
    assert adapter_cfg.sync_idle_enter_interval_s == 0.55
    assert adapter_cfg.sync_poll_sleep_s == 0.07
    assert adapter_cfg.read_poll_sleep_s == 0.08
    assert adapter_cfg.pre_command_drain_s == 0.09
    assert adapter_cfg.recover_soft_sync_timeout_cap_s == 1.5
    assert adapter_cfg.recover_soft_command_timeout_cap_s == 1.6
    assert adapter_cfg.recover_hard_sync_timeout_cap_s == 2.5
    assert adapter_cfg.debug is True
    assert binary == "/tmp/example-bin"
    assert cwd == "/tmp"


def test_load_settings_without_config_uses_defaults(
    monkeypatch: pytest.MonkeyPatch, tmp_path: Path
) -> None:
    config_path = tmp_path / "missing.toml"
    monkeypatch.setattr(local_shell, "DEFAULT_CONFIG_PATH", config_path)

    adapter_cfg, binary, cwd = local_shell._load_settings()

    assert isinstance(adapter_cfg, AdapterConfig)
    assert binary == local_shell.DEFAULT_BINARY
    assert cwd == local_shell.DEFAULT_CWD


def test_local_shell_uses_config_file(monkeypatch: pytest.MonkeyPatch, tmp_path: Path) -> None:
    binary = tmp_path / "example-bin"
    binary.write_text("#!/bin/sh\n", encoding="utf-8")

    config_path = tmp_path / "cfg.toml"
    config_path.write_text(
        f"""
[adapter]
login = "cfg-user"
password = "cfg-pass"
retries = 7

[local_shell]
binary = "{binary}"
cwd = "{tmp_path}"
""",
        encoding="utf-8",
    )
    monkeypatch.setattr(local_shell, "DEFAULT_CONFIG_PATH", config_path)

    captured: dict[str, object] = {}

    class DummyTransport:
        def __init__(
            self, cmd: list[str], cwd: Optional[str] = None, read_timeout_s: float = 0.05
        ) -> None:
            captured["cmd"] = cmd
            captured["cwd"] = cwd

    class DummyAdapter:
        def __init__(self, config: AdapterConfig, transport: DummyTransport) -> None:
            captured["config"] = config
            captured["transport"] = transport

        def sync(self) -> None:
            captured["synced"] = True

        def close(self) -> None:
            captured["closed"] = True

    monkeypatch.setattr(local_shell, "PtyProcessTransport", DummyTransport)
    monkeypatch.setattr(local_shell, "WshShellAdapter", DummyAdapter)
    monkeypatch.setattr(local_shell, "_run_repl", lambda _: 0)

    exit_code = local_shell.main()

    assert exit_code == 0
    assert captured["cmd"] == [str(binary)]
    assert captured["cwd"] == str(tmp_path)

    cfg = captured["config"]
    assert isinstance(cfg, AdapterConfig)
    assert cfg.login == "cfg-user"
    assert cfg.password == "cfg-pass"
    assert cfg.retries == 7
    assert captured["synced"] is True
    assert captured["closed"] is True
