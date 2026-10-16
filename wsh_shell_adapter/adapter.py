import json
import re
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable, Dict, Mapping, Optional, Union

import toml

from .exceptions import CommandError, SyncError, WshShellAdapterError
from .shell_state import ENQ, STATE_BUSY as SHELL_BUSY, STATE_READY as SHELL_READY, ShellStateWatch, find_shell_states
from .transport import BaseTransport, SerialDiscoveryConfig, SerialTransport

ANSI_RE = re.compile(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
ANSI_LEGACY_RE = re.compile(r"\x1B[0-9]")
# No trailing `\s*$`: on a device that also streams async log lines over the
# same UART (e.g. `log -l trace`), those lines can land on the same physical
# line as the prompt — the device writes "user@host > " with no newline
# after it while it waits for input, and whatever prints next (a trace line)
# gets appended right there. A `$`-anchored pattern then never matches again
# once anything trails the prompt. `>` is excluded from the user/host
# character classes so a stray `>` inside trailing noise can't pull the match
# past the prompt's own `>`.
DEFAULT_PROMPT_REGEX = r"^(?:[^\r\n>]+@[^\r\n>]+\s*>|wsh-shell>)"
DEFAULT_CONFIG_PATH = Path(__file__).resolve().with_name("config.toml")
DEFAULT_SYNC_PROBE_COMMAND = "ping_command_and_response"

STATE_DISCONNECTED = "disconnected"
STATE_CONNECTED = "connected"
STATE_SYNCED = "synced"
STATE_EXECUTING = "executing"
# The shell said it is busy (a prompt-wait holds its input, e.g. a sniffer):
# the link is fine, but anything written would end that wait.
STATE_BUSY = "busy"

ADAPTER_TOML_KEYS = (
    "port",
    "baudrate",
    "bytesize",
    "parity",
    "stopbits",
    "read_timeout_s",
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
    "state_query",
    "state_query_timeout_s",
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
    bytesize: int = 8
    parity: str = "N"
    stopbits: int = 1
    read_timeout_s: float = 0.05
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
    # Fallback probe for a shell without the state line (older firmware).
    ping_command_and_response: Optional[tuple[str, str]] = None
    # Ask the shell for its state line (ENQ) before anything else in sync().
    state_query: bool = True
    state_query_timeout_s: float = 1.0
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
        io_callback: Optional[Callable[[str, Union[bytes, str]], None]] = None,
    ) -> None:
        self.config = config or AdapterConfig()
        self.transport = transport or SerialTransport(
            port=self.config.port,
            baudrate=self.config.baudrate,
            bytesize=self.config.bytesize,
            parity=self.config.parity,
            stopbits=self.config.stopbits,
            read_timeout_s=self.config.read_timeout_s,
            discovery=SerialDiscoveryConfig(vid=self.config.vid, pid=self.config.pid),
        )
        self._state_callback = state_callback
        # Sees every byte the adapter itself sends or reads ("tx", "rx",
        # "drain") and its state notes ("state", "recover"), for a host that
        # keeps a transcript. Settable after construction.
        self.io_callback = io_callback
        # MULTILINE so `^` matches after every "\n" in the accumulated buffer,
        # not just at the very start — needed now that prompt detection scans
        # the whole buffer instead of only its last line (see
        # _is_prompt_visible).
        self._prompt_re = re.compile(self.config.prompt_regex, re.MULTILINE)
        self.state = STATE_DISCONNECTED
        self.port_name = ""
        self.last_raw_response = b""
        # The last state line the shell printed (shell_state.py), None until it
        # has printed one. Older firmware never does.
        self.shell_state: Optional[Dict[str, Any]] = None
        # Whether this shell answers ENQ: None until known. Once True, sync()
        # trusts state lines and never falls back to the ping probe.
        self.state_line_supported: Optional[bool] = None

    def connect(self) -> None:
        self.transport.open()
        self.port_name = self.transport.name
        self._set_state(STATE_CONNECTED, f"transport opened on {self.port_name}")

    def close(self) -> None:
        self.transport.close()
        self._set_state(STATE_DISCONNECTED, "transport closed")

    @property
    def tx_bytes(self) -> int:
        return self.transport.tx_bytes

    @property
    def rx_bytes(self) -> int:
        return self.transport.rx_bytes

    def reset_io_counters(self) -> None:
        self.transport.reset_counters()

    def status(self) -> Dict[str, str]:
        return {
            "state": self.state,
            "port": self.port_name or self.transport.name,
            "tx_bytes": self.transport.tx_bytes,
            "rx_bytes": self.transport.rx_bytes,
        }

    def _ensure_ready(self, force_sync: bool = False) -> None:
        if not self.transport.is_open:
            self.connect()

        if force_sync or self.state != STATE_SYNCED:
            self.sync()

        if self.state == STATE_BUSY:
            # An explicit command outranks whatever holds the input: end the
            # wait (Enter does, for every prompt-wait) and sync again.
            self._log("state", "shell busy — ending its wait for an explicit command")
            self._write_raw(self.config.newline.encode("utf-8"))
            self.sync()

    def query_state(self, timeout_s: Optional[float] = None) -> Optional[Dict[str, Any]]:
        """Ask the shell for its state line (ENQ) without disturbing it.

        Safe in every mode: ENQ never reaches the command line, a login prompt
        or a prompt-wait. Returns the line, or None from a shell that doesn't
        answer (older firmware), leaving `state_line_supported` False.
        """
        if not self.transport.is_open:
            self.connect()
        timeout = timeout_s if timeout_s is not None else self.config.state_query_timeout_s
        self._write_raw(ENQ)
        watch = ShellStateWatch()
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            chunk = self.transport.read(self.config.read_chunk_size)
            if not chunk:
                time.sleep(self.config.read_poll_sleep_s)
                continue
            self._log("rx", chunk)
            state = watch.feed(self._clean_bytes(chunk))
            if state is not None:
                self._note_shell_state(state)
                self.state_line_supported = True
                return state
        if self.state_line_supported is None:
            self.state_line_supported = False
        return None

    def _note_shell_state(self, state: Dict[str, Any]) -> None:
        self.shell_state = state
        self._log("shell_state", json.dumps(state))

    def _settle_from_shell_state(self, state: Dict[str, Any]) -> bool:
        """Act on a state line during sync; True when sync is done."""
        kind = state.get("state")
        if kind == SHELL_READY:
            self._set_state(STATE_SYNCED, f"shell reports ready ({state.get('user')})")
            return True
        if kind == SHELL_BUSY:
            self._set_state(STATE_BUSY, f"shell busy ({state.get('busy')}) — left alone")
            return True
        return False

    def sync(self, timeout_s: Optional[float] = None) -> str:
        if not self.transport.is_open:
            self.connect()

        timeout = timeout_s if timeout_s is not None else self.config.sync_timeout_s
        deadline = time.monotonic() + timeout

        # Ask first, touch nothing: a shell that answers says whether it is
        # ready (done — no Enter, no ping), busy (done — nothing may be
        # written) or waiting for a login (handled below).
        if self.config.state_query and self.state_line_supported is not False:
            state = self.query_state(min(self.config.state_query_timeout_s, max(deadline - time.monotonic(), 0.0)))
            if state is not None and self._settle_from_shell_state(state):
                return ""

        buffer = bytearray()
        last_clean_text = ""
        last_enter_ts = 0.0
        watch = ShellStateWatch()

        while time.monotonic() < deadline:
            chunk = self.transport.read(self.config.read_chunk_size)
            if chunk:
                buffer.extend(chunk)
                self._log("rx", chunk)
                # A login prints "ready" by itself — that ends sync, no probe.
                state = watch.feed(self._clean_bytes(chunk))
                if state is not None:
                    self._note_shell_state(state)
                    self.state_line_supported = True
                    if self._settle_from_shell_state(state):
                        return self._clean_bytes(buffer)

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

                states = find_shell_states(clean_text)
                if states:
                    self._note_shell_state(states[-1])
                    self.state_line_supported = True

                if req.force_post_sync:
                    self.sync()
                elif states and states[-1].get("state") == SHELL_BUSY:
                    # The command started something that holds the input (a
                    # sniffer): the next command has to end it first.
                    self._set_state(STATE_BUSY, f"command left the shell busy ({states[-1].get('busy')})")
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

    def write_line(self, text: str) -> None:
        """Send one line without waiting for a reply — for a host that reads
        the port itself (a live terminal) rather than through `execute`."""
        self._write_line(text)

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
        # A shell with the state line confirms itself with it; the command
        # probe is only for one without (older firmware).
        if self.config.state_query and self.state_line_supported:
            state = self.query_state(timeout_s)
            if state is None or state.get("state") != SHELL_READY:
                raise SyncError(f"Shell state after sync is not ready: {state}")
            return

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

    def _last_prompt_match(self, clean_text: str) -> Optional[re.Match]:
        # Search the WHOLE buffer, not just its last line: once other async
        # output (e.g. trace logs sharing the UART) keeps arriving after the
        # real prompt, that prompt stops being "the last line" long before we
        # get a chance to notice it. Take the last match — if the prompt
        # printed more than once (e.g. after a `-h` help dump), that's the
        # one that actually terminates this read.
        matches = list(self._prompt_re.finditer(clean_text))
        return matches[-1] if matches else None

    def _is_prompt_visible(self, clean_text: str) -> bool:
        return self._last_prompt_match(clean_text) is not None

    def _extract_command_output(self, clean_text: str, command: str) -> str:
        text = clean_text.replace("\r", "")
        match = self._last_prompt_match(text)
        # Anything at/after the prompt is the prompt itself plus whatever
        # unrelated async output arrived after it — not part of this
        # command's response.
        body = text[: match.start()] if match else text

        lines = [line for line in body.split("\n") if line.strip()]
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

    def clean_text(self, data: Union[bytes, bytearray]) -> str:
        """Shell output as plain text: decoded, ANSI escapes and BEL removed."""
        return self._clean_bytes(data)

    def _clean_bytes(self, data: Union[bytes, bytearray]) -> str:
        text = bytes(data).decode("utf-8", errors="ignore")
        text = ANSI_RE.sub("", text)
        text = ANSI_LEGACY_RE.sub("", text)
        text = text.replace("\x07", "")  # BEL
        return text

    def _tail(self, text: str, size: int) -> str:
        return text[-size:] if len(text) > size else text

    def _log(self, topic: str, payload: Union[bytes, str]) -> None:
        if self.io_callback is not None:
            self.io_callback(topic, payload)
        if not self.config.debug:
            return
        if isinstance(payload, bytes):
            message = repr(payload)
        else:
            message = payload
        print(f"[wsh_shell_adapter] {topic}: {message}", file=sys.stderr)
