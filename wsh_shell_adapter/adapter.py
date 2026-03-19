import json
import re
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable, Dict, Mapping, Optional, Union

import toml

from .exceptions import CommandError, SyncError, WshShellAdapterError
from .transport import BaseTransport, SerialDiscoveryConfig, SerialTransport

ANSI_RE = re.compile(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
ANSI_LEGACY_RE = re.compile(r"\x1B[0-9]")
DEFAULT_PROMPT_REGEX = r"^(?:[^\r\n]+@[^\r\n]+\s*>|wsh-shell>)\s*$"
DEFAULT_CONFIG_PATH = Path(__file__).resolve().with_name("config.toml")
DEFAULT_SYNC_PROBE_COMMAND = "ping_command_and_response"

STATE_DISCONNECTED = "disconnected"
STATE_CONNECTED = "connected"
STATE_SYNCED = "synced"
STATE_EXECUTING = "executing"

ADAPTER_TOML_KEYS = (
    "port",
    "baudrate",
    "vid",
    "pid",
    "login",
    "password",
    "newline",
    "sync_timeout_s",
    "command_timeout_s",
    "retries",
    "read_chunk_size",
    "auto_recover",
    "ping_command_and_response",
    "prompt_regex",
    "sync_enter_interval_s",
    "sync_idle_enter_interval_s",
    "sync_poll_sleep_s",
    "read_poll_sleep_s",
    "pre_command_drain_s",
    "recover_soft_sync_timeout_cap_s",
    "recover_soft_command_timeout_cap_s",
    "recover_hard_sync_timeout_cap_s",
    "debug",
)


@dataclass
class AdapterConfig:
    port: Optional[str] = None
    baudrate: int = 115200
    vid: Optional[int] = None
    pid: Optional[int] = None
    login: Optional[str] = None
    password: Optional[str] = None
    newline: str = "\r"
    sync_timeout_s: float = 6.0
    command_timeout_s: float = 3.0
    retries: int = 1
    read_chunk_size: int = 4096
    auto_recover: bool = True
    ping_command_and_response: Optional[tuple[str, str]] = None
    prompt_regex: str = DEFAULT_PROMPT_REGEX
    sync_enter_interval_s: float = 0.2
    sync_idle_enter_interval_s: float = 0.6
    sync_poll_sleep_s: float = 0.03
    read_poll_sleep_s: float = 0.02
    pre_command_drain_s: float = 0.05
    recover_soft_sync_timeout_cap_s: float = 2.0
    recover_soft_command_timeout_cap_s: float = 2.0
    recover_hard_sync_timeout_cap_s: float = 3.0
    debug: bool = False


def _normalize_ping_command_and_response(value: Any) -> Optional[tuple[str, str]]:
    if value is None:
        return None

    if isinstance(value, (list, tuple)) and len(value) == 0:
        return None

    if not isinstance(value, (list, tuple)) or len(value) != 2:
        raise ValueError("`ping_command_and_response` must be an array of two strings.")

    command, expected = value
    if not isinstance(command, str) or not isinstance(expected, str):
        raise ValueError("`ping_command_and_response` must be an array of two strings.")

    command = command.strip()
    if not command:
        raise ValueError("`ping_command_and_response[0]` must not be empty.")

    return (command, expected)


def adapter_config_from_mapping(data: Mapping[str, Any]) -> AdapterConfig:
    cfg = AdapterConfig()
    for key in ADAPTER_TOML_KEYS:
        if key in data:
            if key == "ping_command_and_response":
                setattr(cfg, key, _normalize_ping_command_and_response(data[key]))
            else:
                setattr(cfg, key, data[key])
    return cfg


def _load_toml_document(path: Optional[Union[str, Path]] = None) -> dict[str, Any]:
    cfg_path = Path(path) if path is not None else DEFAULT_CONFIG_PATH
    if not cfg_path.exists():
        return {}

    try:
        doc = toml.loads(cfg_path.read_text(encoding="utf-8"))
    except Exception as exc:
        raise ValueError(f"Failed to parse TOML config `{cfg_path}`: {exc}") from exc

    if not isinstance(doc, dict):
        raise ValueError(f"Invalid TOML config `{cfg_path}`.")

    return doc


def load_adapter_config_from_toml(
    path: Optional[Union[str, Path]] = None,
    return_toml_data: bool = False,
) -> AdapterConfig | tuple[AdapterConfig, dict[str, Any]]:
    doc = _load_toml_document(path)

    adapter_data = doc.get("adapter", {})
    if adapter_data is None:
        adapter_data = {}
    if not isinstance(adapter_data, dict):
        raise ValueError("`adapter` section must be a table.")

    adapter_cfg = adapter_config_from_mapping(adapter_data)
    if return_toml_data:
        return adapter_cfg, doc
    return adapter_cfg


@dataclass
class CommandRequest:
    body: str
    timeout_s: Optional[float] = None
    retries: Optional[int] = None
    force_pre_sync: bool = False
    force_post_sync: bool = False
    parse_to_dict: bool = True


@dataclass
class CommandResult:
    ok: bool
    command: str
    text: str
    data: Optional[Any]
    attempts: int
    duration_ms: int
    port: str
    state: str


class WshShellAdapter:

    def __init__(
        self,
        config: Optional[AdapterConfig] = None,
        transport: Optional[BaseTransport] = None,
        state_callback: Optional[Callable[[str, str], None]] = None,
    ) -> None:
        self.config = config or AdapterConfig()
        self.transport = transport or SerialTransport(
            port=self.config.port,
            baudrate=self.config.baudrate,
            discovery=SerialDiscoveryConfig(vid=self.config.vid, pid=self.config.pid),
        )
        self._state_callback = state_callback
        self._prompt_re = re.compile(self.config.prompt_regex)
        self.state = STATE_DISCONNECTED
        self.port_name = ""
        self.last_raw_response = b""

    def connect(self) -> None:
        self.transport.open()
        self.port_name = self.transport.name
        self._set_state(STATE_CONNECTED, f"transport opened on {self.port_name}")

    def close(self) -> None:
        self.transport.close()
        self._set_state(STATE_DISCONNECTED, "transport closed")

    def status(self) -> Dict[str, str]:
        return {"state": self.state, "port": self.port_name or self.transport.name}

    def _ensure_ready(self, force_sync: bool = False) -> None:
        if not self.transport.is_open:
            self.connect()

        if force_sync or self.state != STATE_SYNCED:
            self.sync()

    def sync(self, timeout_s: Optional[float] = None) -> str:
        if not self.transport.is_open:
            self.connect()

        timeout = timeout_s if timeout_s is not None else self.config.sync_timeout_s
        deadline = time.monotonic() + timeout
        buffer = bytearray()
        last_clean_text = ""
        last_enter_ts = 0.0

        while time.monotonic() < deadline:
            chunk = self.transport.read(self.config.read_chunk_size)
            if chunk:
                buffer.extend(chunk)
                self._log("rx", chunk)

            clean_text = self._clean_bytes(buffer)
            if clean_text:
                last_clean_text = clean_text
            if self._is_prompt_visible(clean_text):
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    raise SyncError("Sync timeout before probe verification.")
                self._verify_sync_probe(timeout_s=min(self.config.command_timeout_s, remaining))
                self._set_state(STATE_SYNCED, "prompt detected and sync probe verified")
                return clean_text

            if "Login:" in clean_text:
                if not self.config.login:
                    raise SyncError("Login prompt detected, but login is not configured.")
                self._write_line(self.config.login)
                buffer.clear()
                continue

            if "Password:" in clean_text:
                if not self.config.password:
                    raise SyncError("Password prompt detected, but password is not configured.")
                self._write_line(self.config.password)
                buffer.clear()
                continue

            if (
                "Press <Enter> to log in..." in clean_text
                or "Press <Enter> to continue..." in clean_text
            ) and (time.monotonic() - last_enter_ts > self.config.sync_enter_interval_s):
                self._write_raw(self.config.newline.encode("utf-8"))
                last_enter_ts = time.monotonic()
                continue

            if not chunk and (
                time.monotonic() - last_enter_ts > self.config.sync_idle_enter_interval_s
            ):
                self._write_raw(self.config.newline.encode("utf-8"))
                last_enter_ts = time.monotonic()

            time.sleep(self.config.sync_poll_sleep_s)

        tail_source = last_clean_text or self._clean_bytes(buffer)
        raise SyncError(f"Sync timeout. Last shell text: {self._tail(tail_source, 200)}")

    def execute(self, request: Union[str, CommandRequest]) -> CommandResult:
        req = request if isinstance(request, CommandRequest) else CommandRequest(body=request)

        if not req.body or not req.body.strip():
            raise CommandError("Command is empty.")

        retries = req.retries if req.retries is not None else self.config.retries
        timeout = req.timeout_s if req.timeout_s is not None else self.config.command_timeout_s

        attempt = 0
        start = time.monotonic()
        last_error: Optional[Exception] = None

        while attempt <= retries:
            attempt += 1
            try:
                self._ensure_ready(force_sync=req.force_pre_sync)

                self._set_state(STATE_EXECUTING, f"running `{req.body}`")
                self._drain_input(self.config.pre_command_drain_s)
                self._write_line(req.body)

                raw, clean_text = self._read_until_prompt(timeout_s=timeout)
                self.last_raw_response = raw
                output_text = self._extract_command_output(clean_text, req.body)
                parsed = self._parse_to_dict(output_text) if req.parse_to_dict else None

                if req.force_post_sync:
                    self.sync()
                else:
                    self._set_state(STATE_SYNCED, "command completed")

                return CommandResult(
                    ok=True,
                    command=req.body,
                    text=output_text,
                    data=parsed,
                    attempts=attempt,
                    duration_ms=int((time.monotonic() - start) * 1000),
                    port=self.port_name or self.transport.name,
                    state=self.state,
                )
            except WshShellAdapterError as exc:
                last_error = exc
                self._set_state(STATE_CONNECTED, f"attempt {attempt} failed: {exc}")
                if attempt <= retries:
                    self._recover()
                    continue

        raise CommandError(f"Command failed after {attempt} attempt(s): {last_error}")

    def _recover(self) -> None:
        if not self.config.auto_recover:
            return

        try:
            self.sync(
                timeout_s=min(
                    self.config.recover_soft_sync_timeout_cap_s, self.config.sync_timeout_s
                )
            )
            self._set_state(STATE_SYNCED, "soft recovery is successful")
            return
        except Exception as exc:
            self._log("recover", f"soft recovery failed: {exc}")

        self.close()
        self.connect()
        self.sync(
            timeout_s=min(self.config.recover_hard_sync_timeout_cap_s, self.config.sync_timeout_s)
        )

    def _read_until_prompt(self, timeout_s: float) -> tuple[bytes, str]:
        deadline = time.monotonic() + timeout_s
        buffer = bytearray()

        while time.monotonic() < deadline:
            chunk = self.transport.read(self.config.read_chunk_size)
            if chunk:
                buffer.extend(chunk)
                self._log("rx", chunk)
                clean_text = self._clean_bytes(buffer)
                if self._is_prompt_visible(clean_text):
                    return bytes(buffer), clean_text
            else:
                time.sleep(self.config.read_poll_sleep_s)

        raise CommandError(f"Timeout waiting for command response/prompt ({timeout_s}s).")

    def _drain_input(self, duration_s: float) -> None:
        deadline = time.monotonic() + duration_s
        while time.monotonic() < deadline:
            chunk = self.transport.read(self.config.read_chunk_size)
            if not chunk:
                return
            self._log("drain", chunk)

    def _write_line(self, text: str) -> None:
        payload = (text + self.config.newline).encode("utf-8")
        self._write_raw(payload)

    def _write_raw(self, payload: bytes) -> None:
        self._log("tx", payload)
        self.transport.write(payload)

    def _run_probe_command(self, command: str, timeout_s: float) -> str:
        self._drain_input(self.config.pre_command_drain_s)
        self._write_line(command)
        raw, clean_text = self._read_until_prompt(timeout_s=timeout_s)
        self.last_raw_response = raw
        return self._extract_command_output(clean_text, command)

    def _verify_sync_probe(self, timeout_s: float) -> None:
        ping_probe = self.config.ping_command_and_response
        if ping_probe is None:
            output_text = self._run_probe_command(DEFAULT_SYNC_PROBE_COMMAND, timeout_s=timeout_s)
            if not output_text.strip():
                raise SyncError(
                    "Sync probe returned empty output. " f"Command: `{DEFAULT_SYNC_PROBE_COMMAND}`."
                )
            return

        command, expected_response = ping_probe
        output_text = self._run_probe_command(command, timeout_s=timeout_s)
        normalized_output = output_text.strip()
        normalized_expected = expected_response.strip()
        output_lines = [line.strip() for line in normalized_output.splitlines() if line.strip()]
        if normalized_output != normalized_expected and normalized_expected not in output_lines:
            raise SyncError(
                "Sync ping returned unexpected output. "
                f"Command: `{command}`. Expected response: `{expected_response}`. "
                f"Actual output: `{self._tail(output_text, 200)}`"
            )

    def _set_state(self, state: str, reason: str) -> None:
        self.state = state
        if self._state_callback is not None:
            self._state_callback(state, reason)
        self._log("state", f"{state}: {reason}")

    def _is_prompt_visible(self, clean_text: str) -> bool:
        line = self._last_non_empty_line(clean_text)
        if not line:
            return False
        return bool(self._prompt_re.search(line))

    def _extract_command_output(self, clean_text: str, command: str) -> str:
        text = clean_text.replace("\r", "")
        lines = [line for line in text.split("\n") if line.strip()]
        if not lines:
            return ""

        if self._is_prompt_visible(lines[-1]):
            lines.pop()

        if lines and lines[0].strip() == command.strip():
            lines.pop(0)

        return "\n".join(lines).strip()

    def _parse_to_dict(self, text: str) -> Optional[Any]:
        stripped = text.strip()
        if not stripped:
            return None

        if stripped.startswith("{") or stripped.startswith("["):
            try:
                obj = json.loads(stripped)
                return obj
            except Exception:
                pass

        parsed: Dict[str, Any] = {}
        for line in stripped.splitlines():
            match = re.match(r"^\s*([A-Za-z0-9_.\-/ ]+?)\s*:\s*(.*?)\s*$", line)
            if match:
                key = match.group(1).strip()
                value = match.group(2).strip()
                parsed[key] = value

        return parsed or None

    def _clean_bytes(self, data: Union[bytes, bytearray]) -> str:
        text = bytes(data).decode("utf-8", errors="ignore")
        text = ANSI_RE.sub("", text)
        text = ANSI_LEGACY_RE.sub("", text)
        text = text.replace("\x07", "")  # BEL
        return text

    def _last_non_empty_line(self, text: str) -> str:
        lines = [line for line in text.replace("\r", "").split("\n") if line.strip()]
        return lines[-1] if lines else ""

    def _tail(self, text: str, size: int) -> str:
        return text[-size:] if len(text) > size else text

    def _log(self, topic: str, payload: Union[bytes, str]) -> None:
        if not self.config.debug:
            return
        if isinstance(payload, bytes):
            message = repr(payload)
        else:
            message = payload
        print(f"[wsh_shell_adapter] {topic}: {message}", file=sys.stderr)
