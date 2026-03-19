"""Public API for wsh_shell_adapter."""

from .adapter import (
    AdapterConfig,
    CommandRequest,
    CommandResult,
    WshShellAdapter,
    adapter_config_from_mapping,
    load_adapter_config_from_toml,
)
from .exceptions import CommandError, SyncError, TransportError, WshShellAdapterError
from .serial_cli import (
    describe_serial_target,
    prompt_for_serial_port,
)
from .transport import (
    BaseTransport,
    PtyProcessTransport,
    SerialDiscoveryConfig,
    SerialPortInfo,
    SerialTransport,
    format_serial_port,
    list_serial_ports,
    match_serial_ports,
    resolve_serial_port,
)

__all__ = [
    "AdapterConfig",
    "BaseTransport",
    "CommandError",
    "CommandRequest",
    "CommandResult",
    "adapter_config_from_mapping",
    "describe_serial_target",
    "format_serial_port",
    "load_adapter_config_from_toml",
    "PtyProcessTransport",
    "prompt_for_serial_port",
    "SerialDiscoveryConfig",
    "SerialPortInfo",
    "SerialTransport",
    "list_serial_ports",
    "match_serial_ports",
    "resolve_serial_port",
    "SyncError",
    "TransportError",
    "WshShellAdapter",
    "WshShellAdapterError",
]
