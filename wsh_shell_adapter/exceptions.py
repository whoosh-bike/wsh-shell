
class WshShellAdapterError(Exception):
    """Base adapter exception."""


class TransportError(WshShellAdapterError):
    """Transport-level problem (open/read/write/close)."""


class SyncError(WshShellAdapterError):
    """Shell sync/auth problem."""


class CommandError(WshShellAdapterError):
    """Command execution problem."""
