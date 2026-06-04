from __future__ import annotations

from typing import Optional

from .adapter import AdapterConfig
from .exceptions import TransportError
from .transport import SerialDiscoveryConfig, SerialTransport


class RawSerialAdapter:

    def __init__(self, config: Optional[AdapterConfig] = None) -> None:
        self.config = config or AdapterConfig()
        self._transport: Optional[SerialTransport] = None
        self._port: Optional[str] = None

    @property
    def is_open(self) -> bool:
        return self._transport is not None and self._transport.is_open

    @property
    def port(self) -> Optional[str]:
        return self._port

    @property
    def in_waiting(self) -> int:
        if self._transport is None:
            return 0
        return self._transport.in_waiting

    def connect(self) -> None:
        if self._transport is not None:
            try:
                self._transport.close()
            except Exception:
                pass
        transport = SerialTransport(
            port=self.config.port,
            baudrate=self.config.baudrate,
            bytesize=self.config.bytesize,
            parity=self.config.parity,
            stopbits=self.config.stopbits,
            read_timeout_s=self.config.read_timeout_s,
            discovery=SerialDiscoveryConfig(vid=self.config.vid, pid=self.config.pid),
        )
        transport.open()
        self._transport = transport
        self._port = transport.name

    def disconnect(self) -> None:
        if self._transport is not None:
            try:
                self._transport.close()
            except Exception:
                pass
            self._transport = None
            self._port = None

    def write(self, data: bytes) -> int:
        if not self.is_open:
            raise TransportError("Not connected. Call connect() first.")
        return self._transport.write(data)

    def read(self, size: int = 4096) -> bytes:
        if not self.is_open:
            raise TransportError("Not connected. Call connect() first.")
        return self._transport.read(size)

    def reset_input_buffer(self) -> None:
        if self._transport is not None:
            self._transport.reset_input_buffer()

    @property
    def tx_bytes(self) -> int:
        return self._transport.tx_bytes if self._transport is not None else 0

    @property
    def rx_bytes(self) -> int:
        return self._transport.rx_bytes if self._transport is not None else 0

    def reset_io_counters(self) -> None:
        if self._transport is not None:
            self._transport.reset_counters()

    def status(self) -> dict:
        return {
            "connected": self.is_open,
            "port": self._port,
            "tx_bytes": self.tx_bytes,
            "rx_bytes": self.rx_bytes,
        }
