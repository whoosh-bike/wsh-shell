import errno
import os
import pty
import select
import subprocess
from dataclasses import dataclass
from typing import Callable, Optional, Sequence

from .exceptions import TransportError


class BaseTransport:
    def open(self) -> None:
        raise NotImplementedError

    def close(self) -> None:
        raise NotImplementedError

    def write(self, data: bytes) -> int:
        raise NotImplementedError

    def read(self, size: int = 4096) -> bytes:
        raise NotImplementedError

    @property
    def is_open(self) -> bool:
        raise NotImplementedError

    @property
    def name(self) -> str:
        return "unknown"


@dataclass
class SerialDiscoveryConfig:
    vid: Optional[int] = None
    pid: Optional[int] = None


@dataclass(frozen=True)
class SerialPortInfo:
    device: str
    description: str = "unknown"
    vid: Optional[int] = None
    pid: Optional[int] = None

    @classmethod
    def from_pyserial(cls, item) -> "SerialPortInfo":
        return cls(
            device=str(item.device),
            description=getattr(item, "description", None) or "unknown",
            vid=getattr(item, "vid", None),
            pid=getattr(item, "pid", None),
        )


SerialPortChooser = Callable[[list[SerialPortInfo]], SerialPortInfo | str]


def _format_hex(value: Optional[int]) -> str:
    if value is None:
        return "n/a"
    return f"0x{value:04x}"


def format_serial_port(item: SerialPortInfo) -> str:
    return (
        f"{item.device} ({item.description}, "
        f"vid={_format_hex(item.vid)}({item.vid}), pid={_format_hex(item.pid)}({item.pid}))"
    )


def _list_pyserial_ports():
    try:
        from serial.tools import list_ports
    except Exception as exc:
        raise TransportError(
            "pyserial is required for serial port discovery. Install it with: pip install pyserial"
        ) from exc
    return list(list_ports.comports())


def list_serial_ports() -> list[SerialPortInfo]:
    return [SerialPortInfo.from_pyserial(item) for item in _list_pyserial_ports()]


def match_serial_ports(
    ports: Sequence[SerialPortInfo], vid: Optional[int] = None, pid: Optional[int] = None
) -> list[SerialPortInfo]:
    matched = []
    for item in ports:
        if vid is not None and item.vid != vid:
            continue
        if pid is not None and item.pid != pid:
            continue
        matched.append(item)
    return matched


def _auto_port_candidates(
    ports: Sequence[SerialPortInfo], prefer_usb: bool = True
) -> list[SerialPortInfo]:
    items = list(ports)
    if not prefer_usb:
        return items

    usb_like = [item for item in items if item.vid is not None or item.pid is not None]
    if usb_like:
        return usb_like
    return items


def _format_requested_vid_pid(vid: Optional[int], pid: Optional[int]) -> str:
    if vid is None and pid is None:
        return "auto"
    vid_part = f"vid={_format_hex(vid)}({vid})" if vid is not None else "vid=any"
    pid_part = f"pid={_format_hex(pid)}({pid})" if pid is not None else "pid=any"
    return f"VID/PID ({vid_part}, {pid_part})"


def _normalize_port_choice(
    choice: SerialPortInfo | str, candidates: Sequence[SerialPortInfo]
) -> str:
    selected = choice.device if isinstance(choice, SerialPortInfo) else str(choice)
    if any(item.device == selected for item in candidates):
        return selected
    raise TransportError(f"Selected serial port `{selected}` is not in the candidate list.")


def resolve_serial_port(
    *,
    port: Optional[str] = None,
    vid: Optional[int] = None,
    pid: Optional[int] = None,
    ports: Optional[Sequence[SerialPortInfo]] = None,
    chooser: Optional[SerialPortChooser] = None,
    prefer_usb: bool = True,
) -> str:
    if port:
        return port

    available_ports = list(ports) if ports is not None else list_serial_ports()
    matched = (
        match_serial_ports(available_ports, vid=vid, pid=pid)
        if vid is not None or pid is not None
        else _auto_port_candidates(available_ports, prefer_usb=prefer_usb)
    )

    if not matched:
        available = ", ".join(format_serial_port(item) for item in available_ports) or "none"
        if vid is None and pid is None:
            raise TransportError(f"No serial ports found. Available ports: {available}")
        raise TransportError(
            f"No serial ports match {_format_requested_vid_pid(vid, pid)}. "
            f"Available ports: {available}"
        )

    if len(matched) > 1 and chooser is not None:
        return _normalize_port_choice(chooser(matched), matched)

    return matched[0].device


class SerialTransport(BaseTransport):
    def __init__(
        self,
        port: Optional[str] = None,
        baudrate: int = 115200,
        read_timeout_s: float = 0.05,
        discovery: Optional[SerialDiscoveryConfig] = None,
    ) -> None:
        self._port = port
        self._baudrate = baudrate
        self._read_timeout_s = read_timeout_s
        self._discovery = discovery or SerialDiscoveryConfig()
        self._ser = None

    @property
    def is_open(self) -> bool:
        return bool(self._ser is not None and self._ser.is_open)

    @property
    def name(self) -> str:
        if self.is_open:
            return str(self._ser.port)
        return self._port or "auto"

    def open(self) -> None:
        if self.is_open:
            return

        serial = _import_serial_module()
        port = resolve_serial_port(
            port=self._port,
            vid=self._discovery.vid,
            pid=self._discovery.pid,
        )

        try:
            self._ser = serial.Serial(
                port=port,
                baudrate=self._baudrate,
                timeout=self._read_timeout_s,
                write_timeout=self._read_timeout_s,
            )
        except Exception as exc:
            raise TransportError(f"Failed to open serial port {port}: {exc}") from exc

        self._port = port

    def close(self) -> None:
        if self._ser is None:
            return
        try:
            if self._ser.is_open:
                self._ser.close()
        except Exception as exc:
            raise TransportError(f"Failed to close serial port: {exc}") from exc
        finally:
            self._ser = None

    def write(self, data: bytes) -> int:
        if not self.is_open:
            raise TransportError("Serial port is not open.")
        try:
            written = self._ser.write(data)
            self._ser.flush()
            return int(written)
        except Exception as exc:
            raise TransportError(f"Serial write failed: {exc}") from exc

    def read(self, size: int = 4096) -> bytes:
        if not self.is_open:
            raise TransportError("Serial port is not open.")
        try:
            return bytes(self._ser.read(size))
        except Exception as exc:
            raise TransportError(f"Serial read failed: {exc}") from exc


class PtyProcessTransport(BaseTransport):
    def __init__(
        self, cmd: list[str], cwd: Optional[str] = None, read_timeout_s: float = 0.05
    ) -> None:
        self._cmd = cmd
        self._cwd = cwd
        self._read_timeout_s = read_timeout_s
        self._proc: Optional[subprocess.Popen] = None
        self._master_fd: Optional[int] = None

    @property
    def is_open(self) -> bool:
        return self._proc is not None and self._proc.poll() is None and self._master_fd is not None

    @property
    def name(self) -> str:
        if self._proc is None:
            return "pty:closed"
        return f"pty:pid={self._proc.pid}"

    def open(self) -> None:
        if self.is_open:
            return

        master_fd, slave_fd = pty.openpty()
        try:
            self._proc = subprocess.Popen(
                self._cmd,
                cwd=self._cwd,
                stdin=slave_fd,
                stdout=slave_fd,
                stderr=slave_fd,
                close_fds=True,
            )
        except Exception as exc:
            os.close(master_fd)
            raise TransportError(f"Failed to start PTY process: {exc}") from exc
        finally:
            os.close(slave_fd)

        os.set_blocking(master_fd, False)
        self._master_fd = master_fd

    def close(self) -> None:
        if self._master_fd is not None:
            try:
                os.close(self._master_fd)
            except OSError:
                pass
            self._master_fd = None

        if self._proc is not None:
            if self._proc.poll() is None:
                self._proc.terminate()
                try:
                    self._proc.wait(timeout=1.0)
                except subprocess.TimeoutExpired:
                    self._proc.kill()
                    self._proc.wait(timeout=1.0)
            self._proc = None

    def write(self, data: bytes) -> int:
        if not self.is_open or self._master_fd is None:
            raise TransportError("PTY transport is not open.")
        try:
            return int(os.write(self._master_fd, data))
        except OSError as exc:
            raise TransportError(f"PTY write failed: {exc}") from exc

    def read(self, size: int = 4096) -> bytes:
        if not self.is_open or self._master_fd is None:
            raise TransportError("PTY transport is not open.")

        ready, _, _ = select.select([self._master_fd], [], [], self._read_timeout_s)
        if not ready:
            return b""

        try:
            return os.read(self._master_fd, size)
        except OSError as exc:
            if exc.errno in (errno.EIO, errno.EBADF):
                return b""
            raise TransportError(f"PTY read failed: {exc}") from exc


def _import_serial_module():
    try:
        import serial  # type: ignore
    except Exception as exc:
        raise TransportError(
            "pyserial is required for SerialTransport. Install it with: pip install pyserial"
        ) from exc
    return serial
