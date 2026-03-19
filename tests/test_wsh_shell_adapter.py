import os
import shlex
import subprocess
from pathlib import Path

import pytest

from wsh_shell_adapter import (
    AdapterConfig,
    BaseTransport,
    CommandError,
    PtyProcessTransport,
    WshShellAdapter,
)


class FakeShellTransport(BaseTransport):
    def __init__(self, fail_first_command: bool = False) -> None:
        self._is_open = False
        self._out = bytearray()
        self._line = bytearray()
        self._state = "wait_enter"
        self._login = ""
        self._fail_first_command = fail_first_command
        self._failed = False

    @property
    def is_open(self) -> bool:
        return self._is_open

    @property
    def name(self) -> str:
        return "fake-uart"

    def open(self) -> None:
        self._is_open = True
        self._enqueue("\x07Press <Enter> to log in...\r\n")

    def close(self) -> None:
        self._is_open = False

    def write(self, data: bytes) -> int:
        for byte in data:
            if byte in (ord("\r"), ord("\n")):
                self._process_line(self._line.decode("utf-8", errors="ignore"))
                self._line.clear()
            else:
                self._line.append(byte)
        return len(data)

    def read(self, size: int = 4096) -> bytes:
        if not self._out:
            return b""
        chunk = bytes(self._out[:size])
        del self._out[:size]
        return chunk

    def _process_line(self, line: str) -> None:
        line = line.strip()

        if self._state == "wait_enter":
            if line:
                self._enqueue("Press <Enter> to continue...\r\n")
            else:
                self._enqueue("Login: ")
                self._state = "wait_login"
            return

        if self._state == "wait_login":
            self._login = line
            self._enqueue("Password: ")
            self._state = "wait_password"
            return

        if self._state == "wait_password":
            if self._login == "root" and line == "1234":
                self._enqueue("\x1b[36mtest-device\x1b[37m@\x1b[35mroot\x1b[0m > ")
                self._state = "synced"
            else:
                self._enqueue("Login: ")
                self._state = "wait_login"
            return

        if self._state != "synced":
            return

        if line == "wsh --ping":
            self._enqueue("wsh --ping\r\npong\r\ntest-device@root > ")
        elif self._fail_first_command and not self._failed:
            self._failed = True
            return
        elif line == "wsh":
            self._enqueue(
                "wsh\r\nVer: 2.5\r\nDevice name: test-device\r\nUser: root\r\n"
                "test-device@root > "
            )
        elif line == "json":
            self._enqueue('json\r\n{"a":1,"b":"x","c":true,"d":{"e":2}}\r\ntest-device@root > ')
        else:
            self._enqueue(f'{line}\r\nCommand "{line}" not found!\r\ntest-device@root > ')

    def _enqueue(self, text: str) -> None:
        self._out.extend(text.encode("utf-8"))


class FragmentedWriteTransport(BaseTransport):
    def __init__(self, inner: BaseTransport, first_chunk_size: int = 3) -> None:
        self._inner = inner
        self._first_chunk_size = first_chunk_size

    @property
    def is_open(self) -> bool:
        return self._inner.is_open

    @property
    def name(self) -> str:
        return self._inner.name

    def open(self) -> None:
        self._inner.open()

    def close(self) -> None:
        self._inner.close()

    def write(self, data: bytes) -> int:
        if len(data) <= self._first_chunk_size:
            return self._inner.write(data)
        left = data[: self._first_chunk_size]
        right = data[self._first_chunk_size :]
        written = self._inner.write(left)
        written += self._inner.write(right)
        return written

    def read(self, size: int = 4096) -> bytes:
        return self._inner.read(size)


def _build_assert_probe(repo_root: Path, tmp_path: Path) -> Path:
    assert_probe_source = """
    #include "wsh_shell.h"

    int main(void) {
        (void)WshShell_Init(NULL, "assert-probe", NULL, NULL);
        return 0;
    }
    """.lstrip()
    source = tmp_path / "assert_probe.c"
    binary = tmp_path / "assert_probe"
    source.write_text(assert_probe_source, encoding="utf-8")

    cc = shlex.split(os.environ.get("CC", "gcc"))
    cmd = [
        *cc,
        "-std=gnu11",
        "-O0",
        "-g",
        "-DDEBUG",
        "-DWSH_SHELL_ASSERT_ENABLE",
        "-include",
        str(repo_root / "src" / "wsh_shell_cfg_def.h"),
        f"-I{repo_root / 'src'}",
        f"-I{repo_root / 'example'}",
        str(source),
        *(str(path) for path in sorted((repo_root / "src").glob("*.c"))),
        "-o",
        str(binary),
    ]

    subprocess.run(cmd, cwd=repo_root, capture_output=True, text=True, check=True)
    return binary


def test_sync_and_execute_wsh() -> None:
    adapter = WshShellAdapter(
        config=AdapterConfig(login="root", password="1234", retries=0),
        transport=FakeShellTransport(),
    )

    adapter.sync()
    result = adapter.execute("wsh")

    assert result.ok
    assert result.text
    assert result.data is not None
    assert result.data["Ver"] == "2.5"
    assert result.data["User"] == "root"


def test_execute_json_output_to_dict() -> None:
    adapter = WshShellAdapter(
        config=AdapterConfig(login="root", password="1234", retries=0),
        transport=FakeShellTransport(),
    )
    adapter.sync()
    result = adapter.execute("json")

    assert result.data == {"a": 1, "b": "x", "c": True, "d": {"e": 2}}


def test_fragmented_write_command() -> None:
    adapter = WshShellAdapter(
        config=AdapterConfig(login="root", password="1234", retries=0),
        transport=FragmentedWriteTransport(FakeShellTransport(), first_chunk_size=2),
    )
    adapter.sync()
    result = adapter.execute("wsh")
    assert result.ok
    assert result.data is not None
    assert result.data["Ver"] == "2.5"


def test_retry_after_timeout() -> None:
    adapter = WshShellAdapter(
        config=AdapterConfig(
            login="root",
            password="1234",
            retries=1,
            command_timeout_s=0.15,
            auto_recover=False,
            ping_command_and_response=("wsh --ping", "pong"),
        ),
        transport=FakeShellTransport(fail_first_command=True),
    )
    adapter.sync()
    result = adapter.execute("wsh")

    assert result.ok
    assert result.attempts == 2


def test_raises_on_empty_command() -> None:
    adapter = WshShellAdapter(
        config=AdapterConfig(login="root", password="1234"),
        transport=FakeShellTransport(),
    )
    adapter.sync()
    with pytest.raises(CommandError):
        adapter.execute("   ")


def test_integration_with_example_binary_pty() -> None:
    repo_root = Path(__file__).resolve().parents[1]
    binary = repo_root / "example" / "build" / "example"
    if not binary.exists():
        pytest.skip("example binary not found; run `make example` first")

    transport = PtyProcessTransport([str(binary)], cwd=str(repo_root))
    adapter = WshShellAdapter(
        config=AdapterConfig(
            login="root",
            password="1234",
            retries=0,
            sync_timeout_s=8.0,
            command_timeout_s=3.0,
            auto_recover=False,
        ),
        transport=transport,
    )

    try:
        adapter.sync()
        result = adapter.execute("wsh")
        assert "Ver:" in result.text
        assert result.data is not None
        assert result.data.get("User") == "root"
    finally:
        adapter.close()


def test_wsh_shell_assert_exits_non_zero(tmp_path: Path) -> None:
    repo_root = Path(__file__).resolve().parents[1]
    binary = _build_assert_probe(repo_root, tmp_path)

    result = subprocess.run([str(binary)], cwd=repo_root, capture_output=True, text=True)

    output = "\n".join(part for part in (result.stdout, result.stderr) if part)
    assert result.returncode != 0, output or "assert probe exited successfully"
    assert "assert" in output.lower()
    assert "fail" in output.lower()
