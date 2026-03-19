from typing import Callable, Optional, Sequence

from .transport import SerialPortInfo, format_serial_port

SerialPortInput = Callable[[str], str]
SerialPortOutput = Callable[[str], object]


def _format_target_hex(value: Optional[int]) -> str:
    if value is None:
        return "any"
    return f"0x{value:04x}"


def describe_serial_target(
    *, port: Optional[str] = None, vid: Optional[int] = None, pid: Optional[int] = None
) -> str:
    if port:
        return f"Serial target: fixed port `{port}`"

    if vid is not None or pid is not None:
        vid_part = f"{_format_target_hex(vid)}({vid})" if vid is not None else "any"
        pid_part = f"{_format_target_hex(pid)}({pid})" if pid is not None else "any"
        return f"Serial target: auto by vid/pid (vid={vid_part}, pid={pid_part})"

    return "Serial target: auto-discover serial port (prefer USB devices)"


def prompt_for_serial_port(
    ports: Sequence[SerialPortInfo],
    *,
    input_fn: SerialPortInput = input,
    output_fn: SerialPortOutput = print,
) -> str:
    items = list(ports)
    output_fn("Available serial ports:")
    for index, item in enumerate(items, start=1):
        output_fn(f"  {index}. {format_serial_port(item)}")

    while True:
        try:
            raw = input_fn(f"Select port [1-{len(items)}]: ").strip()
        except (EOFError, KeyboardInterrupt) as exc:
            raise RuntimeError("Serial port selection cancelled.") from exc

        try:
            choice = int(raw)
        except ValueError:
            output_fn("Enter a number.")
            continue

        if 1 <= choice <= len(items):
            return items[choice - 1].device

        output_fn("Choice out of range.")
